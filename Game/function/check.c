#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "check/checkElectricalPanel.c"

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

	if (argc < 3) {
		write(2, "\033[31mSelect an object to check.\n\033[37m", strlen("\033[31mSelect an object to check.\n\033[37m"));
		exit(1);
	}

	if (argc > 3) {
		write(2, "\033[31mToo much arguments. Usage: check <object>.\n\033[37m", strlen("\033[31mToo much arguments. Usage: check <object>.\n\033[37m"));
		exit(1);
	}

	// Add the extension the the object
	strncpy(objectWithExtension, argv[1], 30);
	strcat(objectWithExtension, ".obj");

	// Check if the object is in the current room
	if (access(objectWithExtension, R_OK) == -1) {
		printf("\033[31mThe object does not exist or is not in this room.\n\033[37m");
		exit(1);
	}

	// Load corresponding function to each object
	if (strcmp(objectWithExtension, "electrical-panel.obj") == 0) checkElectricalPanel();

	return 0;
}
