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
#include <uistd.h>

#include <string.h>
#include "data.h"
#include "message.h"
#include "queue.h"



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