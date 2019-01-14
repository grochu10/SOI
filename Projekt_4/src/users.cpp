#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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


#include "../inc/data.h"
#include "../inc/message.h"
#include "../inc/queue.h"
#include "../inc/semaphore.h"


//losowanie litery do wiadomosci
char random_lit(int queue_id)
{
    char tab[MESS_SIZE] = {'a', 'b', 'c'};
    int i;
    int list[10];
    int d;
    for(i=0;i<10;++i)
    {
        list[i] = rand()/(i+1+queue_id) % 3;
    }
    d = rand() % 10;
    return tab[list[d]];
}

//losowanie kolejki do wyslanai wiadomosci przez super producenta
int random_queue(int queue_id)
{
    int tab[3] = {1,2,3};
    int i;
    int list[10];
    int d;
    for(i=0;i<10;++i)
    {
        list[i] = rand()/(i+1+queue_id) % 3;
    }
    d = rand() % 10;
    return tab[list[d]];
}


//inicjalizacja procesu producent dla danej kolejki
int producer(int queue_id)
{
    int i;
    int q_id;
    int q_key;
    int priority = 0;
	struct Message msg;
    char message[MESS_SIZE];
    switch(queue_id){//sprawdzenie ktora to kolejka
        case 1:
            q_key = QUEUE_A_KEY;
            break;
        case 2:
            q_key = QUEUE_B_KEY;
            break;
        case 3:
            q_key = QUEUE_C_KEY;
            break;
    }
	q_id = shmget(q_key, sizeof(struct Queue), 0666);
	Queue * queue = (Queue*)shmat(q_id, NULL, 0);	// utworzenie wskaznika na kolejke
	
	while(1)//glowna petla producenta
	{	
        sleep(PRODUCER_DELAY);
        for(i=0;i<MESS_SIZE;++i)
        {
            message[i] = random_lit(queue_id);
        }
		set_msg(&msg,priority,queue_id,message);
		queue->send_msg(msg);
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
	}
    return 0;		
}

//inicjalizacja procesu producent ochronny
int security_producer(void)
{
    int i,j;
    int queue_id;
    int priority = 2;
	struct Message msg;
    char message[MESS_SIZE];
    int q_id_A = shmget(QUEUE_A_KEY, sizeof(struct Queue), 0666);
    int q_id_B = shmget(QUEUE_B_KEY, sizeof(struct Queue), 0666);
    int q_id_C = shmget(QUEUE_C_KEY, sizeof(struct Queue), 0666);
	Queue * queue_A = (Queue*)shmat(q_id_A, NULL, 0);	// utworzenie wskaznika na kolejke A
    Queue * queue_B = (Queue*)shmat(q_id_B, NULL, 0);	// utworzenie wskaznika na kolejke B
    Queue * queue_C = (Queue*)shmat(q_id_C, NULL, 0);	// utworzenie wskaznika na kolejke C
	while(1)//glowna petla producenta
	{
        sleep(SECURITY_DELAY);
        for(j=0;j<MESS_SIZE;j++)
        {
            message[j] = '0';
        }
        queue_id = 1;
        if(queue_B->size > queue_A->size){
            queue_id = 2;
            if(queue_C->size > queue_B->size)queue_id = 3;
        }else if(queue_C->size > queue_A->size)queue_id = 3;
        set_msg(&msg,priority,queue_id,message);
        switch(queue_id){
            case 1:
                queue_A->send_msg(msg);
                printf("Producent ochronny wyslal wiadomosc do kolejki A.\n");	 
                break;
            case 2:
                queue_B->send_msg(msg);
                printf("Producent ochronny wyslal wiadomosc do kolejki B.\n");
                break;
            case 3:
                queue_C->send_msg(msg);
                printf("Producent ochronny wyslal wiadomosc do kolejki C.\n"); 
                break;
        } 		
	}
    return 0;	
}

