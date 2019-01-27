#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsdata.h"
#include "fsfun.h"

int main(int argc, char* argv[])
{
    if(argc < 3){
        printf("ERROR. Za malo argumentow.\n");
        return -1;
    }

    if(argc == 3){
        if(strcmp(argv[1],"--help") == 0){
            fun_help(argv[2]);
            return 0;
        }   
        if(strcmp(argv[1],"ls_fs") == 0){
            ls_fs(argv[2]);
            return 0;
        }
        if(strcmp(argv[1],"info_fs") == 0){
            info_fs(argv[2]);
            return 0;
        }
        if(strcmp(argv[1],"rm_fs") == 0){
            rm_fs(argv[2]);
            return 0;
        }
        if(strcmp(argv[1],"disp_map") == 0){
            disp_map(argv[2]);
            return 0;
        }
    }

    if(argc == 4){
        if(strcmp(argv[1],"crt_fs") == 0){
            create_fs(argv[2],atoi(argv[3]));
            return 0;
        }
        if(strcmp(argv[1],"rm_file_fs") == 0){
            rm_file_fs(argv[2],argv[3]);
            return 0;
        }
    }

    if(argc == 5){
        if(strcmp(argv[1],"cp_to") == 0){
            copy_to_fs(argv[2],argv[3],argv[4]);
            return 0;
        }
        if(strcmp(argv[1],"cp_from") == 0){
            copy_from_fs(argv[2],argv[3],argv[4]);
            return 0;
        }
    }

    if(strcmp(argv[1],"gcp_to") == 0){
        int i;
        int j;
        j = argc -(argc - 3)/2;
        for(i = 3;i < argc -(argc - 3)/2; ++i,++j)
            copy_to_fs(argv[2],argv[i],argv[j]);
        return 0;
    }

    if(strcmp(argv[1],"gcp_from") == 0){
        int i;
        int j;
        j = argc-(argc - 3)/2;
        for(i = 3;i < argc -(argc - 3)/2; ++i,++j)
            copy_from_fs(argv[2],argv[i],argv[j]);
        return 0;
    }

    printf("ERROR. Bledne argumenty wywolania.\n");
    return 0;
}