#ifndef USERS_H
#define USERS_H

//inicjalizacja procesu producent dla danej kolejki
int producer(int queue_id);

//inicjalizacja procesu super producent
int super_producer(void);

//inicjalizacja procesu konsumenta dla danej kolejki
int consumer(int queue_id,float pr);

//losowanie litery do wiadomosci
char random_lit(int queue_id);

//losowanie kolejki do wyslanai wiadomosci przez super producenta
int random_queue(int queue_id);

#endif
