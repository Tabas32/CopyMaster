#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <errno.h>

#include <math.h>
#include <string.h>

#include "../errors.h"

int create(int* out, const char* outfile, char* argument){
    mode_t mode = 0;
    if(argument[0] != '0' && strlen(argument) == 9){
        for(int i = 0; i < 3; i++){
            if(argument[i * 3] == 'r'){
                mode += pow(2, 8 - (i * 3));
            }

            if(argument[i * 3 + 1] == 'w'){
                mode += pow(2, 8 - (i * 3 + 1));
            }

            if(argument[i * 3 + 2] == 'x'){
                mode += pow(2, 8 - (i * 3 + 2));
            }
        }
    }
    else if(argument[0] == '0' && strlen(argument) == 4){
        mode = strtol(argument, NULL, 8);
    }
    else{
        options_error("-c");
        return 20;
    }

    
    *out = open(outfile, O_RDWR | O_CREAT | O_EXCL, mode);
    if(*out == -1){
        if(errno == EEXIST)
            return error_description(23, file_exists_error, "-c");
        return error_description(23, other_error, "-c");
    }

    return 0;
}
