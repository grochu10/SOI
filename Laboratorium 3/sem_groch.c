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
#include "users.h"

void reader(int input)
{
	int queue_id, i, size;	
	struct Message msg;

	queue_id = shmget(QUEUE_KEY, sizeof(struct Queue), 0666);
	struct Queue *queue = (struct Queue*)shmat(queue_id, NULL, 0);	// utworzenie wskaznika na wspoldzielony bufor

	int fullId = semget(FULL_KEY, 1, 0600);
	int emptyId = semget(EMPTY_KEY, 1, 0600);
	int mutexId = semget(MUTEX_KEY, 1, 0600);
	
	size = 0;
	i = 0;

	while(i<input)
	{	
	
		down(fullId, 0);
		
		down(mutexId, 0);

		printf("------------------\n");
		msg = read_msg(queue);
		if(msg.pri == 1) printf("Uzytkownik A napisal: %c\n", msg.m);
		else if (msg.pri == 0) printf("Uzytkownik B napisal: %c\n", msg.m);

		up(mutexId, 0);
		up(emptyId, 0);

		size = get_queue_size(queue);
		printf("W buforze: %d\n", size);

		
		i++;	
	}
}



int main(int argc, char * argv[])
{
	int queue_id, status, size, A, B, read;	
	
	queue_id = shmget(QUEUE_KEY, sizeof(struct Queue), IPC_CREAT|0666);
	struct Queue *queue = (struct Queue*)shmat(queue_id, NULL, 0);	// utworzenie wskaznika na wspoldzielony bufor

	queue_init(queue);

	int fullId = semget(FULL_KEY, 1, IPC_CREAT|IPC_EXCL|0600);	
	semctl(fullId, 0, SETVAL, (int)0);
	
	int emptyId = semget(EMPTY_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
	semctl(emptyId, 0, SETVAL, BUFFER_SIZE);
	
	int mutexId = semget(MUTEX_KEY, 1, IPC_CREAT|IPC_EXCL|0600);
 	semctl(mutexId, 0, SETVAL, (int)1);

	if(argc == 1)
	{
		printf("ARGUMENT ERROR!\n");
		return 0;
	}

	A = atoi(argv[1]);
	B = atoi(argv[2]);
	read = atoi(argv[3]);
	
	
	if( B > 0 )
	{
		switch (fork())
		{
			case 0:
				printf("Uzytkownik B zalogowal sie\n");
				user_B(B);
				exit(0);
			default:
				break;
		}
	}

	if( A > 0)
	{
		switch (fork())
		{
			case 0:
				printf("Uzytkownik A zalogowal sie\n");
				user_A(A);
				exit(0);
			default:
				break;
		}
	}

	if( read > 0)
	{
		switch (fork())
		{
			case 0:
				printf("Witam jestem czatem\n");
				reader(read);
				exit(0);
			default:
				break;
		}
	}
	while(wait(&status) != -1);	

	
	shmdt(queue);

	return 0;
}