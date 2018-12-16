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

#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>
#include <unistd.h>
#include "../inc/data.h"
#include "../inc/message.h"
#include "../inc/queue.h"
#include "../inc/users.h"


int main(int argc, char * argv[])
{
    float pr =1;
    int i;
	char* args;
	int q_key_A = QUEUE_A_KEY;
    int fullId_A = semget(FULL_A_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(fullId_A, 0, SETVAL, (int)0); 
    int emptyId_A = semget(EMPTY_A_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(emptyId_A, 0, SETVAL, (int)BUFFER_SIZE);
    int mutexId_A = semget(MUTEX_A_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(mutexId_A, 0, SETVAL, (int)1);	
    int q_key_B = QUEUE_B_KEY;
    int fullId_B = semget(FULL_B_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(fullId_B, 0, SETVAL, (int)0);
    int emptyId_B = semget(EMPTY_B_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(emptyId_B, 0, SETVAL, (int)BUFFER_SIZE);
    int mutexId_B = semget(MUTEX_B_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(mutexId_B, 0, SETVAL, (int)1);	
    int q_key_C = QUEUE_C_KEY;
    int fullId_C = semget(FULL_C_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(fullId_C, 0, SETVAL, (int)0);
    int emptyId_C = semget(EMPTY_C_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(emptyId_C, 0, SETVAL, (int)BUFFER_SIZE);
    int mutexId_C = semget(MUTEX_C_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
    semctl(mutexId_C, 0, SETVAL, (int)1);	
	int q_id_A = shmget(q_key_A, sizeof(struct Queue), IPC_CREAT|0666);
    int q_id_B = shmget(q_key_B, sizeof(struct Queue), IPC_CREAT|0666);
    int q_id_C = shmget(q_key_C, sizeof(struct Queue), IPC_CREAT|0666);
	struct Queue *queue_A = (struct Queue*)shmat(q_id_A, NULL, 0);	// utworzenie wskaznika na kolejke A
    struct Queue *queue_B = (struct Queue*)shmat(q_id_B, NULL, 0);	// utworzenie wskaznika na kolejke B
    struct Queue *queue_C = (struct Queue*)shmat(q_id_C, NULL, 0);	// utworzenie wskaznika na kolejke C
	
    #define WAIT_TIME 200

    if( fork()==0 )
	{ 
        exit(super_producer());
    }
    printf("Utworzony zostal producent specjalny.\n"); 
    usleep(WAIT_TIME);
    for(i=1;i<4;i++)
    {
        if( fork()==0 )
        {
            exit(producer(i));
        }
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
        if( fork()==0 )
        {
            
            exit(consumer(i,pr));
        }
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
    int count;
    count = 0;
    while( count <= (5*60*1000/250))
    {
        printf("%c: %d | %c: %d |%c: %d |", 'A', queue_A->size,'B',queue_B->size,'C',queue_C->size);
        printf("time: %.2fs\n", (float)(count*250.0/1000));
        
        usleep(1000*250);
        count++;
    }
    while(wait(&i) != -1);	
    return 0;
}
