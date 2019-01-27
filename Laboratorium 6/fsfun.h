#ifndef FSFUN_H
#define FSFUN_H

#include "fsdata.h"

/*tworzenie systemu plikow*/
int create_fs(char* fs_name, int size);

/* wyswietlenie zawartosci systemu*/
int ls_fs(char* fs_name);

/*kopiownie z systemu plikow do linuxa*/
int copy_from_fs(char* fs_name, char* filename, char* goalfile);

/*kopiownie do systemu plikow z linuxa*/
int copy_to_fs(char* fs_name, char* linux_file, char* filename);

/*usuniecie pliku z systemu plikow*/
int rm_file_fs(char* fs_name, char* filename);

/*funkcja znajdujaca miejsce w pamieci dla danego pliku*/
int find_hole(int size, FILE* file);

/*wyswietlenie informacji o systemi plikow*/
int info_fs(char* fs_name);

/*usuniecie systemu plikow*/
int rm_fs(char* fs_name);

/*wyswietlenie mapy zajetosci systemu*/
int disp_map(char* fs_name);

/*wyswietlenie iformacji o instrukcji*/
int fun_help(const char* fun_name);

#endif //FSFUN_H