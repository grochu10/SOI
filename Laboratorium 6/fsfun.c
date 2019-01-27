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
    printf("Tworzenie systemu plikow: %.20s o pojemnosci %d.\n", fs_name,size);
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
        printf("Nazwa pliku\t\t\tRozmiar\t\tAdres\n");
    }
    fileMetaData_t meta;
    for(i = super.metadataStart;i < super.fileDataStart;i += sizeof(fileMetaData_t))
    {
        fseek(file,i,0);
        fread(&meta,sizeof(fileMetaData_t),1,file);
        if(meta.used == 1){
            printf("%-32s%-16d%-16d\n",meta.name,meta.size,meta.base);
        }  
    }
    fclose(file);
    return 0;
}

/*kopiownie z systemu plikow do linuxa*/
int copy_from_fs(char* fs_name, char* filename, char* goalfile)
{
    printf("Kopiowanie pliku z systemu plikow...\n");
    superBlock_t super;
    int i;
    FILE* file = fopen(fs_name,"r");
    FILE* copy_file = fopen(goalfile,"w");
    fread(&super,sizeof(superBlock_t),1,file);
    fileMetaData_t meta;
    for(i = super.metadataStart;i < super.fileDataStart;i += sizeof(fileMetaData_t))
    {
        fseek(file,i,0);
        fread(&meta,sizeof(fileMetaData_t),1,file);
        if((meta.used == 1) && (strcmp(meta.name,filename)== 0)){
            fseek(file,meta.base,0);
            char data[meta.size];
            fread(&data,meta.size,1,file);
            fwrite(data,meta.size,1,copy_file);
        }
    }
    fclose(file);
    fclose(copy_file);
    printf("Plik %s zostal skopiowany prawidlowo.\n",meta.name);
    return 0;
}

