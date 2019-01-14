#ifndef MESSAGE_H
#define MESSAGE_H
#include "data.h"

//struktura wiadomosci
struct Message
{
	int pri;
	int source;
	char m[MESS_SIZE];
    int next;
    int free_place;
};

//ustawienie wartosci pol struktury wiadomosc
void set_msg(struct Message *m, int pri, int src, char* msg);


#endif