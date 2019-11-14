#include <stdlib.h>
#include <stdio.h>

void env_vals(char* env[]){
 	 for(int i = 0; env[i] != NULL; i++){
 	    printf("%s\n",env[i]);
     }
}
