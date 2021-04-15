#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
   int seconds = ( rand() % 5) + 1 ;
   printf ("Riding for  %d seconds \n ", seconds);
   sleep (seconds);
}

void walkAroundTime()
{
   int seconds = (rand() % 10) + 1 ;
   printf ("Walking around for  %d \n ", seconds);
   sleep (seconds);
}