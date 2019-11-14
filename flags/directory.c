#define _XOPEN_SOURCE 500
#define _BSD_SOURCE

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <errno.h>

#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "../errors.h"

int readDir_recursion(const char* directory, int outfile, int subLoc, void (*write_fcion)(int, char*));
int readDir(const char* directory, int outfile, void (*write_fcion)(int, char*), int*, char);
void basicWrite(int, char*);
void linuxWrite(int, char*);
void windowsWrite(int, char*);

int directory(const char* infile, int outfile, char* arguments){
    enum{ AR_N = 0, AR_L,
        AR_D,
        AR_R,
        AR_T,
        AR_S
    };
    
    char *const tokens[] = {
        [AR_N] = "n", 
        [AR_L] = "l",
        [AR_D] = "d",
        [AR_R] = "R",
        [AR_T] = "t",
        [AR_S] = "s", 
        NULL
    };

    char* value;

    int arg1[] = {0, 0, 0};
    int recursive = 0;
    int* size = NULL;
    char type = '\0';

    while(*arguments != '\0'){
        switch(getsubopt(&arguments, tokens, &value)){
            case AR_N:
                arg1[0] = 1;
                break;

            case AR_L:
                arg1[1] = 1;
                break;

            case AR_D:
                arg1[2] = 1;
                break;

            case AR_R:
                recursive = 1;
                break;

            case AR_T:
                if(value == NULL){
                    options_error("-D");
                    return 20;
                }
                type = *value;
                break;

            case AR_S:
                if(value == NULL){
                    options_error("-D");
                    return 20;
                }
                size = (int*)calloc(1, sizeof(int));
                sscanf(value, "%d", size);
                break;

            default:
                options_error("-D");
                return 20;
                break;
        }
    }

    if(arg1[0] + arg1[1] + arg1[2] != 1){ 
            options_error("-D");
            return 20;
    }
    
    if(arg1[0]){
        if(recursive){
            if(size) return 0;
            if(type == 'f') return 0;
            return readDir_recursion(infile, outfile, 0, basicWrite);
        }
        else
            return readDir(infile, outfile, basicWrite, size, type);
    }
    else if(arg1[1]){
        if(recursive){
            if(size) return 0;
            if(type == 'f') return 0;
            return readDir_recursion(infile, outfile, 0, linuxWrite);
        }
        else
            return readDir(infile, outfile, linuxWrite, size, type); 
    }
    else if(arg1[2]){
        if(recursive){
            if(size) return 0;
            if(type == 'f') return 0;
            return readDir_recursion(infile, outfile, 0, windowsWrite);
        }
        else
            return readDir(infile, outfile, windowsWrite, size, type);
    }

    return 0;
}

int readDir(const char* directory, int outfile, void (*write_fcion)(int, char*), int* size, char type){
    DIR* dir = opendir(directory);
    if(errno != 0){
        switch(errno){
            case ENOTDIR:
                return error_description(28, notDir_error, "-D");
                break;
            case ENOENT:
                return error_description(28, file_missing_error, "-D");
                break;
            default:
                return error_description(28, other_error, "-D");
        }
    }

    struct dirent *pDirent;
    while((pDirent = readdir(dir)) != NULL){
        struct stat st;

        if(lstat(pDirent->d_name, &st) < 0){
            return error_description(28, other_error, "-D");
        }

        if(size){
            if(st.st_size >= *size && S_ISREG(st.st_mode)){
                (*write_fcion)(outfile, pDirent->d_name);
            }
        }
        else if(type != '\0'){
            if((type == 'f' && S_ISREG(st.st_mode)) || (type == 'd' && S_ISDIR(st.st_mode))){
                (*write_fcion)(outfile, pDirent->d_name);
            }
        }
        else
            (*write_fcion)(outfile, pDirent->d_name);
    }

    if(closedir(dir) == -1){
        return error_description(28, other_error, "-D");
    }
}

void basicWrite(int outfile, char* name){
    char* nname = name;
    nname = strrchr(name, '/') ? : name; 
    write(outfile, nname, strlen(nname));
    write(outfile, "\n", 1);
}

