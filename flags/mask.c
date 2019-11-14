#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>

void mask(char* arguments){
    mode_t mask = 0;
    int k = 0, per = 0;

    for(int i = 0, len = strlen(arguments); i < len; i++){
        if(arguments[i] == '-'){
            if(arguments[i - 1] == 'u')
                k = 0;

            if(arguments[i - 1] == 'g')
                k = 1;

            if(arguments[i - 1] == 'o')
                k = 2;

            if(arguments[i + 1] == 'r')
                per = 0;

            if(arguments[i + 1] == 'w')
                per = 1;

            if(arguments[i + 1] == 'x')
                per = 2;

            mask += pow(2, 8 - (k * 3 + per));
        }
    }

    umask(mask);
}
