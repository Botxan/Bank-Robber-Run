#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/syscall.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../defines.h"

int searchAmount(char *word , int file){
	char line[4] ;
	int i = 4; int e = 0;

	while (e == 0) {
		while (read(file, line, 4) > 0) {
			if(strcmp(line, word)){
				e = 1;
				return i;
			}
			memset(line,0,strlen(line));
   			i = i+4;
   		}
		return i;
	}
	return i;
}

/**
 * Function: pickup
 * ----------------
 * Takes a .tool file from the current room and stores in players inventory (/Inv)
 * @param argc number of arguments
 * @param argv arguments for the program
 * 	- 0: pickup command
	- 1: target .tool file name
	- 2: root directory
 */

int main(int argc,char* argv[]) {
	char current[PATH_MAX];
	char invPath[PATH_MAX];
	char pathToTool[PATH_MAX];
        struct stat file;

	if (argc != 3) {
		write(2, "Usage: pickup <tool>\n", strlen("Usage: pickup <tool>\n"));
		return 1;
	}

	strcpy(current, getcwd(current, 200));
	strcat(current, "/");
	strcat(current, argv[1]);
	strcat(current, ".tool");
	// Check if file exists in the current room
	if (stat(current, &file) == -1) {
		write(2, "There is no such tool to pick-up in the room.\n", strlen("There is no such tool to pick-up in the room.\n"));
		return 1;
	}

	strcpy(invPath, argv[2]);
	strcat(invPath, "/Inv/");
	strcat(invPath, argv[1]);
	strcat(invPath, ".tool");

	// Get the value of the symbolic link to the real tool
	readlink(current, pathToTool, sizeof(pathToTool));

	// Create a symlink in /Inv (~ move the tool to inv)
	if (symlink(pathToTool, invPath) == -1) {
		write(2, "Could not add the tool to the inventory: ", strlen("Could not add the tool to the inventory: "));
		write(2, strerror(errno), strlen(strerror(errno)));
		write(2, ".\n", 2);
	}

	// Remove the tool from the environment
	unlink(current);
	return 0;
}
