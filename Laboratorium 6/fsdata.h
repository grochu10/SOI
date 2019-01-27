#ifndef FSDATA_H
#define FSDATA_H

/*struktura metadanych o pliku*/
typedef struct{
    int used;
    char name[256];
    unsigned int base;
    unsigned int size;
}fileMetaData_t;

/*struktura superbloku*/
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

/*struktura informacji o adresach pliku*/
typedef struct{
    int metaAdr;
    int size;
    int base;
    char name[256];
}dataInfo_t;

#endif //FSDATA_H