#ifndef QUEUE_H
#define QUEUE_H

#include "data.h"
#include "message.h"

struct Queue
{
	struct Message table[BUFFER_SIZE];
	int head;
	int tail;
	int size;
};

void queue_init(struct Queue *q);

int get_queue_size(struct Queue *q);

//wyslanie wiadomosci do kolejki
void send_msg(struct Queue *q, struct Message m);

//odczytanie wiadomosci z kolejki
struct Message read_msg(struct Queue *q);

#endif