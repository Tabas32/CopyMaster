#include <stdlib.h>     //calloc, free

#include <sys/types.h>  //
#include <sys/stat.h>   // open
#include <fcntl.h>      //

#include <unistd.h>     //write, read, close 

#include <errno.h>      // errno

#include "../flags.h"
#include "../errors.h"

int append(int* out, const char* outfile){
    *out = open(outfile, O_RDWR | O_APPEND);
    if(*out == -1){
        if(errno == ENOENT){
            return error_description(22, file_missing_error, "-a");
        }
        return error_description(22, other_error, "-a");
    }

    return 0;
}