void linuxWrite(int outfile, char* name){
    char str[11], *s = str;
    struct stat st;
    mode_t owner, group, other;

    if(lstat(name, &st) < 0){
        error_description(28, other_error, "-D");
        return;
    }

    if(S_ISREG(st.st_mode))
        *s++ = '-';
    else if(S_ISDIR(st.st_mode))
        *s++ = 'd';
    else if(S_ISCHR(st.st_mode))
        *s++ = 'c';
    else if(S_ISBLK(st.st_mode))
        *s++ = 'b';
    else if(S_ISFIFO(st.st_mode))
        *s++ = 'f';
    else if(S_ISLNK(st.st_mode))
        *s++ = 'l';
    else
        *s++ = 's';

    owner = st.st_mode & S_IRWXU;
    group = st.st_mode & S_IRWXG;
    other = st.st_mode & S_IRWXO;

    *s++ = owner & S_IRUSR ? 'r' : '-';
    *s++ = owner & S_IWUSR ? 'w' : '-';
    *s++ = owner & S_IXUSR ? 'x' : '-';

    *s++ = group & S_IRGRP ? 'r' : '-';
    *s++ = group & S_IWGRP ? 'w' : '-';
    *s++ = group & S_IXGRP ? 'x' : '-';

    *s++ = other & S_IROTH ? 'r' : '-';
    *s++ = other & S_IWOTH ? 'w' : '-';
    *s++ = other & S_IXOTH ? 'x' : '-';

    *s = '\0';
    
    char string[512];
    s = string;
    s += sprintf(s, "%s", str);
    s += sprintf(s, "\t%3lu", st.st_nlink);
    
    struct passwd* pwd;
    pwd = getpwuid(st.st_uid);
    s += sprintf(s, "\t%s", pwd->pw_name);

    struct group* grp;
    grp = getgrgid(st.st_gid);
    s += sprintf(s, "\t%s", grp->gr_name);

    s += sprintf(s, "\t%12lu", st.st_size);

    struct tm* timeinfo;
    timeinfo = localtime(&(st.st_mtime));
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeinfo);
    s += sprintf(s, "\t%s", buffer);

    char* nname = name;
    nname = strrchr(name, '/') ? : name;
    sprintf(s, "\t%s\n", nname);

    if(write(outfile, string, strlen(string)) == -1){
        error_description(28, other_error, "-D");
        return;
    }

}

void windowsWrite(int outfile, char* name){
    char string[512], *s = string;
    struct stat st;

    if(lstat(name, &st) < 0){
        error_description(28, other_error, "-D");
        return;
    }

    struct tm* timeinfo;
    timeinfo = localtime(&(st.st_mtime));
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", timeinfo);

    s += sprintf(s, "%s", buffer);

    if(S_ISDIR(st.st_mode)){
        s += sprintf(s, "\t<DIR>");
        s += sprintf(s, "\t            ");
    }
    else{
        s += sprintf(s, "\t     ");
        s += sprintf(s, "\t%12lu", st.st_size);
    }

    char* nname = name;
    nname = strrchr(name, '/') ? : name;
    sprintf(s, "\t%s\n", nname);
    
    if(write(outfile, string, strlen(string)) == -1){
        error_description(28, other_error, "-D");
        return;
    }
}

int readDir_recursion(const char* directory, int outfile, int subLoc, void (*write_fcion)(int, char*)){
    DIR* dir = opendir(directory);
    if(errno != 0){
        switch(errno){
            case ENOTDIR:
                return error_description(28, notDir_error, "-D");
                break;
            case ENOENT:
                return error_description(28, file_missing_error, "-D");
                break;
            default:
                return error_description(28, other_error, "-D");
        }
    }


    char tabs[128], *s = tabs;
    for(int i = 0; i < subLoc; i++){
        s += sprintf(s, "\t"); 
    }

    struct dirent *pDirent;
    while((pDirent = readdir(dir)) != NULL){
        //remember sub directories
        if(pDirent->d_type == DT_DIR){
            if(strcmp(pDirent->d_name, "..") && strcmp(pDirent->d_name, ".")){
                char* name = (char*)calloc(strlen(directory) + strlen(pDirent->d_name) + 2, sizeof(char));
                sprintf(name, "%s/%s", directory, pDirent->d_name);

                if(write(outfile, tabs, strlen(tabs)) == -1)
                    return error_description(28, other_error, "-D");

                (*write_fcion)(outfile, name);
                readDir_recursion(name, outfile, subLoc+1, (*write_fcion));
                free(name);
            }
        } 
    }
    if(pDirent == NULL && errno != 0){
        other_error("-D");
        return 28;
    }

    if(closedir(dir) == -1){
        return error_description(28, other_error, "-D");
    }
}
