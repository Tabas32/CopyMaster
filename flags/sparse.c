#define _XOPEN_SOURCE 500

#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include "../errors.h"

int sparse(int out, size_t buf_size){
    //make backup    
    int swp  = open(".temp.swp", O_RDWR | O_CREAT , 00666);
    if(swp == -1){
        return error_description(41, other_error, "-S");
    }
    
    if(lseek(out, 0, SEEK_SET) == -1) return error_description(41, other_error, "-S");
    
    char* buffer = calloc(1000000, sizeof(char));
    if(buffer == NULL){
        return error_description(41, other_error, "-S");
    }
    
    
    int er;
    while((er = read(out, buffer, 1000000)) > 0){
        if(er == -1){
            return error_description(41, other_error, "-S");
        }

        er = write(swp, buffer, er);
        if(er == -1){
            return error_description(41, other_error, "-S");
        }
    }

    free(buffer);

    //sparse to outfile
    if(lseek(swp, 0, SEEK_SET) == -1) return error_description(41, other_error, "-S");
    if(ftruncate(out, 0) == -1) return error_description(41, other_error, "-S");
    if(lseek(out, 0, SEEK_SET) == -1) return error_description(41, other_error, "-S");

    buffer = calloc(buf_size, sizeof(char));
    if(buffer == NULL){
        return error_description(41, other_error, "-S");
    }

    char* writeBuffer = (char*)calloc(buf_size, sizeof(char));
    if(writeBuffer == NULL){
        return error_description(41, other_error, "-S");
    }

    int bytes;
    while((bytes = read(swp, buffer, buf_size)) > 0){
        if(bytes == -1){
            return error_description(41, other_error, "-S");
        }
 
        int toJump = 0;
        int toWrite = 0;

        for(int i = 0; i < bytes; i++){
            if(buffer[i] == '\0'){
                toJump++;
                if(toJump == 1 && toWrite > 0){
                    er = write(out, writeBuffer, toWrite);
                    if(er == -1){
                        return error_description(41, other_error, "-S");
                    }
                }
                toWrite = 0;
            }
            else{
                writeBuffer[toWrite] = buffer[i];
                toWrite++;
                if(toWrite == 1 && toJump > 0){
                    er = lseek(out, toJump, SEEK_CUR);
                    if(er == -1){
                        return error_description(41, other_error, "-S");
                    }
                }
                toJump = 0;
            }
        }

        if(toJump > 0){
            er = lseek(out, toJump, SEEK_CUR);
            if(er == -1){
                return error_description(41, other_error, "-S");
            }
        }
        if(toWrite > 0){
            er = write(out, writeBuffer, toWrite);
            if(er == -1){
                return error_description(41, other_error, "-S");
            }
        } 
    }
    
    //delete temp file
    if(unlink(".temp.swp") == -1)
        return error_description(41, other_error, "-S");
    er = close(swp);
    if(er == -1){
        return error_description(41, other_error, "-S");
    }

    free(buffer);
    free(writeBuffer);

    return 0;
}
