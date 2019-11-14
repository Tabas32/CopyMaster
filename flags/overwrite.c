#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <errno.h>

#include "../errors.h"

int overwrite(int* out, const char* outfile){
    *out = open(outfile, O_RDWR | O_TRUNC);
    if(*out == -1){
        if(errno == ENOENT){
            return error_description(24, file_missing_error, "-o");
        }
        return error_description(24, other_error, "-o");
    }

    return 0;
}
