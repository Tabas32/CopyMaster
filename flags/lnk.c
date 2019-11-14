#include <unistd.h>

#include "../errors.h"

int lnk(char* infile, char* outfile){
    if(link(infile, outfile)){
        return error_description(30, cannot_create_error, "-K");
    }
}
