#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include "../errors.h"

int parse_input(int* fd_in, int* fd_out, int x, int pos1,
        int* pos2, int num);

int seek(int in, int out, char* arguments, size_t buf_size){
    char x;
    int pos1, pos2, num, er = 0; 
    if(sscanf(arguments, "%c,%d,%d,%d", &x, &pos1, &pos2, &num) != 4)
        return error_description(33, options_error, "-l");

    int whence;
    if(x == 'b') whence = SEEK_SET;
    if(x == 'e') whence = SEEK_END;
    if(x == 'c') whence = SEEK_CUR;
    
    if((num = parse_input(&in, &out, whence, pos1, &pos2, num)) == -1)
        return error_description(33, position_error, "-l");

    char* buffer = calloc(buf_size, sizeof(char));
    if(buffer == NULL){
        return error_description(33, other_error, "-l");
    }
    

    //create temp file for saving end of outfile
    if(lseek(out, pos2, SEEK_SET) == -1)
        return error_description(33, other_error, "-l");
    int temp_file = open(".temp.swp", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if(temp_file == -1){
        return error_description(33, other_error, "-l");
    }

    int done = 0;
    while(!done){
        if((er = read(out, buffer, buf_size)) == -1)
            return error_description(33, other_error, "-l");

        if(er < buf_size)
            done = 1;

        if(write(temp_file, buffer, er) == -1)
            return error_description(33, other_error, "-l");
    }
    
    //copy from infile at position pos1 to outfile at position pos2
    if(lseek(in, pos1, SEEK_SET) == -1)
        return error_description(33, other_error, "-l");
    if(lseek(out, pos2, SEEK_SET) == -1)
        return error_description(33, other_error, "-l");

    done = 0;
    int sum = 0;
    while(!done){
        er = read(in, buffer, buf_size);
        if(er == -1)
            return error_description(33, other_error, "-l");
        
        if((sum += er) > num){
            er -= sum - num;
            done = 1;
        }

        if(write(out, buffer, er) == -1)
            return error_description(33, other_error, "-l");
    }

    //copy saved end of outfile back
    if(lseek(temp_file, 0, SEEK_SET) == -1)
        return error_description(33, other_error, "-l");
    done = 0;
    while(!done){
        if((er = read(temp_file, buffer, buf_size)) == -1)
            return error_description(33, other_error, "-l");

        if(er < buf_size)
            done = 1;

        if(write(out, buffer, er) == -1)
            return error_description(33, other_error, "-l");
    }

    //remove temp file
    if(unlink(".temp.swp") == -1)
        return error_description(33, other_error, "-l");

    er = close(temp_file);
    if(er == -1){
        return error_description(33, other_error, "-l");
    }

    free(buffer);

    return 0;
}

int parse_input(int* fd_in, int* fd_out, int x, int pos1,
        int* pos2, int num){
        
    if(pos1 < 0) return -1;
    
    struct stat file_stat;
    if(fstat(*fd_in, &file_stat) == -1)
        return -1;

    if(pos1 > file_stat.st_size)
        return -1;
    
    if(*pos2 < 0){
        int offset = 0;
        if((offset = lseek((*fd_out), (off_t)0, x)) == -1)
                return -1;

        if(*pos2 + offset < 0)
            return -1;
        else{
            (*pos2) += offset;

            if(lseek(*fd_out, 0, SEEK_SET) == -1)
                exit(error_description(33, other_error, "-l"));
        }
    }

    if(file_stat.st_size < pos1 + num)
        return file_stat.st_size - pos1;
    else
        return num;
}
