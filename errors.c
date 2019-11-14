#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "errors.h"

int error_description(int ret_val, void (*errorType)(char* option), char* option){
    if(errno != 0){
        fprintf(stderr, "%s : %d\n", option, errno);
        fprintf(stderr, "%s : %s\n", option, strerror(errno));
    }

    (*errorType)(option);

    return ret_val;
}

void file_missing_error(char* option){
    if(errno == ENOENT){
        fprintf(stderr, "%s : SUBOR NEEXISTUJE\n", option);
    }
    else{
        fprintf(stderr, "%s : INA CHYBA\n", option);
    }
}

void other_error(char* option){
    fprintf(stderr, "%s : INA CHYBA\n", option);
}

void options_error(char* option){
    fprintf(stderr, "%s : CHYBA PREPINACOV\n", option);
}

void notDir_error(char* option){
    fprintf(stderr, "%s : VSTUPNY SUBOR NIE JE ADRESAR\n", option);
}

void position_error(char* option){
    if(errno == EINVAL || errno == EOVERFLOW){
        fprintf(stderr, "%s : CHYBA POZICIE\n", option);
    }
    else{
        fprintf(stderr, "%s : INA CHYBA\n", option);
    }
}

void permissions_error(char* option){
    fprintf(stderr, "%s : ZLE PRAVA\n", option);
}

void file_type_error(char* option){
    fprintf(stderr, "%s : ZLY TYP VSTUPNEHO SUBORU\n", option);
}

void inode_error(char* option){
    fprintf(stderr, "%s : ZLY INODE\n", option);
}

void file_exists_error(char* option){
    fprintf(stderr, "%s : SUBOR EXISTUJE\n", option);
}

void cannot_create_error(char* option){
    fprintf(stderr, "%s : VYSTUPNY SUBOR NEVYTVORENY\n", option);
}

void notdeleted_error(char* option){
    fprintf(stderr, "%s : SUBOR NEBOL ZMAZANY\n", option);
}

void notchanged_error(char* option){
    fprintf(stderr, "%s : VSTUPNY SUBOR NEZMENENY\n", option);
}
