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

#include "../inc/monitor.h"
#include "../inc/data.h"
#include "../inc/semaphore.h"

#include <string.h>

Monitor::Monitor(int q_id)
{
    switch(q_id){//sprawdzenie ktora to kolejka
        case 1:
            fullId = semget(FULL_A_KEY, 1, IPC_CREAT|IPC_EXCL|0600);	
            semctl(fullId, 0, SETVAL, (int)0);        
            emptyId = semget(EMPTY_A_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
            semctl(emptyId, 0, SETVAL, BUFFER_SIZE);          
            mutexId = semget(MUTEX_A_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
            semctl(mutexId, 0, SETVAL, (int)1);
            printf("Utworzono monitor dla kolejki A.\n");
            break;
        case 2:
            fullId = semget(FULL_B_KEY, 1, IPC_CREAT|IPC_EXCL|0600);	
            semctl(fullId, 0, SETVAL, (int)0);        
            emptyId = semget(EMPTY_B_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
            semctl(emptyId, 0, SETVAL, BUFFER_SIZE);          
            mutexId = semget(MUTEX_B_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
            semctl(mutexId, 0, SETVAL, (int)1);
            printf("Utworzono monitor dla kolejki B.\n");
            break;
        case 3:
            fullId = semget(FULL_C_KEY, 1, IPC_CREAT|IPC_EXCL|0600);	
            semctl(fullId, 0, SETVAL, (int)0);        
            emptyId = semget(EMPTY_C_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
            semctl(emptyId, 0, SETVAL, BUFFER_SIZE);          
            mutexId = semget(MUTEX_C_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
            semctl(mutexId, 0, SETVAL, (int)1);
            printf("Utworzono monitor dla kolejki C.\n");
            break;
    }

}

void Monitor::enter()
{
	sem_down(mutexId, 0);
}

void Monitor::leave()
{
	sem_up(mutexId, 0);
}

void Monitor::wait_full()
{
    sem_down(emptyId, 0);
}

void Monitor::signal_full()
{
	sem_up(emptyId, 0);
}

void Monitor::wait_empty()
{
    sem_down(fullId, 0);
}

void Monitor::signal_empty()
{
	sem_up(fullId, 0);
}