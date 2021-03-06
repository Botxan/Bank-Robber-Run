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
#include "util.c"

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
	if (argc < 3) {
		write(1, "\033[31mSelect an item to use.\n\033[37m", strlen("\033[31mSelect an item to use.\n\033[37m"));
		return 1;
	}

	if (argc > 4) {
		write(1, "\033[31mToo many arguments.\n\033[37m", strlen("\033[31mToo many arguments.\n\033[37m"));
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
        char *commandPath=malloc(PATH_MAX);
        char roomPath[PATH_MAX];
	char rootPath[PATH_MAX];
	char obj[30];
	char line[256];
	char opt;

        // Put extension to the tool
        strncpy(tool, argv[2], 20);
        strcat(tool, ".tool");

	// Check if item is in inventory
	strncpy(invPath, argv[0], sizeof(invPath));
	strncat(invPath, "/Inv/", sizeof(invPath));

	strncpy(itemPath, invPath, sizeof(itemPath));
	strncat(itemPath, tool, sizeof(itemPath));

	if (access(itemPath, F_OK) == -1) {
		free(commandPath);
		write(1, "\033[31mYou don't have that object in the inventory.\n\033[37m", strlen("\033[31mYou don't have that object in the inventory.\n\033[37m"));
		return 1;
	}

	// non-targeted use command
	if (argc == 3) {

		// Use radio to communicate with you teammate
		if (strcmp(argv[2], "radio") == 0) {
			// Get current room
			if (getcwd(cwd, sizeof(cwd)) == NULL) {
				free(commandPath);
				perror("getcwd() failed");
				return 1;
			}
			cwdBase = basename(cwd);

			// It does not make sense to use the radio in the van
			if (strcmp(cwdBase, "Van") == 0) write(1, "It does not make sense to use the radio in the van.\n", strlen("It does not make sense to use the radio in the van.\n"));
			// Check radio is safe for use (current room = electrical panel, wc or janitors room
			else if ((strcmp(cwdBase, "ElectricalPanelRoom") == 0) || (strcmp(cwdBase, "WC") == 0) || (strcmp(cwdBase, "JanitorRoom") == 0)) {

				if (fork() == 0) {
					// Get the root path of the project
	                                strncpy(rootPath, argv[0], PATH_MAX);
	                                strcat(rootPath, "/..");

        	                        strncpy(commandPath, argv[0], PATH_MAX);

                	                strcat(commandPath, "/../talk");
	                                execlp(commandPath, "talk", "Robert", rootPath, NULL);
	                                if (errno != 0) {
						free(commandPath);
        	                                write(2, "Could not talk with your gang: ", strlen("Could not talk with your gang"));
	                                        write(2, strerror(errno), strlen(strerror(errno)));
	                                        write(2, ".\n", 2);
        	                                return 1;
	                                }
				} else wait(NULL);
			} else write(1, "*This is not a safe place to use the radio*\n", strlen("*This is not a safe place to use the radio*\n"));

                } else if (strcmp(argv[2], "night-vision-goggles") == 0) {
			printf("\x1b[32m*You're wearing night vision goggles. Now you can see in the dark.*\x1b[37m\n");
			// change night vision goggles state to wearing (1) and unlink
			strncat(invPath, tool, PATH_MAX);
			int fd = open(invPath, O_WRONLY);
			write(fd, "1", 2);
			close(fd);
			unlink(invPath);

		} else if (strcmp(argv[2], "thesecret") == 0) {
			strncat(invPath, tool, PATH_MAX);
			char buff[500];
			int fd = open(invPath, O_RDONLY);
			read(fd, &buff, sizeof(buff));
			write(2, buff, sizeof(buff));
		} else write(1, "\033[31mObject used in the room. Nothing happened.\n\033[37m", strlen("\033[31mObject used in the room. Nothing happened.\n\033[37m"));

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
			// Director office opens only when cameras are off
			if ((strcmp(argv[2], "boss-picture") == 0) && (strcmp(argv[3], "BossOffice") == 0)) {
				if (getObjStateInRoom("SecurityRoom/monitors") == 0) {
					write(1, "\033[31mCannot perform this action with the cameras online.\n\033[37m", strlen("\033[31mCannot perform this action with the cameras online.\n\033[37m"));
                                	return 0;
				}
			}

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
				if (strcmp(tool, keyDoor[keyDoorI].key) == 0) {
					// Change door permissions
					strncpy(commandPath, argv[0], PATH_MAX);
					strncat(commandPath, "/../chmod", PATH_MAX);
                                        realpath(dir->d_name, roomPath);

					if (fork() == 0) {
						execlp(commandPath, commandPath, roomPath, "0755", (char *) NULL);
						if (errno != 0) {
							printf("\033[31mProblem unlocking the door: %s.\n\033[37m", strerror(errno));
							free(commandPath);
							return 1;
						}
					} else wait(NULL);

					write(1, "\x1b[32mDoor unlocked.\x1b[0m\n", strlen("\x1b[32mDoor unlocked.\x1b[0m\n"));
				} else write(1, "\033[31mWrong key.\n\033[37m", strlen("\033[31mWrong key.\n\033[37m"));

			} else write(1, "This door doesn't require any key to be opened.\n", strlen("This door doesn't require any key to be opened.\n"));

			break;
		case DT_LNK: // symbolic link (e.g. obj)

			if ((strcmp(argv[2], "laxatives") == 0) && (strcmp(argv[3], "coffee-machine") == 0)) {
				// Add laxatives to coffee machine and obtain coffeee with laxatives
				printf("You have poured laxatives into the coffee machine.\nThe coffee in the machine is now full of laxatives.\n\x1b[34mCoffee-with-laxatives has been added to your inventory.\x1b[0m\n");
				symlink("../../assets/tool/coffee-with-laxatives.tool", "../../../../Inv/coffee-with-laxatives.tool");
				// Remove laxatives from inventory
				unlink("../../../../Inv/laxatives.tool");

			} else if ((strcmp(argv[2], "hacking-tool") == 0) && strcmp(argv[3], "monitors") == 0) {
				// Hacking animation
				printf("\033[33m*Hacking security cameras...*\033[37m\n");
				sleep(1);
				printf("\033[33m*Executing hackCameras.sh...*\033[37m\n");
				sleep(1);
				printf("\033[33m*Waiting TCP response for reverse shell*\033[37m\n");
				sleep(1);
				printf("\033[33m0%%\033[37m\n");
				sleep(1);
				printf("\033[33m17%%\033[37m\n");
                                sleep(1);
				printf("\033[33m48%%\033[37m\n");
                                sleep(1);
				printf("\033[33m69%%\033[37m\n");
				sleep(1);
				printf("\033[33m100%%\033[37m\n");
				printf("\033[32m*Cameras hacked succesfully*\033[37m\n\n");

				// Change monitors state to 2 (hacked)
				setObjStateInRoom("monitors", 1);

				// Remove hacking-tool from inventory
				unlink("../../../../../Inv/hacking-tool.tool");

			} else if ((strcmp(argv[2], "decoder") == 0) && (strcmp(argv[3], "laptop") == 0)) {
				while (1) {
					printf("Which is the cypher?\n\n");
					printf("A: Rot13\n");
					printf("B: Base64\n");
					printf("C: Morse\n");
					printf("Choose an option: ");

					fgets(line, sizeof(line), stdin);
					opt = line[0];

					if (opt == 'A' || opt == 'B' || opt == 'C') break;
				}

				printf("Decoding pass.txt...\n");
				sleep(1);
				printf("...\n");
				sleep(1);
				printf("...\n");
				sleep(1);

				switch (opt) {
					case 'A':
						printf("ITuyVUOup3A3o3WxVTMipvO0nTHtMTS0LJWup2HtnKZ6VTIcM2u0VTMcqzHtqUqiVUE3oj==\n");
						return 0;
					case 'B':
						printf("The password for the database is: eight five two two\n");
						return 0;
					case 'C':
						printf("...- --. .... .-.. .. .... -... .... -.-. ...-- -. ...-- -... ...-- .--- -.- .. --. --.. ...- -.-. .. -... ----- .- --. ..- --. --.. --. ..-. ----- -.-- .-- .--- .... -.-. ..--- ..- --. .- -..- -- -.... .. --. ...- .--. --.. ..--- .... ----- .. --. --.. .--. -.. -- ..- --. -.. .... -.. ...- .. .... .-. ...-- -... .-- -...- -...-\n");
						return 0;

				}
			} else if ((strcmp(argv[2], "bleach") == 0) && (strcmp(argv[3], "coffee-machine") == 0)) {
				printf("*You have poisoned the coffee with bleach.*\n");
				// unlink bleach
				unlink("../../../../Inv/bleach.tool");
				// set coffeee-machine to poisoned (3)
				setObjStateInRoom("coffee-machine", 3);

			} else {
				write(1, "Nothing happened.\n", strlen("Nothing happened.\n"));
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

	free(commandPath);
	return 0;
}
