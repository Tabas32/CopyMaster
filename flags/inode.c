#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>

#include "../errors.h"

int inode(const char* file, char* argument){
    int num;
    if(sscanf(argument, "%d", &num) != 1)
        return error_description(27, other_error, "-i");

    struct stat file_stat;
    if(stat(file, &file_stat))
        return error_description(27, other_error, "-i");

    if(!S_ISREG(file_stat.st_mode)){
        file_type_error("-i");
        return 27;
    }

    if(file_stat.st_ino == (ino_t)num)
        return 0;
    else{
        inode_error("-i");
        return 27;
    }
}
