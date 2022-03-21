# Makefile

CC=gcc -rdynamic 
CPPFLAGS= -MMD
CFLAGS= -Wall -Werror -Wextra -std=c99 -g

SRC= myshell0.c  function/view.c function/cd.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: myshell0 

-include ${DEP}

myshell0 : ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} myshell0

# END
