#ifndef QUEUE_H
#define QUEUE_H

#include "data.h"
#include "message.h"
#include "monitor.h"

class Queue : public Monitor
{
	public:
		struct Message table[BUFFER_SIZE];
		int head;
		int tail;
		int size;
		int q_id;

		Queue(int q_id);

		//wyslanie wiadomosci do kolejki
		void send_msg(struct Message m);

		//odczytanie wiadomosci z kolejki
		struct Message read_msg();

};

#endif