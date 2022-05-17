# Makefile

CC = gcc -DFUNCTION -rdynamic 
CC1 = gcc -rdynamic
CPPFLAGS = -MMD 
CFLAGS = -Wall -Wextra  -std=gnu99 -g 

SRC = myshell0.c  function/cd.c function/view.c function/inv.c function/Leave.c function/pickUp.c function/talk.c function/chmod.c function/newGame.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

SRC1 = function/view.c
OBJ1 = ${SRC1:.c=.o}
DEP1 = ${SRC1:.c=.d}

SRC2 = function/cd.c
OBJ2 = ${SRC2:.c=.o}
DEP2 = ${SRC2:.c=.d}

SRC3 = function/inv.c
OBJ3 = ${SRC3:.c=.o}
DEP3 = ${SRC3:.c=.d}

SRC4 = function/Leave.c
OBJ4 = ${SRC4:.c=.o}
DEP4 = ${SRC4:.c=.d}

SRC5 = function/pickUp.c
OBJ5 = ${SRC5:.c=.o}
DEP5 = ${SRC5:.c=.d}

SRC6 = function/talk.c
OBJ6 = ${SRC6:.c=.o}
DEP6 = ${SRC6:.c=.d}

SRC7 = function/pwd.c
OBJ7 = ${SRC7:.c=.o}
DEP7 = ${SRC7:.c=.d}

SRC8= function/chmod.c
OBJ8 = ${SRC8:.c=.o}
DEP8 = ${SRC8:.c=.d}

SRC9= function/use.c
OBJ9 = ${SRC9:.c=.o}
DEP9 = ${SRC9:.c=.d}

SRC10= ../Hold-up-Game.c
OBJ10 = ${SRC10:.c=.o}
DEP10 = ${SRC10:.c=.d}

all: view myshell0 cd inv leave pickUp talk pwd chmod use ../Hold-up-Game

-include ${DEP1}
view: ${SRC1}
	$(CC1) -o $@ $^ $(CFLAGS)

-include ${DEP}
myshell0: ${OBJ}
	$(CC) -o $@ $^ $(CFLAGS) -pthread -lpthread -fPIE
		
-include ${DEP2}
cd: ${SRC2}
	$(CC1) -o $@ $^ $(CFLAGS)

-include ${DEP3}
inv: ${SRC3}
	$(CC1) -o $@ $^ $(CFLAGS)
		
-include ${DEP4}		
leave: ${SRC4}
	$(CC1) -o $@ $^ $(CFLAGS)

-include ${DEP5}
pickUp: ${SRC5}
	$(CC1) -o $@ $^ $(CFLAGS)

-include ${DEP6}
talk: ${SRC6}
	$(CC1) -o $@ $^ $(CFLAGS)
	
-include ${DEP7}
pwd: ${SRC7}
	$(CC1) -o $@ $^ $(CFLAGS)
	
-include ${DEP8}
chmod: ${SRC8}
	$(CC1) -o $@ $^ $(CFLAGS)

-include ${DEP9}
use: ${SRC9}
	$(CC1) -o $@ $^ $(CFLAGS) -D_DEFAULT_SOURCE -D_BSD_SOURCE

-include ${DEP10}
Hold-up-Game: ${SRC10}
	$(CC1) -o $@ $^ $(CFLAGS)


clean:
	@rm -f ${OBJ} ${DEP} myshell0 ${DEP1} ${OBJ1} view  ${DEP2} ${OBJ2} cd ${DEP3} ${OBJ3} inv ${DEP4} ${OBJ4} Leave ${DEP5} ${OBJ5} pickUp ${DEP6} ${OBJ6} talk ${DEP7} ${OBJ7} pwd ${DEP8} ${OBJ8} chmod ${DEP9} ${OBJ9} use ${DEP10} ${OBJ10} ../Hold-up-Game
# END