//inicjalizacja procesu super producent
int super_producer(void)
{
    int i;
    int queue_id;
    int priority = 1;
	struct Message msg;
    char message[MESS_SIZE];	
	int q_id_A = shmget(QUEUE_A_KEY, sizeof(struct Queue), 0666);
    int q_id_B = shmget(QUEUE_B_KEY, sizeof(struct Queue), 0666);
    int q_id_C = shmget(QUEUE_C_KEY, sizeof(struct Queue), 0666);
	Queue * queue_A = (Queue*)shmat(q_id_A, NULL, 0);	// utworzenie wskaznika na kolejke A
    Queue * queue_B = (Queue*)shmat(q_id_B, NULL, 0);	// utworzenie wskaznika na kolejke B
    Queue * queue_C = (Queue*)shmat(q_id_C, NULL, 0);	// utworzenie wskaznika na kolejke C
	while(1)//glowna petla super producenta
	{
        sleep(PRODUCER_DELAY);
        for(i=0;i<MESS_SIZE;++i)
        {
            message[i] = random_lit(queue_id);
        }
        queue_id = random_queue(queue_id);
        set_msg(&msg,priority,queue_id,message);
        switch(queue_id){
            case 1:
                queue_A->send_msg(msg);
                printf("Super producent wyslal wiadomosc do kolejki A.\n");	 
                break;
            case 2:
                queue_B->send_msg(msg);
                printf("Super producent wyslal wiadomosc do kolejki B.\n");
                break;
            case 3:
                queue_C->send_msg(msg);
                printf("Super producent wyslal wiadomosc do kolejki C.\n"); 
                break;
        } 		
	}
    return 0;	
}

//inicjalizacja procesu konsument dla danej kolejki
int consumer(int queue_id, float pr)
{
    float l;
    int i;
    int q_id;
    int q_key;
	int q_id_A = shmget(QUEUE_A_KEY, sizeof(struct Queue), 0666);
    int q_id_B = shmget(QUEUE_B_KEY, sizeof(struct Queue), 0666);
    int q_id_C = shmget(QUEUE_C_KEY, sizeof(struct Queue), 0666);
	Queue * queue_A = (Queue*)shmat(q_id_A, NULL, 0);	// utworzenie wskaznika na kolejke A
    Queue * queue_B = (Queue*)shmat(q_id_B, NULL, 0);	// utworzenie wskaznika na kolejke B
    Queue * queue_C = (Queue*)shmat(q_id_C, NULL, 0);	// utworzenie wskaznika na kolejke C
    char tmp;
	struct Message msg;
    char message[MESS_SIZE];
    switch(queue_id){//sprawdzenie ktora to kolejka
        case 1:
            q_key = QUEUE_A_KEY;	
            break;
        case 2:
            q_key = QUEUE_B_KEY;	
            break;
        case 3:
            q_key = QUEUE_C_KEY;	
            break;
    }
	q_id = shmget(q_key, sizeof(struct Queue), 0666);
	Queue * queue = (Queue*)shmat(q_id, NULL, 0);	// utworzenie wskaznika na kolejke
	
	while(1)//glowna petla konsumenta
	{	
		usleep(CONSUMER_DELAY);
        msg = queue->read_msg();	
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
            msg.m[MESS_SIZE-1] = '0';
            if(pr != 0){
                l = rand()/RAND_MAX;
                if(l <= pr){
                    if(msg.m[MESS_SIZE-2]== '0')msg.m[MESS_SIZE-1] = random_lit(queue_id);
                    else msg.m[MESS_SIZE-1] = random_lit(queue_id);
                }
            }      
        int a;
		if(tmp == 'a')a =1;
		else if (tmp=='b') a =2;
		else a=3;
        switch(a){
            case 1:
                queue_A->send_msg(msg);
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
                break;
            case 2:
                queue_B->send_msg(msg);
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
                break;
            case 3:
                queue_C->send_msg(msg);
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
                break;
            }
        }else{
            if(msg.pri == 2){//wiadomosc od producenta ochronnego
                for(i=0;i<5;++i){
                    msg = queue->read_msg();	
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
                        msg.m[MESS_SIZE-1] = '0';
                        if(pr != 0){
                            l = rand()/RAND_MAX;
                            if(l <= pr){
                                if(msg.m[MESS_SIZE-2]== '0')msg.m[MESS_SIZE-1] = random_lit(queue_id);
                                else msg.m[MESS_SIZE-1] = random_lit(queue_id);
                            }
                        }
                        int a;
                        if(tmp == 'a')a =1;
                        else if (tmp=='b') a =2;
                        else a=3;
                        switch(a){
                            case 1:
                                queue_A->send_msg(msg);
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
                                break;
                            case 2:
                                queue_B->send_msg(msg);
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
                                break;
                            case 3:
                                queue_C->send_msg(msg);
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
                                break;
                        }
                    }
                }
            }	
	    }
    }
    return 0;		
}