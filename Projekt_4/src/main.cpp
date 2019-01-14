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
    float pr = 0;
    int i;	
	int q_id_A = shmget(QUEUE_A_KEY, sizeof(struct Queue), IPC_CREAT|0666);
    int q_id_B = shmget(QUEUE_B_KEY, sizeof(struct Queue), IPC_CREAT|0666);
    int q_id_C = shmget(QUEUE_C_KEY, sizeof(struct Queue), IPC_CREAT|0666);
	Queue * queue_A = (Queue*)shmat(q_id_A, NULL, 0);	// utworzenie wskaznika na kolejke A
    Queue * queue_B = (Queue*)shmat(q_id_B, NULL, 0);	// utworzenie wskaznika na kolejke B
    Queue * queue_C = (Queue*)shmat(q_id_C, NULL, 0);	// utworzenie wskaznika na kolejke C
	*queue_A = Queue(1);
    *queue_B = Queue(2);
    *queue_C = Queue(3);
    #define WAIT_TIME 200

    if( fork()==0 )
	{ 
        exit(super_producer());
    }
    printf("Utworzony zostal producent specjalny.\n"); 
    usleep(WAIT_TIME);
    if( fork()==0 )
	{ 
        exit(security_producer());
    }
    printf("Utworzony zostal producent ochronny.\n"); 
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
        printf("Stany kolejek:\n%c: %d || %c: %d ||%c: %d ||", 'A', queue_A->size,'B',queue_B->size,'C',queue_C->size);
        printf("czas symulacji: %.2fs\n", (float)(count*500.0/1000));
        
        usleep(1000*500);
        count++;
    }
    while(wait(&i) != -1);	
    return 0;
}