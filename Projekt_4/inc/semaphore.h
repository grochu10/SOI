#ifndef SEMAPHORE_H
#define SEMAPHORE_H

//dekrementacja semafora
void sem_down(int semid, int semnum);

//inkrementacja semafora
void sem_up(int semid, int semnum);
#endif