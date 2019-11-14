all: append.o create.o overwrite.o sparse.o directory.o noFlags.o seek.o\
	 mod.o inode.o mask.o ioctl.o lnk.o env_vals.o env_vars.o trunc.o
	

append.o: append.c
	gcc -std=c11 -c append.c -o append.o

create.o: create.c
	gcc -std=c11 -c create.c -lm -o create.o

overwrite.o: overwrite.c
	gcc -std=c11 -c overwrite.c -o overwrite.o

sparse.o: sparse.c
	gcc -std=c11 -c sparse.c -o sparse.o

directory.o: directory.c
	gcc -std=c11 -c directory.c -o directory.o

noFlags.o: noFlags.c
	gcc -std=c11 -c noFlags.c -o noFlags.o

seek.o: seek.c
	gcc -std=c11 -c seek.c -o seek.o

mod.o: mod.c
	gcc -std=c11 -c mod.c -o mod.o

inode.o: inode.c
	gcc -std=c11 -c inode.c -o inode.o

mask.o : mask.c
	gcc -std=c11 -c mask.c -o mask.o

ioctl.o: ioctl.c
	gcc -std=c11 -c ioctl.c -o ioctl.o

lnk.o: lnk.c
	gcc -std=c11 -c lnk.c -o lnk.o

env_vals.o: env-vals.c
	gcc -std=c11 -c env-vals.c -o env_vals.o

env_vars.o: env-vars.c
	gcc -std=c11 -c env-vars.c -o env_vars.o

trunc.o: trunc.c
	gcc -std=c11 -c trunc.c -o trunc.o

clean:
	rm *.o
