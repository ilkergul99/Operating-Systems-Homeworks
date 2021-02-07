#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <queue>
#include <semaphore.h>
#include <time.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared queue
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size

pthread_mutex_t Cout_Mutex = PTHREAD_MUTEX_INITIALIZER; // second mutex
int Done_Threads = 0;
int Array_index = 0;


void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	node GonnaBeAdded;

	GonnaBeAdded.id = thread_id;
	GonnaBeAdded.size = size;

	myqueue.push(GonnaBeAdded);
	// I do not need to use mutex since we are entering the crtitical region
	// Because I locked before I enter the function itself

}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	while(Done_Threads < NUM_THREADS)
	{
		pthread_mutex_lock(&sharedLock);
		if(!myqueue.empty())
		{
			if(MEMORY_SIZE - Array_index > myqueue.front().size )
			{
				thread_message[myqueue.front().id] = Array_index;
				Array_index += myqueue.front().size;
			}
			else
			{
				thread_message[myqueue.front().id] = -1;
			}
			int temp = myqueue.front().id;
			myqueue.pop();
			sem_post(&semlist[temp]);
			Done_Threads++;
		}
		pthread_mutex_unlock(&sharedLock);
	}
}

void * thread_function(void * id)
{
	//This function will create a random size, and call my_malloc
	int * ThreadIDPtr = (int*) id;
	char outchr = '0' + *ThreadIDPtr;
	// This is for character assignment to memory array
	int RandGenSize = rand() % (MEMORY_SIZE/6) + 1; // Between 1 and MEMORY_SIZE/6

	pthread_mutex_lock(&sharedLock);
	my_malloc(*ThreadIDPtr, RandGenSize);
	pthread_mutex_unlock(&sharedLock);
	//Block
	sem_wait(&semlist[*ThreadIDPtr]);
	//Then fill the memory with id's or give an error prompt
	if(thread_message[*ThreadIDPtr] == -1)
	{
		pthread_mutex_lock(&Cout_Mutex);
		cout << "Thread " << *ThreadIDPtr << ": Not enough memory" << endl;
		pthread_mutex_unlock(&Cout_Mutex);
	}
	else
	{
		pthread_mutex_lock(&Cout_Mutex);
		for(int j = 0; j < RandGenSize; j++)
		{
			memory[thread_message[*ThreadIDPtr] + j] = outchr;
		}
		pthread_mutex_unlock(&Cout_Mutex);
	}
}

void init()
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //start server
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory()
{
 // You need to print the whole memory array here.
 	cout << "Content of the Memory Array is as follows:" << endl;
  for(int a = 0; a < MEMORY_SIZE; a++)
	{
		cout << memory[a];
	}

}

int main (int argc, char *argv[])
{
  // Acoording to some search on using rand, if we initilize the random seed
	// We are going to get a different value every time the program is run.
	// Useful for thread function
	srand (time(NULL));

 	//You need to create a thread ID array here
	int thread_ID[NUM_THREADS];
	for(int i = 0; i < NUM_THREADS; i++)
	{
		thread_ID[i] = i;
	}


 	init();	// call init

 	//You need to create threads with using thread ID array, using pthread_create()
	pthread_t Usable_threads[NUM_THREADS];
	for(int i = 0; i < NUM_THREADS; i++)
	{
		pthread_create(&Usable_threads[i], NULL, thread_function, (void*) &thread_ID[i]);
	}

 	//You need to join the threads
	for(int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(Usable_threads[i], NULL);
	}


 	dump_memory(); // this will print out the memory

 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
}
