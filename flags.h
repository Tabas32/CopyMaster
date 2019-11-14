//Description:
//  It open outfile.
//
//Parameters:
//  out      - pointer to outfile file descriptor
//  outfile  - path to outfile file
//
//Return value:
//  noFlags() returns 21 if error appears 0 otherwise
int noFlag(int* out, const char* outfile);

//Description:
//  It open outfile for appending
//  File outfile must exists.
//
//Parameters:
//  out     - pointer to outfile file descriptor
//  outfile - path to outfile
//
//Return value:
//  append() returns 22 if error appears 0 otherwise
int append(int* out, const char* outfile);

//Description:
//  It creates outfile if possible.
//
//Parameters:
//  out      - pointer to outfile file descriptor
//  outfile  - path to outfile
//  argument - permissions to use in case a new file is created
//
//Return value:
//  create() returns 23 if error appears 0 otherwise
int create(int* out, const char* outfile, char* argument);

//Description:
//  It open outfile and overwrite it.
//
//Parameters:
//  out      - pointer to outifile file descriptor
//  outfile  - path to outfile file
//
//Return value:
//  overwrite() returns 24 if error appears 0 otherwise
int overwrite(int* out, const char* outfile);

//Description:
//  It prints content of directory infile to outfile from and to given position.
//
//Parameters:
//  infile   - file descriptor of infile
//  outfile  - file descriptor of outfile
//  argumets - string representing argument of option --lseek
//  buf_size - size of buffer for reading and writing
//
//Return value:
//  seek() returns 33 if error appears, 0 otherwise
int seek(int infile, int outfile, char* arguments, size_t buf_size);

//Description:
//  It change permissions of file.
//
//Parameters:
//  file   - path to file
//  argument - permissions to use
//
//Return value:
//  mod() returns -1 if error appears 0 otherwise
int mod(const char* file, char* argument);

//Description:
//  It check if file has same inode numer as numer passed in argument.
//
//Parameters:
//  file   - path to file
//  argument - char* with inode numea to compare
//
//Return value:
//  inode() returns 27 if error appears or inonde numbers are not the same, 0 otherwise
int inode(const char* file, char* argument);

//Description:
//  It set file mode creation mask.
//
//Parameters:
//  argument - permissions to remove or set unchanged
void mask(char* argument);

//Description:
//  It make sparse file from outfile if possible.
//
//Parameters:
//  outfile  - file descriptor of outfile
//  buf_size - size of buffer for reading and writing
//
//Return value:
//  sparse() returns 41 if error appears or sparse file was not created 
//  0 otherwise
int sparse(int outfile, size_t buf_size);

//Description:
//  It prints content of directory infile to outfile.
//  Infile must exitsts and it must be directory.
//
//Parameters:
//  infile   - path to directory
//  outfile  - file descriptor of open outfile
//  argumets - string representing argument of option --directory
//
//Return value:
//  directory() returns 28 if error appears, 0 otherwise
int directory(const char* infile, int outfile, char* arguments);

//Description:
//  It sets echo in terminal
//
//Parameters:
//  infile    - file descriptor of terminal file
//  argument  - argument for getopt
void ioctl(int infile, char* argument);

//Description:
//  It makes new hardlink to file infile
//  If outfile is nonexistent file, will be created
//
//Parameters:
//  infile   - path to infile file
//  outfile  - path to outfile file
//
//Return value:
//  lnk() returns 30 if error appears 0 otherwise
int lnk(const char* infile, const char* outfile);

//Description:
//  It print on standart output enviroment variables
//
//Parameters:
//  envp    - third argument to main()
void env_vars(char** envp);

//Description:
//  It print on standart output enviroment variables with value
//
//Parameters:
//  envp    - third argument to main()
void env_vals(char** envp);

//Description:
//  It sets size of file to size passed in argument(from getopt)
//
//Parameters:
//  file      - path to file
//  argument  - size in form of string
//
//Return value:
//  trnc() returns 31 if error appears 0 otherwise
int trnc(char* file, char* argument);
