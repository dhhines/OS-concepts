#include <iostream> 
#include <cstdlib> 
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "sleeper.h"

using namespace std;

/*
int main( ) {  //Example usage
   rideTime();
   cout << "Ride over" << endl;
   walkAroundTime();
   cout <<  "Finished walking around " << endl;
}
*/

void rideTime() {
   int seconds = (random() % 5) + 1 ;
   cout << "Riding for  " <<  seconds << " seconds  " <<endl;
   sleep (seconds);
}

void walkAroundTime() {
   int seconds = (random() % 10) + 1 ;
   cout <<"Walking around for  "<<  seconds << " seconds  "<<endl;
   sleep (seconds);
}