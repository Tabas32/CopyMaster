#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include "../errors.h"

int noFlag(int* out, const char* outfile){
    *out = open(outfile, O_RDWR | O_CREAT, 00666);
    if(*out == -1){
        return error_description(21, other_error, "bez prepinaca");
    }

    return 0;
}
