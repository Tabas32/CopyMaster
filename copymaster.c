#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

#include "flags.h"
#include "errors.h"

#define APPEND 0
#define CREATE 1
#define OVERWRITE 2
#define FAST 3
#define LSEEK 4
#define DELETE 5
#define SLOW 6
#define CHMOD 7
#define INODE 8
#define UMASK 9
#define DIRECTORY 10
#define IOCTL 11
#define ENV_VARS 12
#define ENV_VALS 13
#define LINK 14
#define TRUNCATE 15
#define SPARSE 16

//reutrn 1 if passed options collide together, 0 otherwise
int checkCollisions(int[]);

//copy data from open infile to outfile
int copy(int infile, int outfile, int buf_size);

int main(int argc, char* argv[], char** envp){
    int flags[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char* arguments[17];
    for(int i = 0; i < 17; i++){
        arguments[i] = NULL;
    }

    if (argc > 2){
        int opt, option_index = 0;

        static struct option long_options[] = {
            {"append", no_argument, NULL, 'a'},
            {"create", required_argument, NULL, 'c'},
            {"overwrite", no_argument, NULL, 'o'},
            {"fast", no_argument, NULL, 'f'},
            {"lseek", required_argument, NULL, 'l'},
            {"delete", no_argument, NULL, 'd'},
            {"slow", no_argument, NULL, 's'},
            {"chmod", required_argument, NULL, 'm'},
            {"inode", required_argument, NULL, 'i'},
            {"umask", required_argument, NULL, 'u'},
            {"directory", required_argument, NULL, 'D'},
            {"ioctl", required_argument, NULL, 'I'},
            {"env-vars", no_argument, NULL, 'e'},
            {"env-vals", no_argument, NULL, 'E'},
            {"link", no_argument, NULL, 'K'},
            {"truncate", required_argument, NULL, 't'},
            {"sparse", no_argument, NULL, 'S'},
            {0, 0, 0, 0}
        };

        while(opt != -1){
            opt = getopt_long(argc, argv, ":ac:ofl:dsm:i:u:D:I:eEKt:S",
                        long_options, &option_index);

            switch(opt){
                case 'a':
                    flags[APPEND] = 1;
                    break;

                case 'c':
                    flags[CREATE] = 1;

                    arguments[CREATE] = (char*)calloc(strlen(optarg) + 1, sizeof(char));
                    strcpy(arguments[CREATE], optarg);
                    
                    break;

                case 'o':
                    flags[OVERWRITE] = 1;
                    break;

                case 'f':
                    flags[FAST] = 1;
                    break;

                case 'l':
                    flags[LSEEK] = 1;
                    if(optarg[0] == '-'){
                        options_error(argv[optind -2]);
                        return 20;
                    }

                    arguments[LSEEK] = (char*)calloc(strlen(optarg) + 1, sizeof(char));
                    strcpy(arguments[LSEEK], optarg);

                    break;

                case 'd':
                    flags[DELETE] = 1;
                    break;

                case 's':
                    flags[SLOW] = 1;
                    break;

                case 'm':
                    flags[CHMOD] = 1;

                    arguments[CHMOD] = (char*)calloc(strlen(optarg) + 1, sizeof(char));
                    strcpy(arguments[CHMOD], optarg);

                    break;

                case 'i':
                    flags[INODE] = 1;
                    if(optarg[0] == '-'){
                        options_error(argv[optind -2]);
                        return 20;
                    }

                    arguments[INODE] = (char*)calloc(strlen(optarg) + 1, sizeof(char));
                    strcpy(arguments[INODE], optarg);
                    
                    break;

                case 'u':
                    flags[UMASK] = 1;
                    if(optarg[0] == '-'){
                        options_error(argv[optind -2]);
                        return 20;
                    }

                    arguments[UMASK] = (char*)calloc(strlen(optarg) + 1, sizeof(char));
                    strcpy(arguments[UMASK], optarg);
                    
                    break;

                case 'D':
                    flags[DIRECTORY] = 1;
                    if(optarg[0] == '-'){
                        options_error(argv[optind -2]);
                        return 20;
                    }
                    
                    arguments[DIRECTORY] = (char*)calloc(strlen(optarg) + 1, sizeof(char));
                    strcpy(arguments[DIRECTORY], optarg);
                    break;

                case 'I':
                    flags[IOCTL] = 1;
                    if(optarg[0] == '-'){
                        options_error(argv[optind -2]);
                        return 20;
                    }
                    
                    arguments[IOCTL] = (char*)calloc(strlen(optarg) + 1, sizeof(char));
                    strcpy(arguments[IOCTL], optarg);
                    break;

                case 'e':
                    flags[ENV_VARS] = 1;
                    break;

                case 'E':
                    flags[ENV_VALS] = 1;
                    break;

                case 'K':
                    flags[LINK] = 1;
                    break;

                case 't':
                    flags[TRUNCATE] = 1;
                    if(optarg[0] == '-'){
                        options_error(argv[optind -2]);
                        return 20;
                    }
                   
                    arguments[TRUNCATE] = (char*)calloc(strlen(optarg) + 1, sizeof(char));
                    strcpy(arguments[TRUNCATE], optarg);
                    break;

                case 'S':
                    flags[SPARSE] = 1;
                    break;

                case '?':
                    options_error(argv[optind -2]);
                    return 20;
                    break;
                
                case ':':
                    options_error(argv[optind -2]);
                    return 20;
                    break;
            }
        }

        if(checkCollisions(flags)){
            options_error("Options colision");
            return 20; 
        }

        int ret_val = 0;

        size_t buf_size = 100;
        if(flags[FAST]) buf_size = 4096;
        if(flags[SLOW]) buf_size = 1;

        if(flags[INODE]){
            if(inode(argv[argc - 2], arguments[INODE]) == 27)
                return 27;
        }

        if(flags[ENV_VARS])
            env_vars(envp);

        if(flags[ENV_VALS])
            env_vals(envp);

        if(flags[UMASK]){
            mask(arguments[UMASK]);
        }
        
        //open infile
        int in;
        if(!flags[DIRECTORY] && !flags[LINK]){
            in  = open(argv[argc - 2], O_RDONLY);
            if(in == -1){
                return error_description(-1, file_missing_error, "-"); 
            }
        }


        //open outfile
        int out;
        if(flags[APPEND]){
            ret_val = append(&out, argv[argc - 1]);
        }
        else if(flags[CREATE]){
            ret_val = create(&out, argv[argc - 1], arguments[CREATE]);
        }
        else if(flags[OVERWRITE]){
            ret_val = overwrite(&out, argv[argc - 1]);
        }
        else if(!flags[LINK]){
            ret_val = noFlag(&out, argv[argc - 1]);
        }

        //ioctl
        if(flags[IOCTL])
            ioctl(in, arguments[IOCTL]);

        //copy options
        if(!ret_val && flags[LSEEK]){
            ret_val = seek(in, out, arguments[LSEEK], buf_size);
        }

        if(!ret_val && flags[DIRECTORY]){
            ret_val = directory(argv[argc - 2], out, arguments[DIRECTORY]);
        }

        if(flags[LSEEK] + flags[DIRECTORY] == 0 && !flags[LINK]){
            ret_val = copy(in ,out, buf_size);
        }

        if(!ret_val && flags[SPARSE]){
            ret_val = sparse(out, buf_size);
        }

        //close infile and outfile
        if(!flags[DIRECTORY] && !flags[LINK]){
            if(close(in) == -1){
                return error_description(-1, other_error, "-");
            }
        }
        if(!flags[LINK])
            if(close(out) == -1){
                return error_description(-1, other_error, "-");
            }
        

        if(!ret_val && flags[LINK])
            ret_val = lnk(argv[argc - 2], argv[argc - 1]);

        if(!ret_val && flags[CHMOD]){
            ret_val = mod(argv[argc - 1], arguments[CHMOD]);
        }

        if(!ret_val && flags[TRUNCATE]){
            ret_val = trnc(argv[argc - 2], arguments[TRUNCATE]);
        }

        if(!ret_val && flags[DELETE]){
            if(unlink(argv[argc - 2]))
                ret_val = error_description(26, notdeleted_error, "-d");
        }


        for(int i = 0; i < 17; i++){
            if(arguments[i] != NULL)
                free(arguments[i]);
        }
        return ret_val;
    }
    else
        printf("Wrong num. of arguments\n");
}

int checkCollisions(int flags[]){
    if(flags[APPEND] && (flags[CREATE] + flags[OVERWRITE] + flags[LSEEK] + flags[LINK]))
        return 1;
    if(flags[CREATE] && (flags[APPEND] + flags[OVERWRITE] + flags[LINK]))
        return 1;
    if(flags[OVERWRITE] && (flags[APPEND] + flags[CREATE] + flags[LINK]))
        return 1;
    if(flags[FAST] && flags[SLOW])
        return 1;
    if(flags[LSEEK] && (flags[APPEND] + flags[DIRECTORY] + flags[IOCTL] + flags[LINK]))
        return 1;
    if(flags[DELETE] && (flags[DIRECTORY] + flags[IOCTL] + flags[TRUNCATE]))
        return 1;
    if(flags[DIRECTORY] && (flags[LSEEK] + flags[DELETE] + flags[IOCTL] + flags[LINK] + flags[TRUNCATE]))
        return 1;
    if(flags[IOCTL] && (flags[LSEEK] + flags[DELETE] + flags[DIRECTORY] + flags[LINK] + flags[TRUNCATE]))
        return 1;
    if(flags[LINK] && (flags[APPEND] + flags[CREATE] + flags[OVERWRITE] + flags[LSEEK] + flags[DIRECTORY] + flags[IOCTL] + flags[SPARSE]))
        return 1;
    if(flags[TRUNCATE] && (flags[DIRECTORY] + flags[IOCTL] + flags[DELETE]))
        return 1;
    if(flags[SPARSE] && flags[LINK])
        return 1;
    if(flags[ENV_VARS] && flags[ENV_VALS])
        return 1;
}

int copy(int in, int out, int buf_size){
    char* buffer = calloc(buf_size, sizeof(char));
    if(buffer == NULL){
        return error_description(22, other_error, "-");
    }
    
    
    int er;
    while((er = read(in, buffer, buf_size)) > 0){
        if(er == -1){
            return error_description(22, other_error, "-");
        }

        er = write(out, buffer, er);
        if(er == -1){
            return error_description(22, other_error, "-");
        }
    }

    free(buffer);
}
