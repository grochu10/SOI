#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fsfun.h"
#include "fsdata.h"

/*tworzenie systemu plikow*/
int create_fs(char* fs_name, int size)
{
    FILE *file = fopen(fs_name, "w");
    char *p = malloc(size);
    superBlock_t superBlock;
    strcpy(superBlock.name_fs,fs_name);
    superBlock.size_fs = size;
    superBlock.metadataStart = sizeof(superBlock_t);
    superBlock.metadataNumber = size/(4096);
    superBlock.fileDataStart  = superBlock.metadataStart;
    superBlock.fileDataStart += superBlock.metadataNumber*sizeof(fileMetaData_t);
    superBlock.fileDataSize = size - superBlock.fileDataStart;
    superBlock.metadataSize = 0;
    printf("Tworzenie systemu plikow: %.20s\n", fs_name);
    memset(p, 0, size);
    memcpy(p, &superBlock, sizeof(superBlock_t));
    fwrite(p, size, 1, file);
    fclose(file);
    free(p);
    printf("Tworzenie systemu plikow zakonczone.\n");
    return 0;
}

/* wyswietlenie zawartosci systemu*/
int ls_fs(char* fs_name)
{
    superBlock_t super;
    int i;
    FILE* file = fopen(fs_name,"r");
    fread(&super,sizeof(superBlock_t),1,file);
    if(super.metadataSize == 0) printf("Brak plikow w systemie.\n");
    else{
        printf("Zawartosc systemu plikow:\n");
        printf("Nazwa pliku\t Rozmiar\t Adres\n");
    }
    fileMetaData_t meta[super.fileDataStart];
    fread(&meta,super.fileDataStart,1,file);
    for(i = super.metadataStart;i < super.fileDataStart;i += sizeof(fileMetaData_t))
    {
        if(meta[i].used == 1){
            printf("%s\t%d\t%d",meta[i].name,meta[i].size,meta[i].base);
        }  
    }
    fclose(file);
    return 0;
}
/*kopiownie z systemu plikow do linuxa*/
int copy_from_fs(char* fs_name, char* filename, char* goalfile)
{
    superBlock_t super;
    int i;
    FILE* file = fopen(fs_name,"r+");
    FILE* copy_file = fopen(goalfile,"w");
    fread(&super,sizeof(superBlock_t),1,file);
    fileMetaData_t meta[super.fileDataStart];
    fread(&meta,super.fileDataStart,1,file);
    for(i = super.metadataStart;i < super.fileDataStart;i += sizeof(fileMetaData_t))
    {
        if((meta[i].used == 1) && (strcmp(meta[i].name,filename == 0))){
            memcpy();
        }
    }
    fclose(file);
    fclose(copy_file);
    return 0;
}

/*kopiownie do systemu plikow z linuxa*/
int copy_to_fs(char* fs_name, char* linux_file, char* filename)
{
    superBlock_t super;
    int i;
    FILE* file = fopen(fs_name,"r+");
    FILE* copy_file = fopen(linux_file,"r");
    long filesize = ftell(copy_file);
    fread(&super,sizeof(superBlock_t),1,file);
    fileMetaData_t meta[super.fileDataStart];
    fread(&meta,super.fileDataStart,1,file);
    /*if(super.metadataSize == 0)/*jesli system plikow pusty
    {
        fseek(file,super.metadataStart,0);
        meta[super.metadataStart].used = 1;
        strcpy(meta[super.metadataStart].name,filename);
        meta[super.metadataStart].size = filesize;
        meta[super.metadataStart].base = super.fileDataStart;
        ++super.metadataSize;
        fseek(file,0,0);
        fwrite(&super,sizeof(super),1,file);
        fseek(file, super.metadataStart, 0);
        fwrite(&meta[super.metadataStart],sizeof(fileMetaData_t),1,file);
    }
    else{ /*gdy w systemie sa jakies pliki*/
    for(i = super.metadataStart;i < super.fileDataStart;i += sizeof(fileMetaData_t))
    {
        if((meta[i].used == 0)){
            if((meta[i].size >= filesize) || (meta[i].size == 0)){
                fseek(file,i,0);
                meta[i].used = 1;
                strcpy(meta[i].name,filename);
                meta[i].size = filesize;
                /*gdy plik wchodzi na pierwsza metadane*/
                if(i == super.metadataStart)meta[i].base = super.fileDataStart;
                else if(meta[i].size == 0)meta[i].base = meta[i-sizeof(fileMetaData_t)].base + meta[i-sizeof(fileMetaData_t)].size+1;
                ++super.metadataSize;
                fseek(file,0,0);
                fwrite(&super,sizeof(super),1,file);
                fseek(file, i, 0);
                fwrite(&meta[i],sizeof(fileMetaData_t),1,file);
            }
        }else
        {
            continue;
        }
        break;   
    }
    fclose(copy_file);
    fclose(file);
    return 0;
}

