# Makefile

CC=gcc -fsanitize=address
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra -std=c99 -O0 -g
LDFLAGS=
LDLIBS=

SRC= myshell0.c  function/view.c 
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: myshell0 

-include ${DEP}

myshell0 : ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} main

# END
