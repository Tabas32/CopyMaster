#define _BSD_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "../errors.h"

int trnc(char* file, char* argument){
    int size;
    if(sscanf(argument, "%d", &size) != 1)
        return error_description(31, other_error, "-t");

    if(truncate(file, size)){
        return error_description(31, notchanged_error, "-t");
    }

    return 0;
}
