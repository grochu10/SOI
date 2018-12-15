#ifndef USERS_H
#define USERS_H

//inicjalizacja procesu producent dla danej kolejki
void producer(int queue_id);

//inicjalizacja procesu super producent dla danej kolejki
void super_producer(void);

//losowanie litery do wiadomosci
char random_lit(void);

//losowanie kolejki do wyslanai wiadomosci przez super producenta
int random_queue(void);

#endif