#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "./check/checkElectricalPanel.c"
#include "./check/checkDrawers.c"
#include "./check/checkComputer.c"
#include "./check/checkCabinet.c"

/**
 * Function: main (check)
 * ---------------
 * This function will is used when the player interacts with .obj around
 * the scenario. The function will execute the corresponding routine
 * inside .function/check directory.
 * @param argv[0] the command check
 * @param argv[1] the object to be checked
 * @param argv[2] the root directory of the game
 */
int main(int argc, char *argv[]) {
	char objectWithExtension[strnlen(argv[1], 30)];
	char root[PATH_MAX];
	int fd;
	FILE *f;
	int checkedTimes = -1;
	char checkedTimesText[12];

	if (argc < 3) {
		write(2, "\033[31mSelect an object to check.\n\033[37m", strlen("\033[31mSelect an object to check.\n\033[37m"));
		exit(1);
	}

	if (argc > 3) {
		write(2, "\033[31mToo much arguments. Usage: check <object>.\n\033[37m", strlen("\033[31mToo much arguments. Usage: check <object>.\n\033[37m"));
		exit(1);
	}

	strncpy(root, argv[2], PATH_MAX);

	// Add the extension the the object
	strncpy(objectWithExtension, argv[1], 30);
	strcat(objectWithExtension, ".obj");

	// Check if the object is in the current room
	if (access(objectWithExtension, R_OK) == -1) {
		printf("\033[31mThe object does not exist or is not in this room.\n\033[37m");
		exit(1);
	}

	// Add +1 to checked times
	fd = open(objectWithExtension, O_RDWR);
        f = fdopen(fd, "r");
	fscanf(f, "%d", &checkedTimes);
        lseek(fd, 0, SEEK_SET);
        checkedTimes++;
        sprintf(checkedTimesText, "%d", checkedTimes);
        write(fd, checkedTimesText, strlen(checkedTimesText));
        close(fd);
        fclose(f);


	// Load corresponding function to each object

	// Interact with electrical panel room
	if (strcmp(objectWithExtension, "electrical-panel.obj") == 0) return checkElectricalPanel();

	// ------------- OFFICE #1 --------------

	// Check office 1 drawes and obtain laxatives
	if (strcmp(objectWithExtension, "drawers.obj") == 0) return checkDrawers(root, checkedTimes);

	// Check office 1 drawers
        if (strcmp(objectWithExtension, "desk.obj") == 0) printf("A pile of accounting papers, filing cabinets and the computer, turned on.\n");

	// Check office 1 computer => check emails
        if (strcmp(objectWithExtension, "computer.obj") == 0) return checkComputer();

	// Check office 1 cabinet => find employee suit
        if (strcmp(objectWithExtension, "cabinet.obj") == 0) return checkCabinet();


	return 0;
}
