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
#include <uistd.h>

#include ".../inc/data.h"
#include ".../inc/message.h"
#include ".../inc/queue.h"


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
		usleep(PRODUCER_DELAY);		
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
        usleep(PRODUCER_DELAY);		
	}	
}

//inicjalizacja procesu konsument dla danej kolejki
void consumer(int queue_id, float pr)
{
    float l;
    int i;
    int q_id;
    int q_key;
    int fullId;
    int emptyId;
    int mutexId;
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
    char tmp;
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
		
		sem_down(fullId, 0);
		sem_down(mutexId, 0);
        msg = read_msg(queue);
        usleep(CONSUMER_DELAY);	
        if(msg.m[0]!='0'){
             switch(queue_id){
            case 1:
                printf("Konsument A odczytal wiadomosc.\n");
                printf("Oto ona: %c%c%c.\n",msg.m[0],msg.m[1],msg.m[2]);
            break;
            case 2:
                printf("Konsument B odczytal wiadomosc.\n");
                printf("Oto ona: %c%c%c.\n",msg.m[0],msg.m[1],msg.m[2]);
                break;
            case 3:
                printf("Konsument C odczytal wiadomosc.\n");
                printf("Oto ona: %c%c%c.\n",msg.m[0],msg.m[1],msg.m[2]);
                break;
            }
            tmp = msg.m[0];
            for(i=0;i<MESS_SIZE-1;++i)
            {
                msg.m[i] = msg.m[i+1];
            }
            if(pr != 0){
                l = rand()/RAND_MAX;
                if(l <= pr)
                    msg.m[MESS_SIZE-1] = random_lit();
            }else
                msg.m[MESS_SIZE-1] = '0';
            switch(tmp){
            case 'a':
                sem_down(emptyId_A, 0);
                sem_down(mutexId_A, 0);
                send_msg(queue_A, msg);
                switch(queue_id){
                    case 1:
                        printf("Konsument A przeslal wiadomosc do kolejki A.\n"); 
                        break;
                    case 2:
                        printf("Konsument B przeslal wiadomosc do kolejki A.\n"); 
                        break;
                    case 3:
                        printf("Konsument C przeslal wiadomosc do kolejki A.\n"); 
                        break;
                }
                up(mutexId_A, 0);
		        up(fullId_A, 0);
                break;
            case 'b':
                sem_down(emptyId_B, 0);
                sem_down(mutexId_B, 0);
                send_msg(queue_B, msg);
                switch(queue_id){
                    case 1:
                        printf("Konsument A przeslal wiadomosc do kolejki B.\n"); 
                        break;
                    case 2:
                        printf("Konsument B przeslal wiadomosc do kolejki B.\n"); 
                        break;
                    case 3:
                        printf("Konsument C przeslal wiadomosc do kolejki B.\n"); 
                        break;
                }
                up(mutexId_B, 0);
		        up(fullId_B, 0);
                break;
            case 'c':
                sem_down(emptyId_C, 0);
                sem_down(mutexId_C, 0);
                send_msg(queue_C, msg);
                switch(queue_id){
                    case 1:
                        printf("Konsument A przeslal wiadomosc do kolejki C.\n"); 
                        break;
                    case 2:
                        printf("Konsument B przeslal wiadomosc do kolejki C.\n"); 
                        break;
                    case 3:
                        printf("Konsument C przeslal wiadomosc do kolejki C.\n"); 
                        break;
                }
                up(mutexId_C, 0);
		        up(fullId_C, 0);
                break;
            }
        }
		up(mutexId, 0);
		up(emptyId, 0);	
				
	}	
}
