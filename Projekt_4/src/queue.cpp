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
#include "../inc/monitor.h"


Queue::Queue(int q_id) : Monitor(q_id)
{
    int i;
    head = 0;
	tail = 0;
	size = 0;
    for(i=0;i<BUFFER_SIZE;++i)
        table[i].free_place = 1;
}

//wyslanie wiadomosci do kolejki
void Queue::send_msg(struct Message m)
{
    int i;
    int n;
    int tmp;
    wait_full();
    enter();
    if(size == 0){//gdy tablica pusta
        table[0] = m;
        head = 0;
        tail = 0;
    }else{
        for(i = 0;i < BUFFER_SIZE;++i)//poszukiwanie wolnego miejsca w pamieci
        {
            if(table[i].free_place == 1){//gdy znaleziono miejsce
                table[i].free_place = 0;
                table[i] = m;
                if(table[i].pri == 2){
                    if(table[head].pri == 2){//gdy pierwszy proces ma wysoki priorytet
                        tmp = head;
                        //przechodzenie kolejki aby znalezc ostatni proces z wysokim priorytetem
                        while(table[tmp].pri != 1 || tmp == tail)
                            tmp = table[tmp].next;
                        if(tmp == tail){//gdy wszystkie procesy w kolejce maja wysoki priorytet
                            table[tmp].next = table[i].next;
                            tail = i;
                        }else{//gdy znaleziono granice miedzy procesami o roznych priorytetach
                            table[i].next = tmp;
                            table[tmp].next = table[i].next;
                        }
                    }
                }else{
                    if(table[i].pri == 1){//gdy wiadomosc o wysokim priorytecie
                        if(table[head].pri == 1){//gdy pierwszy proces ma wysoki priorytet
                            tmp = head;
                            //przechodzenie kolejki aby znalezc ostatni proces z wysokim priorytetem
                            while(table[tmp].pri != 0 || tmp == tail)
                                tmp = table[tmp].next;
                            if(tmp == tail){//gdy wszystkie procesy w kolejce maja wysoki priorytet
                                table[tmp].next = table[i].next;
                                tail = i;
                            }else{//gdy znaleziono granice miedzy procesami o roznych priorytetach
                                table[i].next = tmp;
                                table[tmp].next = table[i].next;
                            }
                        }else{//gdy w tablicy tylko elementy o normalnym priorytecie
                            table[i].next = head;
                            head = i;
                        }
                    }else{//wiadomosc ma zwykly priorytet
                        table[tail].next = i;
                        i = tail;
                    }  
                }
            break;
            }
        }
    }
	size++;
    leave();
    signal_empty();
}

//odczytanie wiadomosci z kolejki
struct Message Queue::read_msg()
{	
    wait_empty();
    enter();
	struct Message m;
	m = table[head];
    table[head].free_place = 1;
    if(head != tail)//gdy w tablicej jest wiecej niz 1 element
        head = table[head].next;
    size--;
    leave();
    signal_full();
    return m;
}

