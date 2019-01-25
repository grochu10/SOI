#ifndef FSDATA_H
#define FSDATA_H

typedef struct{
    int used;
    char name[256];
    unsigned int base;
    unsigned int size;
}fileMetaData_t;

typedef struct {
        // nazwa systemu plikow
        char name_fs[256];
        // Rozmiar dysku
        int size_fs;
        // adres początku sekcji metadanych
        int metadataStart;
        // max ilość obiektów metadanych
        int metadataNumber;
        // adres początku sekcji danych plików
        int fileDataStart;
        // rozmiar sekcji danych plików
        int fileDataSize;
        int metadataSize;
}superBlock_t;

#endif //FSDATA_H