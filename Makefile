all:
	cd flags;make all;
	gcc -std=c11 copymaster.c \
		flags/append.c flags/create.c flags/overwrite.c \
		flags/seek.c flags/directory.c flags/noFlags.c \
		flags/sparse.c flags/mod.c flags/inode.c flags/mask.c \
		flags/ioctl.c flags/lnk.c flags/env_vals.o \
		flags/env_vars.o errors.c  flags/trunc.o -lm -g -o copymaster

clean:
	cd flags;make clean;
	rm copymaster
