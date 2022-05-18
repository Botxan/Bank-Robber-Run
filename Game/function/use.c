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
#include "../defines.h"

/*
 * Function: use
 * -------------
 * Uses a given option in the environment or the object
 * passed by parameter.
 * @param argc (in) number of arguments
 * @param argv the arguments for the program:
 *	- (in) Starting point of the game
	- (in) Current path
	- (in) command to be executed (use)
	- (in) Item to use
        - (in) (optional) target
 */
int main(int argc, char *argv[]){

	char invPath[strlen(argv[0]) + 6]; // root + "/Inv/ + "\0"
	char itemPath[strlen(argv[0]) + 26];
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
	char chmodPath[strlen(argv[0]) + 10];
	char roomPath[strlen(argv[1]) + 20];

	// Print args
	// for (int i = 0; i < argc; i++)
	//	printf("%d: %s\n", i, argv[i]);
	switch(argc) {
	}
	if (argc < 4) {
		write(1, "Select an item to use.\n", strlen("Select an item to use.\n"));
		return 1;
	}

	if (argc > 5) {
		write(1, "Too much arguments.\n", strlen("Too much arguments.\n"));
		return 1;
	}

	// Check if item is in inventory
	strncpy(invPath, argv[0], sizeof(invPath));
	strncat(invPath, "/Inv/", sizeof(invPath));
	strncpy(itemPath, invPath, sizeof(itemPath));
	strncat(itemPath, argv[3], sizeof(itemPath));

	if (access(itemPath, F_OK) == -1) {
		write(1, "You don't have that object in the inventory.\n", strlen("You don't have that object in the inventory.\n"));
		return 1;
	}

	// non-targeted use command
	if (argc == 4) {
		write(1, "Object used in the room. Nothing happened.\n.", strlen("Object used in the room. Nothing happened.\n"));
		return 0;
	}

	// Get file type
	d = opendir(".");
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			// If target found
			if (strcmp(dir->d_name, argv[4]) == 0) {
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
		case DT_LNK: // symbolic link
			// Read the key_door file to obtain key->door list
			strncpy(keyDoorPath, argv[0], sizeof(keyDoorPath));
			strncat(keyDoorPath, "/../assets/key_door.txt", sizeof(keyDoorPath));
			fd = open(keyDoorPath, O_RDONLY);

			if (fd == -1) {
				write(1, "Error opening key_door.txt.\n", strlen("Error opening key_door.txt.\n"));
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
				if (strcmp(argv[4], keyDoor[keyDoorI].door) == 0) found = 1;
				else keyDoorI++;
			}

			if (found) {
				// Check if key used by player is the one that opens the target door
				if (strcmp(argv[3], keyDoor[keyDoorI].key) == 0) {
					// Change door permissions
					strncpy(chmodPath, argv[0], sizeof(chmodPath));
					strncat(chmodPath, "/../chmod", sizeof(chmodPath));
                                        realpath(dir->d_name, roomPath);

					if (fork() == 0) {
						//char *args[] = {roomPath, "0755"};
						execlp(chmodPath, chmodPath, roomPath, "0755", (char *) NULL);
						if (errno != 0) {
							printf("Problem unlocking the door: %s.\n", strerror(errno));
							return 1;
						}
					} else wait(NULL);
					write(1, "Door unlocked\n", strlen("Dorr unlocked\n"));
				} else write(1, "Wrong key.\n", strlen("Wrong key.\n"));

			} else write(1, "This door doesn't require any key to be opened.\n", strlen("This door doesn't require any key to be opened.\n"));

			break;
		case DT_REG: // npc
			printf("regular file!\n");
			break;
		case -1:
			printf("Target not found\n");
			break;
		default:
			printf("Unknown file type\n");
	}

	return 0;
}
