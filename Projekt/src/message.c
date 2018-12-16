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
#include "../inc/queue.h"
#include "../inc/message.h"

//ustawienie wartosci pol struktury wiadomosc
void set_msg(struct Message *m, int pri, int src, char* msg)
{
	m->pri = pri;
	m->source = src;
    memcpy(m->m,msg,MESS_SIZE);
}