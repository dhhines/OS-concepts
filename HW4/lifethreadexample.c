#include <pthread.h>
#include <stdlib.h>

/* to compile this program, type
   cc -lpthread lifethreadexample.c

   to run it, type
   ./a.out
*/

int sum; /* this data is shared by the threads */


void * run (void * param); /* the thread */

/* description of parameter package */
struct parameter {
int i;
int j;
}; 

main (int argc, char * argv[1]) {

pthread_t tid; /* the thread identifier*/

pthread_attr_t attr;/*set of thread attributes */

struct parameter  * argument;
/* get the default attributes */

pthread_attr_init(&attr);

/* create the thread */
argument = malloc (sizeof(struct parameter));
argument->i = 5;
argument->j = 10;
pthread_create(&tid,&attr,run,argument);

/* now wait for the thread to exit */
pthread_join (tid,NULL);

} /* end of main*/

/* The thread will begin control in this function */
void *run (void *param) 
{
struct parameter * ptrToParm = (struct parameter * ) param;


printf (" i = %d  and j = %d \n", ptrToParm->i,ptrToParm->j);
pthread_exit(0);
}

