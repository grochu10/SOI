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

//semafory kolejka A
sem_t* empty_A;
sem_t* full_A;
sem_t* mutex_A;
//semafory kolejka B
sem_t* empty_B;
sem_t* full_B;
sem_t* mutex_B;
//semafory kolejka C
sem_t* empty_C;
sem_t* full_C;
sem_t* mutex_C;

//inkrementacja semafora
void sem_up(int semid, int semnum)
{
    struct sembuf buf;
    buf.sem_num = semnum;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1)
    {
        perror("Podnoszenie semafora");
        exit(1);
    }
}

//dekrementacja semafora
void sem_down(int semid, int semnum)
{
    struct sembuf buf;
    buf.sem_num = semnum;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1)
    {
        perror("Opuszczenie semafora");
        exit(1);
    }
}