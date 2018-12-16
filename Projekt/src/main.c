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
#include <unistd.h>
#include ".../inc/data.h"
#include ".../inc/message.h"
#include ".../inc/queue.h"
#include ".../inc/users.h"


int main(int argc, char * argv[])
{
    int i;
	char* args;
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
	
    #define WAIT_TIME 200

    if( fork()==0 )
	{
	super_producent();
    printf("Utworzony zostal producent specjalny.\n"); 
    usleep(WAIT_TIME);
    for(i=1;i<4;i++)
    {
        if( fork()==0 )
	    {
            producent(i);
            switch(i){
                case 1:
                    printf("Utworzony zostal producent A.\n"); 
                break;
                case 2:
                    printf("Utworzony zostal producent B.\n"); 
                    break;
                case 3:
                    printf("Utworzony zostal producent C.\n"); 
                    break;
            }
        usleep(WAIT_TIME);
	    }
        usleep(WAIT_TIME);
        if( fork()==0 )
	    {
            consumer(i,pr);
            switch(i){
                case 1:
                    printf("Utworzony zostal konsument A.\n"); 
                break;
                case 2:
                    printf("Utworzony zostal konsument B.\n"); 
                    break;
                case 3:
                    printf("Utworzony zostal konsument C.\n"); 
                    break;
            }
            usleep(WAIT_TIME);
	    }
        usleep(WAIT_TIME);

    }

	while(wait(&i) != -1);	    

	return 0;
}
