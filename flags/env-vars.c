#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void env_vars(char **envp){
    while(*envp != NULL){
        char *len = strchr(*envp, '=');

        if(len == NULL)
            printf("%s\n", *envp);
        else
            printf("%.*s\n", (int)(len - *envp), *envp);

        envp++;
    }
}
