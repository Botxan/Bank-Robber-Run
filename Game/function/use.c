#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <libgen.h>
#include "../defines.h"

/*
 * Function: use
 * -------------
 * Uses a given tool in the environment or in a target
 * passed by parameter.
 * @param argc (in) number of arguments
 * @param argv the arguments for the program:
 *	- (in) Starting point of the game
	- (in) command to be executed (use)
	- (in) Item to use
        - (in) (optional) target
 */
int main(int argc, char *argv[]){

	// Print args
<<<<<<< HEAD
        //for (int i = 0; i < argc; i++)
        //        printf("%d: %s\n", i, argv[i]);

	if (argc < 3) {
		write(2, "Select an item to use.\n", strlen("Select an item to use.\n"));
		return 1;
	}

	if (argc > 4) {
		write(2, "Too much arguments.\n", strlen("Too much arguments.\n"));
=======
	// for (int i = 0; i < argc; i++)
	//	printf("%d: %s\n", i, argv[i]);
	switch(argc) {
	}
	if (argc < 4) {
		write(1, "\033[31mSelect an item to use.\n\033[37m", strlen("\033[31mSelect an item to use.\n\033[37m"));
		return 1;
	}

	if (argc > 5) {
		write(1, "\033[31mToo much arguments.\n\033[37m", strlen("\033[31mToo much arguments.\n\033[37m"));
>>>>>>> 8f53178a0c98d03465e88d24439767ab6b6f6d39
		return 1;
	}

	char tool[strlen(argv[2]) + 6]; // tool + .tool
        char invPath[strlen(argv[0]) + 6]; // root + "/Inv/ + "\0"
        char itemPath[strlen(argv[0]) + 30];
	char cwd[PATH_MAX] = "";
	char * cwdBase = NULL;
        DIR *d;
        struct dirent *dir;
        struct stat stat_buf;
        int targetType = -1; // -1 => not found | 0 => directory | 1 => npc | 2 => object
        int fd;
        int ret;
        char buf[20];
        int i = 0;
        char keyDoorPath[strlen(argv[0]) + 30];
        keyDoor keyDoor[KEY_DOORS_SIZE];
        int keyDoorI = 0;
        int readItem = 0;
        int found = 0;
        char commandPath[PATH_MAX];
        char roomPath[PATH_MAX];
	char rootPath[PATH_MAX];
	char obj[30];

        // Put extension to the tool
        strncpy(tool, argv[2], 20);
        strcat(tool, ".tool");

	// Check if item is in inventory
	strncpy(invPath, argv[0], sizeof(invPath));
	strncat(invPath, "/Inv/", sizeof(invPath));

	strncpy(itemPath, invPath, sizeof(itemPath));
	strncat(itemPath, tool, sizeof(itemPath));

	if (access(itemPath, F_OK) == -1) {
		write(1, "\033[31mYou don't have that object in the inventory.\n\033[37m", strlen("\033[31mYou don't have that object in the inventory.\n\033[37m"));
		return 1;
	}

	// non-targeted use command
<<<<<<< HEAD
	if (argc == 3) {

		// Use radio to communicate with you teammate
		if (strcmp(argv[2], "radio") == 0) {
			// Get current room
			if (getcwd(cwd, sizeof(cwd)) == NULL) {
				perror("getcwd() failed");
				return 1;
			}
			cwdBase = basename(cwd);

			// Check radio is safe for use (current room = electrical panel, wc or janitors room
			if ((strcmp(cwd, "Van") == 0) || (strcmp(cwdBase, "ElectricalPanelRoom") == 0) || (strcmp(cwdBase, "WC") == 0) || (strcmp(cwdBase, "JanitorRoom") == 0)) {

				if (fork() == 0) {
					// Get the root path of the project
	                                strncpy(rootPath, argv[0], PATH_MAX);
	                                strcat(rootPath, "/..");

        	                        strncpy(commandPath, argv[0], PATH_MAX);

                	                strcat(commandPath, "/../talk");
	                                execlp(commandPath, "talk", "Robert", rootPath, NULL);
	                                if (errno != 0) {
        	                                write(2, "Could not talk with your gang: ", strlen("Could not talk with your gang"));
	                                        write(2, strerror(errno), strlen(strerror(errno)));
	                                        write(2, ".\n", 2);
        	                                return 1;
	                                }
				} else wait(NULL);

			} else write(1, "*This is not a safe place to use the radio*\n", strlen("*This is not a safe place to use the radio*\n"));
                } else write(1, "Object used in the room. Nothing happened.\n.", strlen("Object used in the room. Nothing happened.\n"));

=======
	if (argc == 4) {
		write(1, "\033[31mObject used in the room. Nothing happened.\n.\033[37m", strlen("\033[31mObject used in the room. Nothing happened.\n\033[37m"));
>>>>>>> 8f53178a0c98d03465e88d24439767ab6b6f6d39
		return 0;
	}

	// Get file type
	d = opendir(".");
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			// Check if target is found

			// If it is a directory
			if (strcmp(dir->d_name, argv[3]) == 0) {
				if (stat(dir->d_name, &stat_buf) != 0) {
					write(1, "\033[31mError on stat system call.\n\033[37m", strlen("\033[31mError on stat system call.\n\033[37m"));
					return 1;
				};

				targetType = dir->d_type;
				break;
			}

			// If it is an object
			sprintf(obj, "%s.obj", argv[3]);
			if (strcmp(obj, dir->d_name) == 0) {
                                if (stat(dir->d_name, &stat_buf) != 0) {
                                        write(1, "Error on stat system call.\n", strlen("Error on stat system call.\n"));
                                        return 1;
                                };

                                targetType = dir->d_type;
                                break;
                        }


		}
	}
	closedir(d);

	switch(targetType) {
		case DT_DIR: // directory
			// Read the key_door file to obtain key->door list
			strncpy(keyDoorPath, argv[0], sizeof(keyDoorPath));
			strncat(keyDoorPath, "/../assets/key_door.txt", sizeof(keyDoorPath));
			fd = open(keyDoorPath, O_RDONLY);

			if (fd == -1) {
				write(1, "\033[31mError opening key_door.txt.\n\033[37m", strlen("\033[31mError opening key_door.txt.\n\033[37m"));
				return 1;
			}

			while ((ret = read(fd, buf+i, 1)) == 1) {
				if (buf[i] == '\n') {
					buf[i] = '\0';
					switch (readItem) {
						case 0: // key
							strncpy(keyDoor[keyDoorI].key, buf, i+1);
							readItem = 1;
							break;
						case 1: // door
							strncpy(keyDoor[keyDoorI].door, buf, i+1);
							readItem = 2;
							keyDoorI++;
							break;
						case 2: // Separation line
							readItem = 0;
					}
					buf[0] = '\0';
					i = 0;
				} else i++;
			}
			close(fd);

			// Check if target door requires key
			keyDoorI = 0;
			while (!found && keyDoorI < KEY_DOORS_SIZE) {
				if (strcmp(argv[3], keyDoor[keyDoorI].door) == 0) found = 1;
				else keyDoorI++;
			}

			if (found) {
				// Check if key used by player is the one that opens the target door
				if (strcmp(argv[2], keyDoor[keyDoorI].key) == 0) {
					// Change door permissions
					strncpy(commandPath, argv[0], sizeof(commandPath));
					strncat(commandPath, "/../chmod", sizeof(commandPath));
                                        realpath(dir->d_name, roomPath);

					if (fork() == 0) {
						//char *args[] = {roomPath, "0755"};
						execlp(commandPath, commandPath, roomPath, "0755", (char *) NULL);
						if (errno != 0) {
							printf("\033[31mProblem unlocking the door: %s.\n\033[37m", strerror(errno));
							return 1;
						}
					} else wait(NULL);
<<<<<<< HEAD
					write(1, "Door unlocked\n", strlen("Door unlocked\n"));
				} else write(1, "Wrong key.\n", strlen("Wrong key.\n"));
=======
					write(1, "Door unlocked\n", strlen("Dorr unlocked\n"));
				} else write(1, "\033[31mWrong key.\n\033[37m", strlen("\033[31mWrong key.\n\033[37m"));
>>>>>>> 8f53178a0c98d03465e88d24439767ab6b6f6d39

			} else write(1, "This door doesn't require any key to be opened.\n", strlen("This door doesn't require any key to be opened.\n"));

			break;
		case DT_LNK: // symbolic link (e.g. obj)
			if ((strcmp(argv[2], "laxatives") == 0) && (strcmp(argv[3], "coffee-machine") == 0)) {
				// Add laxatives to coffee machine and obtain coffeee with laxatives
				printf("You have poured laxatives into the coffee machine.\nThe coffee in the machine is now full of laxatives.\n[*] coffee-with-laxatives has been added to your inventory [*]\n");
				symlink("../../assets/tool/coffee-with-laxatives.tool", "../../../../Inv/coffee-with-laxatives.tool");
				// Remove laxatives from inventory
				unlink("../../../../Inv/laxatives.tool");
			} else {
				printf("Nothing happened.\n");
			}
			break;
		case DT_REG: // npc
			write(2, "You cannot use something in that target.\n", strlen("You cannot use something in that target.\n"));
			break;
		case -1:
			printf("\033[31mTarget not found\n\033[37m");
			break;
		default:
			printf("\033[31mUnknown file type\n\033[37m");
	}

	return 0;
}
