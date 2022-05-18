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

#include "talk.h"

/**
 * Function: searchTalk
 * --------------------
 * Returns the byte offset of the provided dialogue
 * @param word the code to be searched in the file (e.g 2C)
 * @param file the file where to search
 * @return 0 => npc keeps talking. 1 => stop conversation. 2 => game over
 */
int searchTalk(char *word, int file){
	char c;
   	int readed;

	while ((readed = read(file, &c, 1)) == 1) {
		if (c == word[0]) {
			read(file, &c, 1);
			if (c == word[1]) return lseek(file, 0, SEEK_CUR);
		}
	}

   	return -1;
}

#ifdef FUNCTION
int talk(char* npc){
#else
int main(int argc,char* argv[]) {
	if (argc<1&&strcmp(argv[0],"talk")!=0) return -1;
	char *npc=argv[1];
#endif
	int i = 0;
	int end = 0;
	int lastDialog;
	int lastValidDialog;
	int fd;
	char state[2] = " #";
	char returnV;
	char var = 'A';
	char continues = 'y';
	char text[4096];
	char branch[2];
	char *current;

	// Get npc path
	current = getcwd(NULL, 0);
	strcat(current, "/");
	strcat(current, npc);
	strcat(current, ".npc");

	// Check if npc is the current room
	//if (stat(current, &file) == -1) return -1;
	if (access(current, W_OK) == -1) {
		write(2, "\033[31mThere is no one by that name in this room.\n\033[37m", strlen("\033[31mThere is no one by that name in this room.\n\033[37m"));
		return -1;
	}

	fd = open(current, O_RDWR);
	if (fd == -1) return 1;

	// Read initial state of the npc
	read(fd, &state[0], 1);

	// Seek the byte where the dialogue block starts, right after
	// x# <--
	lastValidDialog = searchTalk(state, fd);

	while (continues == 'y') {
            	branch[0] = i + '0';
		branch[1] = var;

		lastDialog = searchTalk(branch, fd);

		// Valid dialog
		if (lastDialog == -1) {
			// return to previous offset until valid option is provided
                        lseek(fd, lastValidDialog, SEEK_SET);
                        write(1, "Select a valid option.\n", strlen("Select a valid option.\n"));
		} else {
			lastValidDialog = lastDialog;
			end = searchTalk("--", fd);
			write(1, npc, strlen(npc));
	       	        write(1, ":\n", 2);
			read(fd, &continues, 1);
	                lseek(fd, lastDialog-end, SEEK_CUR);
	                read(fd, &text, end-lastDialog-3);
	                write(0, text, end-lastDialog-3);
	                text[0] = '\0';
			i++;
 		}

		// Display when answer is needed (or when incorrect answer/option has been introduced by the user)
		if (continues == 'y') {
			write(1, "\n----- Choose one -----\n", strlen("\n----- Choose one -----\n"));
			scanf(" %[^\n]%*c", &var); // this removes continues value
			write(1, "\n", 1);
			continues = 'y';
		}
	}

	lseek(fd, 3, SEEK_CUR);
	read(fd, &returnV, 1);

	// Write the new initial state
	lseek(fd, 0, SEEK_SET);
	write(fd, &continues, 1);
	close(fd);

	write(0, "\n", 1);

	return returnV + 0;
}
