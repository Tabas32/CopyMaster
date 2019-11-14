#include <sys/stat.h>
#include <sys/types.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../errors.h"

int mod(const char* file, char* argument){
    mode_t mode = 0;
    if(argument[0] != '0' && strlen(argument) == 9){
        for(int i = 0; i < 3; i++){
            if(argument[i * 3] == 'r'){
                mode += pow(2,8 - (i * 3));
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
        permissions_error("-m");
        return -1;
    }

    if(chmod(file, mode) == -1){
        return error_description(-1, other_error, "-m");
    }

    return 0;
}
