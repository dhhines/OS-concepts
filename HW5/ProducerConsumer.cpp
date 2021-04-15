/*  ProducerConsumer.cpp  -- a sample program using
    condition var +  mutex to produce  a monitor */

/*  Compile :  g++ ProducerConsumer.cpp -lpthread */
#include <pthread.h>
#include <iostream>

using namespace std;

pthread_t threads[2];  //Holds thread ids

//This is a mutex...a mutex is like the counting semaphores we went over
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

//These are the condition variables for our monitor
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full  = PTHREAD_COND_INITIALIZER;

//Our local variables/shared data
int cnt = 0;
int in = 0;
int out = 0;

const int MAXBUF = 20;
char buf[MAXBUF];

/*** a monitor  ***/
void producerPut(char c)
{
  pthread_mutex_lock( &m1 );  //Obtain the mutex
  
  while (cnt == MAXBUF)
  {
     cout <<"waiting on full\n";
     pthread_cond_wait(&full,&m1); //Conditional wait
  }
  
  buf[in] = c;
  in = (in + 1) % MAXBUF;
  cnt++;

  cout<<"produced: "<<c<<endl;
  
  pthread_mutex_unlock( &m1 );      //Unlock the mutex
  pthread_cond_signal( &empty );    //Signal consumers
}

void consumerGet(char *c) {
  pthread_mutex_lock( &m1 );
  
  while (cnt == 0)
  {
     cout <<"waiting on empty\n";
     pthread_cond_wait(&empty, &m1);
  }
  
  *c = buf[out];
  out = (out + 1) % MAXBUF;
  cnt--;

  cout <<"consumed: "<<*c<<endl;
  
  pthread_mutex_unlock( &m1 );
  pthread_cond_signal(&full);
}

/* producer/consumer threads */
void * producer(void * dummy)
{
  char c;
  int  j;
  
  for (j = 0; j < 500; j++) {
     c = "abcdefghijklmnopqrstuvwxyz"[j%26];
     producerPut(c);
  }
  
  cout <<" producer finished \n";
}

void * consumer(void * dummy)
{
  char c;
  int  j;
  
  for(j = 0; j < 500; j++) {
     consumerGet(&c);
  }
  
  cout <<" consumer finished \n";
}

int  main() {
  int i; 
  
  pthread_create( &threads[0], NULL, producer,  NULL );
  pthread_create(&threads[1], NULL, consumer,  NULL );
  
 
  for (i = 0; i < 2; i++) { 
    pthread_join (threads[i], NULL);
    cout  << "Joined thread with id  [" << threads[i] << "] \n";
  }

}