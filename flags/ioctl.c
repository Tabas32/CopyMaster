#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <errno.h>


void ioctl(int infile, char* argument){

	struct termios initialrsettings, newrsettings;

	tcgetattr(infile, &initialrsettings);	//ziskanie nastavenia 
	newrsettings = initialrsettings;		// duplikacia povodneho nastavenia
	
	if(strcmp(argument, "ECHO+") == 0){
		newrsettings.c_lflag |= ECHO; //zapne priznak ECHO
 		tcsetattr(infile, TCSAFLUSH, &newrsettings); //zabranime vypisovanie znakov na obrazovku
	}
	if(strcmp(argument, "ECHO-") == 0){
		newrsettings.c_lflag &= ~ECHO; //vypne priznak ECHO
 		tcsetattr(infile, TCSAFLUSH, &newrsettings); //zabranime vypisovanie znakov na obrazovku
	}
	if(strcmp(argument, "ECHOx") == 0){
		newrsettings.c_lflag ^= ECHO; //zneguje priznak ECHO
 		tcsetattr(infile, TCSAFLUSH, &newrsettings); //zabranime vypisovanie znakov na obrazovku
	}	
}
