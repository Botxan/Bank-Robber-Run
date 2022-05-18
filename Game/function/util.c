#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * Returns the state of an object.
 * Note: this function is called by functions executed by check function,
 * so it assumes that the object is in the current room.
 */
int getObjStateInRoom(char *name) {
	int fd;
	char object[strlen(name) + 5];
	char state[1];

	sprintf(object, "%s.obj", name);

	fd = open(object, O_RDONLY);
	if (fd == -1) {
		write(2, "\033[31mCould not obtain the get of the object. Error opening the file.\n\033[37m", strlen("\033[31mCoud not get the state of the object. Error opening the file.\n\033[37m"));
		exit(1);
	}

	read(fd, state, 1);

	return state[0] - '0';
}

/**
 * Changes the state of an object in the current room.
 * Note: this function is called by functions executed by check function,
 * so it assumes that the object is in the current room.
 */
void setObjStateInRoom(char *name, int newState) {
	int fd;
	char object[strlen(name) + 5];
	char state[10] = " "; // conversion to write

	sprintf(object, "%s.obj", name);

	fd = open(object, O_WRONLY);
        if (fd == -1) {
                write(2, "\033[31mCould not set the state of the object. Error opening the file.\n\033[37m", strlen("\033[31mCoud not set the state of the object. Error opening the file.\n\033[37m"));
                exit(1);
        }

	sprintf(state, "%d", newState);
	int a = write(fd, state, 1);
	printf("bytes written: %d\n", a);
}
