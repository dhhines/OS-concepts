/**
 * @author David Hines
 * @file coordinator.c
 *
 * This program simulates riders in an amusement park taking turns
 * riding bumper cars and walking around the park.  There are 3 command
 * line arguments that will determine the number of cars available
 * (bounded buffer), the number of riders (threads), and the time duration
 * of the simulation.
 *
 * The project is focused on solving the bounded buffer problem with monitors
 * and thread synchronization using POSIX C mutex from the pthread library.
 *
 * Compilation: use provided Makefile
 *              -usage: "make" or "make clean"
 *
 */

#include "sleeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//global bounded buffer as int array of number bumper cars in line
//0 will be used to represent no car in the array slot
//this will be a FIFO buffer
int *availCars;

//create mutex data structure for rider data
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

//create mutex data structure for car buffer
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

//conditional variables used for the monitor of availCars buffer
pthread_cond_t none = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

//total number of cars for riders from command line arg
int cars;

//number of cars left in the buffer for use
int numCars;

//current slot to get a cars from buffer
int get = 0;

//slot to put a car back in buffer
int put = 0;

//the rider (thread) number
int rider = 1;

//rider struct for tracking the rider information (thread)
//riderNum stores the rider number
//carID stores the id of the car in use
typedef struct Rider_Struct{
   int riderNum;
   int carID;
}Rider;


int getInLine();

void returnCar (int carID);

void * riderFunc();

/**
 * Main program that starts the park visit (test rider)
 */
int main(int argc, char* argv[]) {

   //count of bumper cars from command line arg
   cars = atoi(argv[1]);
   numCars = cars;

   //initialize car array to number of cars using command line arg
   availCars = (int *) malloc (cars * sizeof(int));

   //populate the cars bounded buffer with the carID numbers
   for (int i = 0; i < atoi(argv[1]); i++)
      availCars[i] = i + 1;

   //create array for thread IDs using command line arg for number of riders
   pthread_t riderThrds[atoi(argv[2])];

   //create and initialize the thread attributes variable
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   //create the correct number of threads as riders
   for (int i = 0; i < atoi(argv[2]); i++)
      pthread_create(&riderThrds[i], &attr, riderFunc, NULL);

   //main process sleeps for the time provided on command line
   sleep(atoi(argv[3]));

   //free the memory used for availCars array then exit program
   free(availCars);
   return EXIT_SUCCESS;
}


/**
 * function called by each thread to visit the park as a rider
 * and call the other functions related to riding cars or walking
 * around the park to kill time.
 */
void * riderFunc(){

   //struct for holding local data for a thread
   Rider r;

   //critical section where thread updates rider count and uses
   //that rider count to store the thread rider number in a struct
   pthread_mutex_lock(&m1);

      //set the riderNum using globar rider value
      r.riderNum = rider;

      //increment the rider value (for next thread)
      rider++;

   //release the mutex lock for allocating rider number
   pthread_mutex_unlock(&m1);

   //loop while the coordinator continues to run (until main process exits)
   while (1){

      printf("Rider %d is walking around the park. \n", r.riderNum);

      walkAroundTime();

      r.carID = getInLine();

      printf("Rider %d is now riding in car %d. \n", r.riderNum, r.carID);
      rideTime();

      printf("Rider %d returned car %d. \n", r.riderNum, r.carID);
      returnCar(r.carID);

   }
}


/**
 * Function for the rider (thread) to get in line for an available
 * bumper car.  If there are no available cars then the thread will
 * block until a car comes availabe.  A signal will be use to make the
 * thread aware a car has been added to the buffer.
 *
 * @return the carID is returned as integer value to caller
 */
int getInLine(){

   int carID;

   pthread_mutex_lock(&m2);

   //if the buffer of cars is empty then block until signal "none"
   while (numCars == 0)
      pthread_cond_wait(&none, &m2);

   carID = availCars[get];
   get = (get + 1) % cars;
   numCars--;

   pthread_mutex_unlock(&m2);

   pthread_cond_signal(&full);

   return carID;
}


/**
 * This function is essentially the producer and will ensure that
 * cars returned by threads will be placed back into the buffer
 * availCars.  If there are threads waiting for available cars then
 * this function will also unblock a waiting rider (thread).
 *
 * @param carID the integer value for the car in use is passed and added to buffer
 */
void returnCar (int carID){

   pthread_mutex_lock(&m2);

   //if the buffer is full then block until signal "full"
   while(numCars == cars)
      pthread_cond_wait(&full, &m2);

   availCars[put] = carID;
   put = (put + 1) % cars;
   numCars++;

   //release mutex lock
   pthread_mutex_unlock(&m2);
   //signal that there is a car now
   pthread_cond_signal(&none);
}