/*usuniecie pliku z systemu plikow*/
int rm_file_fs(char* fs_name, char* filename)
{
    superBlock_t super;
    int i;
    FILE* file = fopen(fs_name,"r+");
    fread(&super,sizeof(superBlock_t),1,file);
    fileMetaData_t meta[super.fileDataStart];
    fread(&meta,super.fileDataStart,1,file);
    for(i = super.metadataStart;i < super.fileDataStart;i += sizeof(fileMetaData_t))
    {
        if(strcmp(meta[i].name,filename) == 0){
            meta[i].used = 0;
            --super.metadataSize;
            fseek(file,0,0);
            fwrite(&super,sizeof(super),1,file);
            fseek(file, i, 0);
            fwrite(&meta[i],sizeof(fileMetaData_t),1,file);
            printf("Plik %s zostal poprawnie usuniety.\n",filename);
        }  
    }
    fclose(file);
    return 0;
}

/*wyswietlenie informacji o systemi plikow*/
int info_fs(char* fs_name)
{
    superBlock_t super;
    FILE* file = fopen(fs_name,"r");
    fread(&super,sizeof(superBlock_t),1,file);
    printf("Nazwa systemu plikow: %s\n",super.name_fs);
    printf("Rozmiar systemu plikow: %d\n",super.size_fs);
    printf("Index poczatku danych o plikach: %d\n",super.metadataStart);
    printf("Maksymalna ilosc plikow w systemie: %d\n",super.metadataNumber);
    printf("Index poczatku danych zawartych w plikach: %d\n",super.fileDataStart);
    printf("Ilosc pamieci dla danych w systemie plikow: %d\n",super.fileDataSize);
    printf("Ilosc plikow w systemie: %d.\n",super.metadataSize);
    fclose(file);
    return 0;
}

/*wyswietlenie iformacji o instrukcji*/
int fun_help(const char* fun)
{
    /*ls_fs*/
    if(strcmp(fun,"ls_fs") == 0){
        printf("Skladnia: ls_fs <nazwa_systemu_plikow>\n");
        printf("Opis:\nFunkcja do wyswietlania aktualnej zawartosci systemu plikow.\n");
        return 0;
    }

    /*crt_fs*/
    if(strcmp(fun,"crt_fs") == 0){
        printf("Skladnia: crt_fs <nazwa_systemu_plikow>\n");
        printf("Opis:\nFunkcja do tworzenia nowego systemu plikow.\n");
        return 0;
    }
    
    /*copy_from_fs*/
    if(strcmp(fun,"cp_from") == 0){
        printf("Skladnia: cp_from <nazwa_systemu_plikow> <nazwa_pliku_w systemie_plikow> <sciezka_i_nazwa_pliku_w linuxie>\n");
        printf("Opis:\nFunkcja do kopiowania pliku o danej nazwie z systemu plikow \ndo pliku z podana sciezka o podanej nazwie systemu Linux.\n");
        return 0;
    }

    /*copy_to_fs*/
    if(strcmp(fun,"cp_to") == 0){
        printf("Skladnia: cp_to <nazwa_systemu_plikow> <sciezka_i_nazwa_pliku_w linuxie> <nazwa_pliku_w systemie_plikow>\n");
        printf("Opis:\nFunkcja do kopiowania pliku o danej nazwie i sciezce z systemu Linux \ndo pliku o podanej nazwie w systemie plikow.\n");
        return 0;
    }

    /*fs_rm_file*/
    if(strcmp(fun,"rm_file_fs") == 0){
        printf("Skladnia: rm_file_fs <nazwa_systemu_plikow> <nazwa_pliku_do_usuniecia>\n");
        printf("Opis:\nFunkcja do usuwania pliku o danej nazwie z systemu plikow.\n");
        return 0;
    }

    /*info_fs*/
    if(strcmp(fun,"info_fs") == 0){
        printf("Skladnia: info_fs <nazwa_systemu_plikow> \n");
        printf("Opis:\nFunkcja do wyswietlania najwazniejszych informacji o systemie plikow.\n");
        return 0;
    }

    /*gcp_to*/
    if(strcmp(fun,"gcp_to") == 0){
        printf("Skladnia: gcp_to <nazwa_systemu_plikow> <sciezka_i_nazwa_pliku_w linuxie> <nazwa_pliku_w systemie_plikow> <sciezka_i_nazwa_pliku_w linuxie> ...\n");
        printf("Opis:\nFunkcja do kopiowania kilku plikow naraz o danych nazwach i sciezkach z systemu Linux \ndo plikow o podanych nazwach w systemie plikow.\n");
        return 0;
    }

    /*gcp_from*/
    if(strcmp(fun,"gcp_from") == 0){
        printf("Skladnia: gcp_from <nazwa_systemu_plikow> <nazwa_pliku_w systemie_plikow> <sciezka_i_nazwa_pliku_w linuxie> <nazwa_pliku_w systemie_plikow> ...\n");
        printf("Opis:\nFunkcja do kopiowania kilku plikow o danych nazwach z systemu plikow \ndo plikow o podanych nazwach z podana sciezka systemu Linux.\n");
        return 0;
    }

    printf("ERROR. Brak takiej funkcji w systemie.\n");
    return 0;
}