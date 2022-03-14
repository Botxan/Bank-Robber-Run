# Makefile

CC=gcc -fsanitize=address
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= 
LDFLAGS=
LDLIBS=

SRC= myshell0.c stee.c 
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: myshell0

-include ${DEP}

main: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} main

# END
