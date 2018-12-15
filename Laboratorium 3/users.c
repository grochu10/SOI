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


//losowanie litery do wiadomosci
char random_lit(void)
{
    char tab[MESS_SIZE] = {"a","b","c"};
    int i;
    int list[10];
    int d;
    for(i=0;i<10;++i)
    {
        list[i] = rand() % 2;
    }
    d = rand() % 9;
    return tab[list[d]];
}

//losowanie kolejki do wyslanai wiadomosci przez super producenta
int random_queue(void)
{
    int tab[3] = {0,1,2};
    int i;
    int list[10];
    int d;
    for(i=0;i<10;++i)
    {
        list[i] = rand() % 2;
    }
    d = rand() % 9;
    return tab[list[d]];

}


//inicjalizacja procesu producent dla danej kolejki
void producer(int queue_id)
{
    int i;
    int q_id;
    int q_key;
    int fullId;
    int emptyId;
    int mutexId;
	struct Message msg;
    char message[MESS_SIZE];
    switch(queue_id){//sprawdzenie ktora to kolejka
        case 1:
            q_key = QUEUE_A_KEY;
            fullId = semget(FULL_A_KEY, 1, 0600);
	        emptyId = semget(EMPTY_A_KEY, 1, 0600);
	        mutexId = semget(MUTEX_A_KEY, 1, 0600);	
            break;
        case 2:
            q_key = QUEUE_B_KEY;
            fullId = semget(FULL_B_KEY, 1, 0600);
            emptyId = semget(EMPTY_B_KEY, 1, 0600);
            mutexId = semget(MUTEX_B_KEY, 1, 0600);	
            break;
        case 3:
            q_key = QUEUE_C_KEY;
            fullId = semget(FULL_C_KEY, 1, 0600);
	        emptyId = semget(EMPTY_C_KEY, 1, 0600);
	        mutexId = semget(MUTEX_C_KEY, 1, 0600);	
            break;
    }
	q_id = shmget(q_key, sizeof(struct Queue), 0666);
	struct Queue *queue = (struct Queue*)shmat(q_id, NULL, 0);	// utworzenie wskaznika na kolejke
	
	while(1)//glowna petla producenta
	{	
        for(i=0;i<MESS_SIZE;++i)
        {
            message[i] = random_lit();
        }
		set_msg(&msg,0,queue_id,message);
		
		sem_down(emptyId, 0);
		sem_down(mutexId, 0);

		send_msg(queue, msg);

        switch(queue_id){
            case 1:
                printf("Producent A wyslal wiadomosc.\n"); 
            break;
            case 2:
                printf("Producent B wyslal wiadomosc.\n");
                break;
            case 3:
                printf("Producent C wyslal wiadomosc.\n");
                break;
        }

		up(mutexId, 0);
		up(fullId, 0);	
				
	}	
}

//inicjalizacja procesu super producent dla danej kolejki
void super_producer(void)
{
    int i;
    int queue_id;
	struct Message msg;
    char message[MESS_SIZE];
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
	while(1)//glowna petla producenta
	{
        for(i=0;i<MESS_SIZE;++i)
        {
            message[i] = random_lit();
        }
		set_msg(&msg,0,queue_id,message);
        queue_id = random_queue();

        switch(queue_id){
            case 1:
                set_msg(&msg,1,queue_id,message);
                sem_down(emptyId_A, 0);
                sem_down(mutexId_A, 0);
                send_msg(queue_A, msg);
                printf("Super producent wyslal wiadomosc do kolejki A.\n");
                up(mutexId_A, 0);
		        up(fullId_A, 0);	 
                break;
            case 2:
                set_msg(&msg,1,queue_id,message);
                sem_down(emptyId_B, 0);
                sem_down(mutexId_B, 0);
                send_msg(queue_B, msg);
                printf("Super producent wyslal wiadomosc do kolejki B.\n");
                up(mutexId_B, 0);
		        up(fullId_B, 0);
                break;
            case 3:
                set_msg(&msg,1,queue_id,message);
                sem_down(emptyId_C, 0);
                sem_down(mutexId_C, 0);
                send_msg(queue_C, msg);
                printf("Super producent wyslal wiadomosc do kolejki C.\n"); 
                up(mutexId_C, 0);
		        up(fullId_C, 0);
                break;
        }		
	}	
}