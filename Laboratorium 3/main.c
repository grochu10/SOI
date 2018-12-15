#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>
#include "data.h"
#include "message.h"
#include "queue.h"
#include "users.h"


int main(int argc, char * argv[])
{
	char* args;
	int tmp;
	int q_key_A = QUEUE_A_KEY;
    int fullId_A = semget(FULL_A_KEY, 1, 0600);
    int emptyId_A = semget(EMPTY_A_KEY, 1, 0600);
    int mutexId_A = semget(MUTEX_A_KEY, 1, 0600);	
    int q_key_B = QUEUE_B_KEY;
    int fullId_B = semget(FULL_B_KEY, 1, 0600);
    int emptyId_B = semget(EMPTY_B_KEY, 1, 0600);
    int mutexId_B = semget(MUTEX_B_KEY, 1, 0600);	
    int q_key_C = QUEUE_C_KEY;
    int fullId_C = semget(FULL_C_KEY, 1, 0600);
    int emptyId_C = semget(EMPTY_C_KEY, 1, 0600);
    int mutexId_C = semget(MUTEX_C_KEY, 1, 0600);	
	int q_id_A = shmget(q_key_A, sizeof(struct Queue), 0666);
    int q_id_B = shmget(q_key_B, sizeof(struct Queue), 0666);
    int q_id_C = shmget(q_key_C, sizeof(struct Queue), 0666);
	struct Queue *queue_A = (struct Queue*)shmat(q_id_A, NULL, 0);	// utworzenie wskaznika na kolejke A
    struct Queue *queue_B = (struct Queue*)shmat(q_id_B, NULL, 0);	// utworzenie wskaznika na kolejke B
    struct Queue *queue_C = (struct Queue*)shmat(q_id_C, NULL, 0);	// utworzenie wskaznika na kolejke C

	if(argc != 2 )
	{
		printf("Niepoprawna ilosc argumentow!\n");
		return -2;
	}
	//TODO
	
    args[0] = argv[0];
    args[2] = argv[1];
    #define WAIT_TIME 200

    args[1] = "A";
    new_child(consumer, 3, args);
    usleep(WAIT_TIME);
    args[1] = "B";
    new_child(consumer, 3, args);
    usleep(WAIT_TIME);
    args[1] = "C";
    new_child(consumer, 3, args);
    usleep(WAIT_TIME);

    args[1] = "A";
    new_child(producer, 2, args);
    usleep(WAIT_TIME);
    args[1] = "B";
    new_child(producer, 2, args);
    usleep(WAIT_TIME);
    args[1] = "C";
    new_child(producer, 2, args);
    usleep(WAIT_TIME);

    new_child(super_producer, 1, args);

	while(wait(&tmp) != -1);	
	
	shmdt(queue_A);
	shmdt(queue_B);
	shmdt(queue_C);

	return 0;
}

void new_child(int (*child)(int, char**), int argc, char * argv[])
{
	if( fork()==0 )
	{
        // only child will enter here
		exit(child(argc, argv));
	}
	/* only parrent will return from function */
}