/*kopiownie do systemu plikow z linuxa*/
int copy_to_fs(char* fs_name, char* linux_file, char* filename)
{
    superBlock_t super;
    int i;
    FILE* file = fopen(fs_name,"r+");
    FILE* copy_file = fopen(linux_file,"r");
    fseek(copy_file,0,SEEK_END);
    long filesize = ftell(copy_file);
    fread(&super,sizeof(superBlock_t),1,file);
    fileMetaData_t meta;
    fseek(file,super.metadataStart,0);
    if(super.metadataNumber == super.metadataSize)
    {
        printf("Nie mozna dodac pliku. Za duzo plikow w systemie.\n");
        return 0;
    }
    printf("Kopiowanie pliku do systemu...\n");
    for(i = super.metadataStart;i < super.fileDataStart;i += sizeof(fileMetaData_t))
    {
        fread(&meta,sizeof(fileMetaData_t),1,file);
        if((meta.used == 0)){
            fseek(file,i,0);
            strcpy(meta.name,filename);
            meta.size = filesize;
            /*gdy plik wchodzi na pierwsza metadane*/
            meta.base = find_hole(filesize, file);
            if(meta.base == 0)
            {
                printf("Brak miejsca na dysku na ten plik.\n");
                return -1;
            }
            meta.used = 1;
            ++super.metadataSize;
            fseek(file,0,0);
            fwrite(&super,sizeof(super),1,file);
            fseek(file, i, 0);
            fwrite(&meta,sizeof(fileMetaData_t),1,file);
            fseek(file,meta.base,0);
            char data[meta.size];
            fseek(copy_file, 0, 0);
            fread(&data,meta.size,1,copy_file);
            fseek(copy_file, meta.base, 0);
            fwrite(data,meta.size,1,file);
            printf("Plik %s zostal poprawnie skopiowany do systemu.\n",filename);
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
    fileMetaData_t meta;
    for(i = super.metadataStart;i < super.fileDataStart;i += sizeof(fileMetaData_t))
    {
        fseek(file,i,0);
        fread(&meta,sizeof(fileMetaData_t),1,file);
        if(strcmp(meta.name,filename) == 0){
            printf("Usuwanie pliku z systemu...\n");
            meta.used = 0;
            --super.metadataSize;
            fseek(file,0,0);
            fwrite(&super,sizeof(super),1,file);
            fseek(file, i, 0);
            fwrite(&meta,sizeof(fileMetaData_t),1,file);
            printf("Plik %s zostal poprawnie usuniety.\n",filename);
            fclose(file);
            return 0;
        }  
    }
    printf("W systemie nie ma pliku o takiej nazwie.\n");
    fclose(file);
    return 0;
}

/*funkcja znajdujaca miejsce w pamieci dla danego pliku*/
int find_hole(int size, FILE* file)
{
    fileMetaData_t meta;
    superBlock_t super;
    fseek(file, 0, 0);
    fread(&super,sizeof(superBlock_t),1,file);
    dataInfo_t dataMap[super.metadataNumber];
    int i,j; 
    i = 0;     
    if(super.metadataSize == 0)
        return super.fileDataStart;
    // stworzenie mapy zajetosci sekcji danych
    fseek(file, super.metadataStart, 0);
    for(j = 0; j < super.metadataNumber; j++)
    {
        fread(&meta, sizeof(fileMetaData_t), 1, file);
        if( meta.used == 1)
        {
            dataMap[i].base = meta.base;
            dataMap[i].size = meta.size;
            strcpy(dataMap[i].name,meta.name);
            dataMap[i].metaAdr = super.metadataStart;
            dataMap[i].metaAdr+= j*sizeof(fileMetaData_t); 
            i++;
        }     
    }

    // sortowanie zajetych obszarow
    for(i = 0; i < (super.metadataSize-1); i++)
        for(j = 0; j < (super.metadataSize-1); j++)
            if( dataMap[j].base > dataMap[j+1].base )
            {
                dataInfo_t tmp;
                tmp = dataMap[j];
                dataMap[j] = dataMap[j+1];
                dataMap[j+1] = tmp;
            }

    // przydzielenie miejsca przed pierwszym plikiem
    if((dataMap[0].base-super.fileDataStart)>=size)
        return super.fileDataStart;

    // przydzielenie miejsca pomiędzy plikami
    for(j=1; j<(super.metadataSize); j++)
        if( dataMap[j].base - dataMap[j-1].base - dataMap[j-1].size >= size )
            return dataMap[j-1].base + dataMap[j-1].size;
    
    // przydzielenie miejsca po ostatnim pliku
    if( super.size_fs - dataMap[super.metadataSize-1].base-dataMap[super.metadataSize-1].size>=size )
        return dataMap[super.metadataSize-1].base + dataMap[super.metadataSize-1].size;

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

/*usuniecie systemu plikow*/
int rm_fs(char* fs_name)
{
    printf("Trwa usuwanie systemu...\n");
    if (remove(fs_name) != 0)
        printf("Brak systemu plikow o takiej nazwie.\n");
    printf("System plikow usuniety prawidlowo.\n");
    return 0;
}

/*wyswietlenie mapy zajetosci systemu*/
int disp_map(char* fs_name)
{
    printf("Tworzenie mapy zajetosci...\n");
    superBlock_t super;
    fileMetaData_t meta;
    int i,j; 
    i = 0; 
    FILE* file = fopen(fs_name,"r");
    fread(&super,sizeof(superBlock_t),1,file);
    if(super.metadataSize == 0)
    {
    printf("Mapa zajetosci:\n%d - %d wolna przestrzen\n",super.fileDataStart,super.size_fs);
    return 0;
    }
    else{
        printf("Mapa zajetosci:\n");
    }
    fseek(file, 0, 0);
    fread(&super,sizeof(superBlock_t),1,file);
    dataInfo_t dataMap[super.metadataNumber];    
    if(super.metadataSize == 0)
        return super.fileDataStart;
    // stworzenie mapy zajetosci sekcji danych
    fseek(file, super.metadataStart, 0);
    for(j = 0; j < super.metadataNumber; j++)
    {
        fread(&meta, sizeof(fileMetaData_t), 1, file);
        if( meta.used == 1)
        {
            dataMap[i].base = meta.base;
            dataMap[i].size = meta.size;
            strcpy(dataMap[i].name,meta.name);
            dataMap[i].metaAdr = super.metadataStart;
            dataMap[i].metaAdr+= j*sizeof(fileMetaData_t); 
            i++;
        }     
    }
    // sortowanie zajetych obszarow
    for(i = 0; i < (super.metadataSize-1); i++)
        for(j = 0; j < (super.metadataSize-1); j++)
            if( dataMap[j].base > dataMap[j+1].base )
            {
                dataInfo_t tmp;
                tmp = dataMap[j];
                dataMap[j] = dataMap[j+1];
                dataMap[j+1] = tmp;
            }
    for(i = 0;i < super.metadataSize;i++)
    {
        if(dataMap[0].base != super.fileDataStart)
        {
            printf("%d - %d\twolna przestrzen\n", super.fileDataStart,dataMap[0].base);
        }
        j = dataMap[i].base + dataMap[i].size;
        printf("%d - %d\tplik %s\n",dataMap[i].base,j,dataMap[i].name);
        if((i != super.metadataSize-1)&&(j != dataMap[i+1].base))
            printf("%d - %d\twolna przestrzen\n", j,dataMap[i+1].base);
        if((i == super.metadataSize-1)&&(j != super.size_fs))
            printf("%d - %d\twolna przestrzen\n", j,super.size_fs);
    }
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
        printf("Skladnia: gcp_to <nazwa_systemu_plikow> <sciezki_i_nazwy_plikow_w linuxie> <nazwy_plikow_w systemie_plikow> \n");
        printf("Opis:\nFunkcja do kopiowania kilku plikow naraz o danych nazwach i sciezkach z systemu Linux \ndo plikow o podanych nazwach w systemie plikow.\n");
        return 0;
    }

    /*gcp_from*/
    if(strcmp(fun,"gcp_from") == 0){
        printf("Skladnia: gcp_from <nazwa_systemu_plikow> <nazwy_plikow_w systemie_plikow> <sciezki_i_nazwy_plikow_w linuxie>\n");
        printf("Opis:\nFunkcja do kopiowania kilku plikow o danych nazwach z systemu plikow \ndo plikow o podanych nazwach z podana sciezka systemu Linux.\n");
        return 0;
    }

    /*rm_fs*/
    if(strcmp(fun,"rm_fs") == 0){
        printf("Skladnia: rm_fs <nazwa_systemu_plikow>\n");
        printf("Opis:\nFunkcja do usuwania systemu plikow o podanej nazwie.\n");
        return 0;
    }

    printf("ERROR. Brak takiej funkcji w systemie.\n");
    return 0;
}