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
#include "../inc/data.h"
#include "../inc/message.h"
#include "../inc/queue.h"



//inicjalizacja kolejki
void queue_init(struct Queue *q)
{
	q->head = 0;
	q->tail = 0;
	q->size = 0;	
}

//pobranie ilosci elementow w kolejce
int get_queue_size(struct Queue *q)
{
	return q->size;
}

//wyslanie wiadomosci do kolejki
void send_msg(struct Queue *q, struct Message m)
{
    int i;
    int n;
    int tmp;
    if(q->size == 0){//gdy tablica pusta
        q->table[0] = m;
        q->head = 0;
        q->tail = 0;
    }else{
        for(i = 0;i < BUFFER_SIZE;++i)//poszukiwanie wolnego miejsca w pamieci
        {
            if(q->table[i].free_place == 1){//gdy znaleziono miejsce
                q->table[i].free_place = 0;
                q->table[i] = m;
                if(q->table[i].pri == 1){//gdy wiadomosc o wysokim priorytecie
                    if(q->table[q->head].pri == 1){//gdy pierwszy proces ma wysoki priorytet
                        tmp = q->head;
                        //przechodzenie kolejki aby znalezc ostatni proces z wysokim priorytetem
                        while(q->table[tmp].pri == 0 || tmp == q->tail)
                            tmp = q->table[tmp].next;
                        if(tmp == q->tail){//gdy wszystkie procesy w kolejce maja wysoki priorytet
                            q->table[tmp].next = q->table[i].next;
                            q->tail = i;
                        }else{//gdy znaleziono granice miedzy procesami o roznych priorytetach
                            q->table[i].next = tmp;
                            q->table[tmp].next = q->table[i].next;
                        }
                    }else{//gdy w tablicy tylko elementy o normalnym priorytecie
                        q->table[i].next = q->head;
                        q->head = i;
                    }
                }else{//wiadomosc ma zwykly priorytet
                    q->table[q->tail].next = i;
                    i = q->tail;
                }  
            }
            break;
        }
    }
	q->size++;
}

//odczytanie wiadomosci z kolejki
struct Message read_msg(struct Queue *q)
{	
	struct Message m;

	m = q->table[q->head];
    if(q->head != q->tail)//gdy w tablicej jest wiecej niz 1 element
        q->head = q->table[q->head].next;
	q->size--;
	return m;
}

