#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "sleeper.h"

/* example of usage

int main( ) {
   rideTime();
   printf ("Ride over\n");
   walkAroundTime();
   printf ("Finished walking around  \n");
}
*/


void rideTime()
{
   srand(time(0));
   int seconds = (rand() % 5) + 1 ;
   //printf ("%d seconds \n", seconds);
   sleep (seconds);
}

void walkAroundTime()
{
   srand(time(0));
   int seconds = (rand() % 10) + 1 ;
   //printf ("%d seconds \n", seconds);
   sleep (seconds);
}