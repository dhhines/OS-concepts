/**
 * @author David Hines
 * @file barrier.cpp
 *
 * This program demonstrates monitors to syncronize threads by creating a
 * barrier where threads wait for signaling before continuing.  The threads
 * are created then passed the Restaurant function which in turn calls the
 * waitForOthers function.  In waitForOthers, the threads will attempt to
 * lock the mutex and then will update the shared data of totalThreads and
 * taskWhileWaiting.  The global data is then used to set some local variables
 * for the thread and print out the thread number, task and state of execution.
 *
 * All of the threads will wait for a signal until the last thread arrives. The
 * last thread will unlock the mutex and also signal to the other threads using
 * the pthread_cond_t notFull signal.  When the signal is sent, the first threads
 * will continue processing until exit.
 *
 * Computing Environment:     NCSU College of Engineering EOS Computing Environment
 * Compile command:           g++ -Wall -std=c++11 barrier.cpp -o barrier -lpthread
 *
 */

#include <pthread.h>
#include <iostream>
using namespace std;

pthread_mutex_t m1   = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
int totalThreads = 0;
char taskWhileWaiting = 'A';

void waitForOthers();

void * Restaurant(void * param);

/**
 * Start of main program
 */
int main(){

   pthread_t hungryThd[4];
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   // create threads
   for (int i = 0; i < 4; i++)
      pthread_create(&hungryThd[i],NULL,Restaurant,NULL);

   // join threads
   for (int i = 0; i < 4; i++)
      pthread_join(hungryThd[i],NULL);

   return 0;
}

/**
 * Wait for others function that enables threads to lock a mutex
 * and then update global shared data and set some local variables
 * with those updates.  The thread then will print its id, task, and
 * then wait for a signal that all threads have arrived before
 * continuing (unless it is the last thread).  The final thread to
 * arrive will unlock the mutex after data updates and finally will
 * signal to the other threads that it is time to eat.
 *
 */
void waitForOthers(){

   int id;
   char ltr;
   // add barrier and output code here
   pthread_mutex_lock(&m1);

   id = ++totalThreads;
   if (id > 1)
      ltr = ++taskWhileWaiting;
   else
      ltr = taskWhileWaiting;

   cout<<"I am thread "<<id<<", working on Task "<<ltr<<" and waiting to eat."<<endl;
   if (totalThreads < 4)
      pthread_cond_wait(&notFull, &m1);

   cout<<"I am thread "<<id<<" and I am eating."<<endl;

   pthread_mutex_unlock(&m1);
   pthread_cond_signal(&notFull);

}

/**
 * Function provided to newly created threads as the starting point
 * for execution.
 *
 * @param param  optional parameter.  Not Used
 */
void * Restaurant(void * param){

    // call wait function
   waitForOthers();

   pthread_exit(NULL);

}
