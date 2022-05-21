// myShell0
//////////////////////////////////////////////////

#include <limits.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <libgen.h>

#include "defines.h"
#include "./function/view.h"
#include "./function/cd.h"
#include "./function/inv.h"
#include "./function/Leave.h"
#include "./function/resetGame.h"
#include "./function/interaction/officerBack.c"
#include "./function/interaction/validateId.c"
#include "./function/interaction/displayVaultCorridorPath.c"
#include "./function/StoreMoves.h"

int eof;
char *prompt;
int dress;
char *home;
int id;
char *function;
char *root; // /Directories
char assets[PATH_MAX]; // /assets
char mapPath[PATH_MAX];
char *savePath;
int table[100];
int visitedTimes;
char visitedTimesText[12];
int fd;
FILE *f;
int pfd[2];
int savefd;
char lastLocation[PATH_MAX];


// situational variables
int distractedGuard = 0;
int isGameOver = 0;
int introduceIdAttempts = 0;
char* introducedId;


// Vault corridor variables
#define maxL 2
#define maxA 7
int i = 0;
int o = 0;
char *ans[] = {"up", "up", "left", "left", "up", "up", "up"};
char mIn[maxA][maxL];
char mOut[maxA][maxL];
char player[maxL];
char *argvStoreMoves[1];


//time
unsigned int hours=0;
unsigned int minutes=0;
unsigned int seconds=0;
unsigned int tempseconds=0;
unsigned int milliseconds=0;
unsigned int totaltime=0,count_down_time_in_secs=0,time_left=0;
clock_t countTime;
clock_t startTime;
void converttimeprint();

idStruct lookuptable[19] = {
	{"Van", VAN},
	{"MainEntrance", ENTRANCE},
	{"MainBankingHall", HALL},
	{"LostAndFound", LANDF},
	{"ElectricalPanelRoom", ELECPANEL},
	{"Corridor", CORRIDOR},
	{"Office1", OFF1},
	{"Office2", OFF2},
	{"SecurityRoom", SEC},
	{"WC", WC},
	{"VentilationDucts", VENT},
	{"JanitorsRoom", JANITOR},
	{"BossOffice", BOSS},
	{"Rooftop", ROOF},
	{"Parking", PARKING},
	{"Basement", BASEMENT},
	{"VaultCorridor", VAULTC},
	{"VaultRoom", VAULTR},
	{"Vault", VAULT}
};

/*
* Function: read_args
* ------------------------
* Reads player's input from stdin.
* @param argcp (out): number of arguments introduced by the user.
* @param args (out): pointer to the first char of the arguments introduced by the user.
* @param max (in): maximum number of parameters allowed.
* @param eofp (out): End of file pointer.
*/
int read_args(int* argcp, char* args[], int max, int* eofp) {
	static char cmd[MAXLINE];
	char* cmdp;
	int ret, i = 0;

	*argcp = 0;
	*eofp = 0;

	// Read stdin byte by byte until line break or exceed MAXLINE
	while ((ret=read(0,cmd+i,1)) == 1) {
		if (cmd[i]=='\n') break;  // correct line
		i++;
		if (i>=MAXLINE) { // line too long
			ret=-2;
			break;
		}
	}

	// check if introduced input is valid
	switch (ret) {
	case 1: // correct reading
		cmd[i+1]='\0';
		break;

	case 0: // end of file
		*eofp = 1;
		return 0;

	case -1: // reading failure
		*argcp = -1;
		fprintf(stderr,"Reading failure \n");
		return 0;

	case -2 : // line too long
		*argcp = -1;
		fprintf(stderr,"Line too long -- removed command\n");
		return 0;
	}

	// Analyzing the line
	cmdp = cmd;
	for (i=0; i<max; i++) {  /* to show every argument */
		if ((args[i]= strtok(cmdp, " \t\n")) == (char*)NULL) break;
		cmdp= NULL;
	}

	if (i >= max) {
		fprintf(stderr,"Too many arguments -- removed command\n");
		return 0;
	}

	*argcp= i;
	return 1;
}


////////////////////////////////////////////////////
//function for room ids
int idFromName(char *newRoom)
{
	int i;
	idStruct room;
	for (i=0; i < NKEYS; i++) {
		room = lookuptable[i];
		if (strcmp(room.name, newRoom) == 0)
		return room.id;
	}
	return 0;
}

/**
 * Function moveNpc
 * ----------------
 * Moves the given npc to the specified room
 * npc: path of the symlink of the npc
 * destination: the relative path to be placed the new symlink
 * Note: this function wont remove npc from its original place
 * if the player has no needed permissions in any of the directories
 * in the path to the current position of the npc
 */
void moveNpc(char *name, char *dest) {
	char *nameWithExtension=malloc(PATH_MAX);
	char *find=malloc(PATH_MAX);
	char *npcPath=malloc(PATH_MAX);
	char *destPath=malloc(PATH_MAX);
	char *destPath1=malloc(PATH_MAX);
	FILE *fp;

	// Remove npc (symlink) from current location
	sprintf(nameWithExtension, "%s.npc", name);
	sprintf(find, "find %s -type l -iname %s -delete 2>/dev/null", root, nameWithExtension);
	system(find);

	// Create new symlink on dest taking the file from assets/npc (if permissions are ok)
	sprintf(npcPath, "%s/npc/%s", assets, nameWithExtension);

	// Find the room
	sprintf(find, "find %s -type d -name %s 2>/dev/null", root, dest);
	fp = popen(find, "r");
	if (fp == NULL) printf("Failed to move %s to new location.\n", name);
	fgets(destPath1, sizeof(destPath1), fp);
	pclose(fp);

	destPath[strcspn(destPath, "\n")] = 0; // remove the newline
	sprintf(destPath, "%s/%s.npc", destPath1, name);
	symlink(npcPath, destPath);
	free(nameWithExtension);
	free(find);
	free(npcPath);
	free(destPath);
	free(destPath1);
}

/* Function removeNpc
 * ------------------
 * Removes the npc from the scenario
 */
void removeNpc(char *name) {
	char *nameWithExtension=malloc(PATH_MAX);
        char *find=malloc(PATH_MAX);

        // Remove npc (symlink) from current location
        sprintf(nameWithExtension, "%s.npc", name);
        sprintf(find, "find %s -type l -iname %s -delete 2>/dev/null", root, nameWithExtension);
        system(find);
		free(nameWithExtension);
		free(find);
}


/**
 * Function: getNpcState
 * --------------------
 * Returns an integer indicating the state of the npc
 * Note: The state is always bounded by 0 and 9
 * @param name the name of the npc
 */
int getNpcState(char *name) {
	char *npcPath=malloc(PATH_MAX);
	char fd;
	char state[1]; // need to be string for read()

	sprintf(npcPath, "%s/npc/%s.npc", assets, name);
	fd = open(npcPath, O_RDONLY);
	read(fd, state, 1);
	close(fd);
	free(npcPath);
	return state[0] - '0';

}

/**
 * Function: setNpcState
 * ---------------------
 * Changes the state of the given npc
 * Note: The state is always bounded by 0 and 9
 * @param name the name of the npc
 * @param state the new state of the npc
 */
void setNpcState(char *name, int state) {
	char *npcPath=malloc(PATH_MAX);
	char fd;
	char newState[10];
	sprintf(npcPath, "%s/npc/%s.npc", assets, name);
	fd = open(npcPath, O_WRONLY);
	sprintf(newState, "%d", state);
	write(fd, newState, 1);
	free(npcPath);
	close(fd);
}


/**
 * Function getObjState
 * --------------------
 * Returns an integer indicating the state of the object
 * Note: The state is always bounded by 0 and 9
 */
int getObjState(char *name) {
	char *objPath=malloc(PATH_MAX);
	char fd;
	char state[1]; // need to be string for read

	sprintf(objPath, "%s/obj/%s.obj", assets, name);
	fd = open(objPath, O_RDONLY);
	read(fd, state, 1);
	close(fd);
	free(objPath);
	return state[0] - '0';
}


/**
 * Function getToolState
 * --------------------
 * Returns an integer indicating the state of the tool
 * Note: The state is always bounded by 0 and 9
 */
int getToolState(char *name) {
        char *objPath=malloc(PATH_MAX);
        char fd;
        char state[1]; // need to be string for read

        sprintf(objPath, "%s/tool/%s.tool", assets, name);
        fd = open(objPath, O_RDONLY);
        read(fd, state, 1);
        close(fd);
        free(objPath);
        return state[0] - '0';
}



/**
 * Function: talkTo
 * ----------------
 * This function is used for avoid repeating
 * the code for calling talk process
 *
 */
void talkTo(char *npc) {
	char *commandPath=malloc(PATH_MAX);
	if (fork() == 0) {
		sprintf(commandPath, "%s/talk", function);
		execlp(commandPath, "talk", npc, function, NULL);
		// This will only be printed if execlp fails
		if (errno != 0) printf("Error on talk function: %s\n", strerror(errno));
		fprintf(stderr, "Unable not talk with %s.\n", npc);
	} else wait(NULL);
	free(commandPath);


	// [*] Special interactions [*]

	// If ramon arrests you, game over
        if (getNpcState("Ramon") == 8) isGameOver = 1;

}

/**
 * Function: getTimesVisited
 * -----------------------------
 * Returns an integer indicating the number of times
 * the given room has been visited
 */
int getTimesVisited(char *roomName) {
	char *roomPath=malloc(PATH_MAX);
	FILE *f;
	int state = -1;
	sprintf(roomPath, "%s/roomVisitedCounter/%sCounter.txt", assets, roomName);
	f = fopen(roomPath, "r");
	fscanf(f, "%d", &state);
	fclose(f);
	free(roomPath);

	return state;
}

/*
 * Function: hasItem
 * -----------------
 * Checks if the player has the given object in the inventory
 * @return 0 if the player has the object in the inventory.
 * Otherwise 1
 */
int hasTool(char *name) {
	char *toolPath=malloc(PATH_MAX);

	sprintf(toolPath, "%s/Inv/%s.tool", root, name);
	if (access(toolPath, R_OK) == 0) {
		free(toolPath);
		return 0;
	}
	free(toolPath);
	return 1;

}

/*
 * Function: removeTool
 * -----------------
 * Removes the given tool from player's directory
 */
void removeTool(char *name) {
        char *toolPath=malloc(PATH_MAX);
        sprintf(toolPath, "%s/Inv/%s.tool", root, name);
        unlink(toolPath);
		free(toolPath);
}


/*
 * Function: hasSkin
 * -----------------
 * Checks if the payer is wearing the given skin
 * @ return 0 if the player is wearing the given skin.
 * Otherwise 1
 */
int hasSkin(char *skin) {
	char *skinPath=malloc(PATH_MAX);

	sprintf(skinPath, "%s/Inv/%s.skin", root, skin);
        if (access(skinPath, R_OK) == 0) {
			free(skinPath);
			return 0;
		}
		free(skinPath);
        return 1;
}

//////////////////////////////////////////////


int execute(int argc, char *argv[])
{
	char path[100];
	char path2[100];
	int child;
	strcpy(path,function);
	strcpy(path2,function);

	// Get current directory basename
	prompt = strrchr(getcwd(NULL, 0), '/') + 1;

	if(strcmp(argv[0], "view") == 0 || strcmp(argv[0], "ls") == 0)
	{
		child=fork();
		if(child==0)
		{
			strcat(path,"/view");
			execlp(path,"",argv[1],argv[2], NULL);
			if (errno != 0)
			{
				printf("Error launching child process: %s\n", strerror(errno));
				return 1;
			}

			} else wait(NULL);

		return 1;
	}
	else if(strcmp(argv[0], "access") == 0 || strcmp(argv[0], "cd") == 0)
	{
		// Pre entering events

		// Using shortcut from electrical panel room to ventilation ducts => avoid possible events
		if ((strcmp(prompt, "ElectricalPanelRoom") == 0) && (strcmp(argv[1], "VentilationDucts") == 0)) {
			// Only if available of course
			if (getTimesVisited("VentilationDucts") > 0) {
				//moveGuard
				moveNpc("Ramon", "MainEntrance");
			}
		}


		// Unreachable ventilation ducts => Too high (hint for ladder)
		if ((strcmp(prompt, "WC") == 0) && (strcmp(argv[1], "VentilationDucts") == 0)) {
			struct stat statBuf;
			if (stat("VentilationDucts", &statBuf) < 0) {
				write(2, "VentiltionDucts not found\n", strlen("VentilationDucts not found\n"));
				exit(0);
			}

			mode_t bits =  statBuf.st_mode;
			if ((bits & S_IXUSR) == 0) {
				write(1, "\033[31mUnreachable. The ducts are too high.\033[37m\n", strlen("\033[31mUnreachable. The ducts are too high.\033[37m\n"));
				return 0;
			}
		}

		// Main banking hall lights are on and guard is there and electrician skin is wear => guard prohibits entrance to coridor
		if ((strcmp(prompt, "MainBankingHall") == 0) && (strcmp(argv[1], "Corridor") == 0) && (access("Ramon.npc", R_OK) == 0) && (getObjState("electrical-panel") >= 2)) {
			talkTo("Ramon");
			return 0;
		}

		// Before leaving office1 for first time, office is coming back!!
		if ((strcmp(prompt, "Office1") == 0) && ((strcmp(argv[1], "Corridor") == 0) || (strcmp(argv[1], "..") == 0)) && (getTimesVisited("Office1") == 1) && (getNpcState("Matt") != 5) && (getNpcState("Matt") != 6) && (getNpcState("Matt") != 7)) {

			char decision = officerBack();
			moveNpc("Matt", "Office1");
			setNpcState("Matt", 1);
			talkTo("Matt");

			switch(decision) {
				case 'A': // Inside cabinet
					setNpcState("Matt", 2);
					talkTo("Matt");
					moveNpc("Matt", "Rooftop");
					break;
				case 'B': // Behind curtains (game over)
					setNpcState("Matt", 3);
					talkTo("Matt");
					isGameOver = 1;
					break;
				case 'C': // Knock out
					setNpcState("Matt", 4);
					talkTo("Matt");
					break;
			}
				return 0;
		}

		// Knocking Jade's door and Jade is not unconscious (7)
		if ((strcmp(prompt, "Corridor") == 0) && (strcmp(argv[1], "Office2") == 0) && (getNpcState("Jade") != 7)) {
			if ((getObjState("electrical-panel") == 0) || (getObjState("electrical-panel") == 2)) {
				setNpcState("Jade", 6);
				moveNpc("Jade", "Corridor"); // need to duplicate because not enough permissions in office 2
				talkTo("Jade");
				removeNpc("Jade"); // remove duplicated
			} else {
				if ((hasTool("coffee-with-laxatives") == 0) && hasSkin("executive") == 0) setNpcState("Jade", 1);
				else setNpcState("Jade", 0);
				moveNpc("Jade", "Corridor");
				talkTo("Jade");
				// If Jade takes laxatives, move to bathroom and open her office room
				if (getNpcState("Jade") == 7) {
					// Open office 2
					system("chmod 755 ./Office2");
					removeNpc("Jade");
					moveNpc("Jade", "WC");
					removeTool("coffee-with-laxatives");
				}
				else removeNpc("Jade");

				if (getNpcState("Jade") == 9) isGameOver = 1;
			}

			return 0;
		}


		// Lost found dialogues depending if the player has checked lost and found email
		// and current skin
		if ((strcmp(prompt, "MainBankingHall") == 0) && (strcmp(argv[1], "LostAndFound") == 0)) {
			// Email not read
			if ((getObjState("office2-laptop") == 0) && (hasTool("officer-card") != 0)) setNpcState("Veronica", 0);
			else { // email read
				if (hasSkin("electrician") == 0) setNpcState("Veronica", 1);
				if (hasSkin("executive") == 0) {
					if (introduceIdAttempts == 0) setNpcState("Veronica", 2);
					else setNpcState("Veronica", 3);
				}
			}
		}

		// Security room is closed from corridor side
		if ((strcmp(prompt, "Corridor") == 0) && (strcmp(argv[1], "SecurityRoom") == 0)) {
			printf("\033[31mIt is locked from the inside.\033[37m\n");
			return 0;
		}


		// Directors room is opened with boss picture (biometric access)
		if ((strcmp(prompt, "Corridor") == 0) && (strcmp(argv[1], "BossOffice") == 0)) {
			struct stat statBuf;
                        if (stat("BossOffice", &statBuf) < 0) {
                                write(2, "BossOffice not found\n", strlen("BossOffice not found\n"));
                                exit(0);
                        }

                        mode_t bits =  statBuf.st_mode;
                        if ((bits & S_IXUSR) == 0) {
				printf("\033[31mIt is locked. Looks like it uses some kind of facial recognition.\033[37m\n");
                                return 0;
                        } else {
				if (getObjState("coffee-machine") != 3) {
				printf("\033[31mCannot enter while the director is inside. There has to be a way to get him out.\033[37m\n");
                                return 0;
				}
			}
		}


		// Accessing the vault corridor without night google visions equiped => deny
		if ((strcmp(prompt, "Basement") == 0) && (strcmp(argv[1], "VaultCorridor") == 0) && (getToolState("night-vision-googles") == 0)) {
			printf("\033[31mNo light enters the next room, and it is the one with the laser system. I'll need something to see in there.\033[37m\n");
			return 0;
		}


		if(cd(argc,argv,home,0)==1)
		{
			// Increase room visited counter
			fd = open("./.counter.txt", O_RDWR);


			if (fd == -1) write(2, "Could not add room visited counter. File .counter not found\n.", strlen("Could not add room visited counter. File .counter not found\n."));

			f = fdopen(fd, "r"); // used to read whole integer with fscanf (may be more than one digit)
			fscanf(f, "%d", &visitedTimes); // fscanf moves the fd offset
			lseek(fd, 0, SEEK_SET);
			visitedTimes++;
			sprintf(visitedTimesText, "%d", visitedTimes);
			write(fd, visitedTimesText, strlen(visitedTimesText));
			close(fd);
			fclose(f);
			/*
			// Display prompt
                        prompt=strrchr(getcwd(NULL, 0),'/')+1;

			//Save location and time left on file
			savefd=open(savePath,O_TRUNC,O_RDWR);
			close(savefd);
			savefd=open(savePath,O_RDWR);
			char *currentWD;
			currentWD=getcwd(NULL,0);
			int iVan=0;
			while(!(currentWD[iVan]=='V'&&currentWD[iVan+1]=='a'&&currentWD[iVan+2]=='n')){iVan++;}//find Van in the string and get pos
			char *saveText;
			currentWD[iVan+3]='#';
			saveText=strrchr(currentWD,'#')+1;
			write(savefd,saveText,strlen(saveText));
			write(savefd,"#",1);
			dprintf(savefd,"%d",time_left);
			*/
			// Special interactions in each room
                        id=idFromName(argv[1]);


			switch(id)
			{
				case ENTRANCE:
					// Bank manager accompanies player to the electrical panel room
					if (visitedTimes == 1) {
						// Talk with the bank manager
						talkTo("Edurne");

						// Move to main banking hall with bank manager
						moveNpc("Edurne", "MainBankingHall");
						char *argv[2] = {"access", "MainBankingHall"};
						execute(2, argv);
					}
					break;
				case HALL:
					if (visitedTimes == 1) {
						// Manager describes the main banking hall
						talkTo("Edurne");

						// Move to electrical panel room with the bank manager
						moveNpc("Edurne", "ElectricalPanelRoom");
						char *argv[2] = {"access", "ElectricalPanelRoom"};
						execute(2, argv);
					}
					if(getObjState("electrical-panel")<2){
						printf("Haven't they thought of installing some windows?? You can't see shit with the lights off...\n\n");

					}
					break;

				case ELECPANEL:
					if (visitedTimes == 1) {
						// Manager describes the electrical panel room
						talkTo("Edurne");

						// Move bank manager to electrical panel room
						moveNpc("Edurne", "MainBankingHall");
					}

					// Dialog to distract ward is available while corridor has not reached and guard
					// is in the main banking hall
					if ((getTimesVisited("Corridor") == 0) && (getNpcState("Ramon") != 9)) {
						setNpcState("Robert", 1);
					}
					break;
				case CORRIDOR:
					if (visitedTimes == 1) {
						// Robert will no longer will display the dialog to distract the guard
						setNpcState("Robert", 4);
						// The ward will return to the main banking hall
						setNpcState("Ramon", 0);
						moveNpc("Ramon", "MainBankingHall");
						distractedGuard = 0;
						talkTo("Matt");

						// If you are rude with Matt
						if (getNpcState("Matt") == 9) isGameOver = 1;

						moveNpc("Matt", "WC");
						setNpcState("Mat", 6);
					}
					break;
				case VENT:
					// Unlock the ventilation ducts to electrical panel (bidirectional) and to the security room (unidirectional)
					if (visitedTimes == 1) {
						printf("\033[32m*You have discovered the ventilation ducts!*\033[37m\n");
						printf("\033[32m*Ventilation ducts are now accesible also from the electrical panel room**\033[37m\n");

						// Create new symlink
						symlink("../Corridor/WC/VentilationDucts", "../../../ElectricalPanelRoom/VentilationDucts");

						// Unlock the security room
						if (fork() == 0) {
							execlp("../../../../../../../chmod", "chmod", "../../SecurityRoom", "0775", NULL);
							printf("\033[31mError changing SecurityRoom permissions: %s.\n\033[37m", strerror(errno));
							exit(0);
						}
					}

					// If coming from electrical panel (shortcut), place guard in main banking hall again
					if (strcmp(prompt, "ElectricalPanel") == 0) moveNpc("Ramon", "MainBankingHall");
				case BOSS:
					// Could not move before (missing permissions in boss office)
					if (visitedTimes == 0) {
						removeNpc("Ignacio");
						moveNpc("Ignacio", "WC");
					}
					break;
			}

			// Display prompt
                        prompt=strrchr(getcwd(NULL, 0),'/')+1;

			// Special interaction
			// Returning to corridor when coffee machine is poisoned and monitors have been hacked
			// If bleach has been used in coffee machine and monitors have been hacked
                        if ((strcmp(prompt, "Corridor") == 0) && (getObjState("coffee-machine") == 3) && (getObjState("monitors") == 1) && (getNpcState("Ignacio") == 0)) {
                        	moveNpc("Ignacio", "Corridor");
                                talkTo("Ignacio");

				// Ignacio is moved to the bathroom
				moveNpc("Ignacio", "WC");
				// The basement card is found in the WC
				symlink("../../../../../../assets/tool/basement-card.tool", "WC/basement-card.tool");
                        }

			// Vault corridor minigame
			if ((strcmp(prompt, "VaultCorridor") == 0) || (hasTool("thesecret") == 0)) {
				if((visitedTimes % 2) != 0) {
                                	displayVaultCorridorPath();
                                       	i = 0;
                                        while(i < maxA) {
                                        	fscanf(stdin, "%s", player);
                                                argvStoreMoves[0] = player;
                                                Storemoves(argvStoreMoves);
                                                strcpy(mIn[i], player);

                                                if(strcmp(ans[i], ans[i]) != 0 || strcmp(ans[i], player) != 0) {
							printf("\n");
							printf("\033[31m*Incorrect path*\033[37m\n");
                                                	isGameOver = 1;
                                                        break;
                                                } else i++;

                                                memset(player, 0, strlen(player));

                                                if(i == 7) {
                                                	printf("\033[32mOk Im in.\033[37m\n");
                                                        chdir("./VaultRoom");
                                                        prompt = "VaultRoom";
                                                }
                                        }
                       		} else {
	                        	memset(player, 0, strlen(player));
	                                o = 0;
	                                i = 6;
	                                while(o < maxA) {
	                                	fscanf(stdin, "%s", player);
	                                        strcpy(mOut[o], player);

	                                        if(strcmp(ans[i], mOut[o]) != 0 || strcmp(player, ans[i]) != 0) {
							printf("\n");
	                                        	isGameOver = 1;
	                                                break;
	                                       	} else {
	                                        	i--;
	                                                o++;
	                                        }

	                                        memset(player,0,strlen(player));

	                                        if(o == 7) {
							printf("\n");
							isGameOver = 2;
	                                        }
	                               	}
	                      	}
			}

                        //Save location and time left on file
                        savefd=open(savePath,O_TRUNC,O_RDWR);
                        close(savefd);
                        savefd=open(savePath,O_RDWR);
                        char *currentWD;
                        currentWD=getcwd(NULL,0);
                        int iVan=0;
                        while(!(currentWD[iVan]=='V'&&currentWD[iVan+1]=='a'&&currentWD[iVan+2]=='n')){iVan++;}//fin$
                        char *saveText;
                        currentWD[iVan+3]='#';
                        saveText=strrchr(currentWD,'#')+1;
                        write(savefd,saveText,strlen(saveText));
                        write(savefd,"#",1);
                        dprintf(savefd,"%d",time_left);
		}
	}

	else if(strcmp(argv[0], "inv") == 0 || strcmp(argv[0], "inventory") == 0)
	{

		child=fork();
		if(child==0)
		{
			write(0, "\n", strlen("\n"));
			strcat(path,"/inv");
			execlp(path,"",root,argv[1],NULL);
			if (errno!=0) write(0, "Unknown error\n", strlen("Unknown error\n"));
		}
		else
		{
			wait(NULL);
		}
	}

	else if(strcmp(argv[0], "pickup") == 0 || strcmp(argv[0], "pu") == 0)
	{
		if(argc == 2)
		{
			if(fork() == 0)
			{
				write(0, "\n", strlen("\n"));
				strcat(path,"/pickup");
				execl(path,argv[0],argv[1],root,NULL);
				if(errno!=1) printf("Error on pickup function: %s\n", strerror(errno));
				else write(0,"The object doesn't exist\n",strlen("The object doesn't exist\n"));

			} else wait(NULL);
		} else write(0,"You can only take an object at a time\n",strlen("You can only take an object at a time\n"));
	}

	else if(strcmp(argv[0], "talk") == 0)
	{
		if(argc == 2) {
			// No dialogues in the Van, just follow radio instructions
			if ((strcmp(prompt, "Van") == 0) && (strcmp(argv[1], "Robert") == 0)) {
				write(1, "If you need my help, use the radio in safe zones.\n", strlen("If you need my help, use the radio in safe zones.\n"));
				return 0;
			}

			talkTo(argv[1]);

			// Special interactions

			// Telling id to Veronica in Lost and Found
			if ((strcmp(prompt, "LostAndFound") == 0) && (strcmp(argv[1], "Veronica") == 0)) {
				// If waiting for the ID (3)
				if (getNpcState("Veronica") == 3) {
					// Correct id
					if (validateId() == 0)  {
						setNpcState("Veronica", 6);
						talkTo("Veronica");

						// Add card to the inventory
						char cardPath[PATH_MAX];
						strcpy(cardPath, assets);
						strcat(cardPath, "/tool/officer-card.tool");
						char invPath[PATH_MAX];
						strcpy(invPath, root);
						strcat(invPath, "/Inv/officer-card.tool");
						symlink(cardPath, invPath);
						printf("*officer-card has been added to your inventory*\n");

						// update radio status
						if (getNpcState("Robert") == 4) setNpcState("Robert", 5);
						else setNpcState("Robert", 6);

						if (fork() == 0) {
							execlp("../../../../../chmod","chmod","../Corridor/Office1","0066", NULL);
					                printf("\033[31mError changing Office1 permissions: %s.\n\033[37m", strerror(errno));
					                exit(0);
						} else wait(NULL);

						if (fork() == 0) {
                                                        execlp("../../../../../chmod","chmod","../Corridor/Office2","0066", NULL);
                                                        printf("\033[31mError changing Office2 permissions: %s.\n\033[37m", strerror(errno));
                                                        exit(0);
                                                } else wait(NULL);

					} else {
						introduceIdAttempts++;
						// Check if maximum attempts reached => Game over
						if (introduceIdAttempts == 3) {
							setNpcState("Veronica", 5);
							talkTo("Veronica");
							isGameOver = 1;
						} else {
							setNpcState("Veronica", 4);
							talkTo("Veronica");
							printf("\nRemaining attempts: %d\n", 3-introduceIdAttempts);
						}
					}
				}

				// If card obtained (4), return to default dialogue
				if (getNpcState("Veronica") == 4) setNpcState("Veronica", 0);
			}
		} else write(0,"You can only talk to a person at a time\n",strlen("You can only talk to a person at a time\n"));
	}

	else if(strcmp(argv[0], "Pause") == 0 || strcmp(argv[0], "P") == 0|| strcmp(argv[0], "quit") == 0|| strcmp(argv[0], "q") == 0)
	{
		int t =Leave();
		if(t==1)
		{
			eof=1;
			exit(0);
		}
		if(t==0)
		{
			eof=0;
		}
		if(t==2)
		{
			//save don't implement for the moment
			eof=1;
		}


	}
	else if (strcmp(argv[0], "Pwd") == 0 || strcmp(argv[0], "pwd") == 0 || strcmp(argv[0], "Room") == 0 || strcmp(argv[0], "r") == 0|| strcmp(argv[0], "R") == 0 || strcmp(argv[0], "room") == 0 )
	{
		int child4=fork();
		if(child4==0)
		{
			char *path=strcat(function,"/pwd");
			execlp(path,*argv,NULL);
			if (errno != 0)
			{
				printf("Error launching child process: %s\n", strerror(errno));
				return 1;
			}

		}
		if(child4>0)
		{
			wait(NULL);
		}
	}
	else if (strcmp(argv[0], "use") == 0)
	{
		child = fork();
		if (child == 0) {
			char *path=strcat(function,"/use");
			execlp(path, root, argv[0], argv[1], argv[2], argv[3], NULL);
			if (errno != 0) {
				printf("Error launching child process: %s\n", strerror(errno));
				return 1;
			}
		} else wait(NULL);

		// Special interactions
                // After using radio, robert distracting guard (4), move guard
                if ((getNpcState("Robert") == 4) && !distractedGuard) {
                        moveNpc("Ramon", "MainEntrance");
                        setNpcState("Ramon", 2);
                        distractedGuard = 1;
                }

		// Bleach used in coffee machine (+ security cameras deactivated)
		if((strcmp((strrchr(getcwd(NULL, 0),'/') + 1), "Corridor") == 0) && (getNpcState("Ignacio") == 0) && (strcmp(argv[1], "bleach") == 0) && (strcmp(argv[2], "coffee-machine") == 0) && (getObjState("coffee-machine") == 3) && (getObjState("monitors") == 1)) {
			moveNpc("Ignacio", "Corridor");
			talkTo("Ignacio");
			moveNpc("Ignacio", "WC");
			// Add basement key card on the floor
			symlink("../../../../../../assets/tool/basement-card.tool", "WC/basement-card.tool");
		}
	}
	else if (strcmp(argv[0], "map") == 0)
	{
		child = fork();
		if (child == 0) execlp("/bin/cat", "/bin/cat", mapPath, (char *) NULL);
		else wait(NULL);
	}
	else if (strcmp(argv[0], "time") == 0)
	{
		//char times2[100]="Electrician arrives in ";
		int temphour = time_left/3600;
		int tempminute = (time_left -(3600*temphour))/60;
		int tempseconds=(time_left -(3600*temphour)-(tempminute*60));
		//char second[5];
		//char Minute[5];
		//char Hour[5];
		printf("\x1b[33mElectrician will arrive in %02d:%02d.\x1b[0m\n", tempminute, tempseconds);
		//sprintf(second, "%d",  tempseconds);
		//sprintf(Minute, "%d", tempminute);
		//sprintf(Hour, "%d", temphour);
		//strcat(times2,Hour),strcat(times2," h:"),strcat(times2,Minute),strcat(times2," m:"),strcat(times2,second),strcat(times2,"s left to finish the game \n");
		converttimeprint();
		//write(0, times2, strlen(times2));
	}

	else if (strcmp(argv[0], "check") == 0)
	{
		if (fork() == 0) {
			if (argc == 2) {
				char *path = strcat(function, "/check");
				execlp(path, argv[0], argv[1], root, NULL);
				if (errno != 0) {
					printf("Error launching child process: %s\n", strerror(errno));
					return 1;
				}
			} else write(2, "Usage: check <object>\n", strlen("Usage: check <object>\n"));
		} else wait(NULL);
	}
	else if (strcmp(argv[0], "man") == 0)
	{
		char *back=getcwd(NULL, 0);
		chdir(function);
		if (fork() == 0) {
			char *path = strcat(function, "/man");
			execlp(path, argv[1], NULL);
			if (errno != 0) {
				printf("Error launching child process: %s\n", strerror(errno));
				return 1;
			}
		}
		else
		{
			wait(NULL);
			chdir(back);
		}
	}
	else if(strcmp(argv[0],"log")==0){
		FILE *fp=malloc(sizeof(FILE));
		char format[1000]="";
		fp = fopen("moves.txt", "r");
		if(fp==NULL)
		{
				return 0;
		}
		else
		{
			while (fgets(format,1000,fp)){
				write(0," ",strlen(" "));
				write(0,format,strlen(format));
			}


			fclose(fp);
			//free(fp);
			return 1;
		}
	}

	else write(1, "\033[31mthis function doesn't exist \n\033[37m ", strlen("\033[31mthis function doesn't exist \n\033[37m"));



	return 1;
}


int countpipe(int argc,char *argv[],char *test[])
{
	int i=1;
	int result=0;
	int t=0;
	while(t !=100)
	{
		table[t]=0;
		t++;
	}

	while(argv[i] !=NULL)
	{

		if(strcmp(argv[i], "||") == 0)
		{
			table[result]=i;
			result++;
		}
		i++;
	}
	int compare = 0;
	if(result==0)
	{
		Storemoves(test);
		execute(argc, test);

	}
	else
	{
		int number=0;
		table[result]=i;
		while(result+1 != compare)
		{
			char *save[100];
			int j=0;
			while(j !=100)
			{
				save[j]=NULL;
				j++;
			}
			int p=0;
			while(table[compare] != number)
			{
				save[p]=argv[number];
				number++;
				p++;
			}
			compare++;
			number++;
			Storemoves(save);
			execute(p, save);
			write(0,"\n",strlen("\n"));
		}
	}
	return 1;
}

/*
* Function: show_main_menu
* ------------------------
* Displays the main menu of the game
* returns: the option selected by the player, -1 in case of error
*/
int show_main_menu() {
	system("clear");
	int fd = open(MAINMENU, O_RDONLY, 0);
	if (fd == -1) {
		write(2, "Unable to open the file ", 24);
		write(2, MAINMENU, strlen(MAINMENU));
		write(2, "\n", 1);
		return -1;
	}

	// Print the menu
	char data[2048];
	read(fd, data, 2048);
	close(fd);
	write(0, data, strlen(data));


	// User selection
	char opt[MAXLINE];
	int i = 0;

	// Loop until valid option is selected, and directly return
	while(1) {
		write(0, "Select an option [1|2|3|4]: ", 29);

		while (read(0, opt+i, 1)) {
			if (opt[i] == '\n') break; // end of user input
			i++;
		}

		// If only one character has been introduced
		if (i == 1)
			switch (opt[0]) {
				case '1': // New game
				case '2': // Load game (not implemented yet)
					return opt[0] - '0';
				case '3': // Quit
					exit(0);
				default: // Invalid option
					write(0, "Invalid option\n", 16);
			}
		// If more than one character, invalid
		else write(0, "Invalid option\n", 16);

		i = 0;
	}
	return -1;
}


/**
 * Displays the end of the gam and resets
 **/
void gameOver() {
char command[strlen(assets) + 30];
	printf("Is game over: %d\n", isGameOver);
	if (isGameOver == 1) {
		sprintf(command, "cat %s/gameOver.txt", assets);
		system(command);
	} else if (isGameOver == 2) {
		sprintf(command, "cat %s/congratulations-ascii.txt", assets);
		system(command);
		printf("Time spent: %d minutes and %d seconds.\n\n", minutes, seconds);
	}
	chdir(function);
	resetGame();
	exit(0);
}

//thread for time
void* Time1(){
	while(1){
		pthread_detach(pthread_self());
		countTime=clock();
		milliseconds=countTime-startTime;
		seconds=(milliseconds/(CLOCKS_PER_SEC))-(minutes*60);
		tempseconds=(milliseconds/(CLOCKS_PER_SEC));
		minutes=(milliseconds/(CLOCKS_PER_SEC))/60;
		hours=minutes/60;
		time_left=count_down_time_in_secs-tempseconds;
	}
	pthread_exit(NULL);
}

void Time(){
	pthread_t ptid;

    pthread_create(&ptid, NULL, &Time1, NULL);
    //printf("This line may be printed"
      //     " before thread terminates\n");

	//printf("");

    // The following line terminates
    // the thread manually
    //pthread_cancel(ptid);

    // Compare the two threads created
    //if(pthread_equal(ptid, pthread_self()))
      //  printf("Threads are equal\n");
    //else
      //  printf("Threads are not equal\n");

    // Waiting for the created thread to terminate
    //pthread_join(ptid, NULL);

   // printf("This line will be printed"
     //      " after thread ends\n");

    //pthread_exit(NULL);
}

//convert number to time

void converttimeprint()
{
	//char millisecond[5];
	//char second[5];
	//char Minute[5];
	//char Hour[5];
	//sprintf(millisecond, "%d", milliseconds);
	//sprintf(second, "%d", seconds);
	//sprintf(Minute, "%d", minutes);
	//sprintf(Hour, "%d", hours);
	//char times1[100]="Time :";
	//strcat(times1,Hour),strcat(times1," h:"),strcat(times1,Minute),strcat(times1," m:"),strcat(times1,second),strcat(times1,"s \n");
	printf("Elapsed time: %02d:%02d.\n", minutes, seconds);
	//write(0, times1, strlen(times1));
}


/*
* Function: main
* --------------
* Displays the main menu and executes the action selected by the user
*/


int begin() {
	pthread_detach(pthread_self());
	// Load the main menu
	savePath=malloc(PATH_MAX);
	int opt = show_main_menu();
	eof=0;
	int argc;
	char *args[MAXARGS];
	function = getcwd(NULL, 0);
	// Obtain map file path
	strncpy(mapPath, function, PATH_MAX);
	strncat(mapPath, "/assets/map.txt", PATH_MAX-100);
	strncpy(savePath, function, PATH_MAX);
	strncat(savePath, "/assets/save.txt",PATH_MAX);
	sprintf(assets, "%s/assets", getcwd(NULL, 0));

	// Perform the corresponding action depending on user selection
	switch(opt) {
	case NEW_GAME:
		system("clear");
		resetGame();
		// Print bank robber run ascii and begin the game!
		system("cat ./assets/newGameAscii.txt");

		chdir("Directories");
		root = getcwd(NULL, 0);
		chdir("Van");

		// Change starting path for fast testing!
		//system("chmod 777 Van/MainEntrance/Parking/Basement");
		//chdir("Van/MainEntrance/Parking/Basement");

		home = getcwd(NULL, 0);
		prompt="Van";
		count_down_time_in_secs=2400;  // 1 minute is 60, 1 hour is 3600

		// Starting dialog
		talkTo("Robert");
		break;
	case LOAD_GAME:
		savefd=open(savePath,O_RDWR);
		system("clear");
		lseek(savefd,0,SEEK_SET);
		int pathSize=0;
		char checkEnd=' ';
		while(checkEnd!='#'&&pathSize<PATH_MAX){//look for divider between path and time left
			read(savefd,&checkEnd,1);
			pathSize++;
		}
		lseek(savefd,0,SEEK_SET);
		read(savefd,&lastLocation,pathSize-1);
		lseek(savefd,1,SEEK_CUR);
		char secs[4];
		read(savefd,&secs,4);
		chdir("Directories");
		root = getcwd(NULL, 0);
                chdir("Van");
                home = getcwd(NULL, 0);
		chdir(lastLocation);
                prompt=strrchr(getcwd(NULL, 0),'/')+1;
		count_down_time_in_secs=atoi(secs);  // 1 minute is 60, 1 hour is 3600
		printf("You are now in: %s with %d seconds left\n",prompt,count_down_time_in_secs);
		close(savefd);
		break;
	default:
		exit(1);
	}
	startTime=clock();  // start clock
        time_left=count_down_time_in_secs-seconds;   // update timer
        Time();

	/*
    if(pipe(pfd))
    {
        write(0,"Log Pipe ERROR", strlen("Log Pipe ERROR"));
        exit(2);
    }

    switch(fork())
    {
    case -1:
    write(0,"FORK ERROR",strlen("FORK ERROR"));
	break;
    case 0:
    close(pfd[0]);*/
    	while (1) {
			write(0, "\033[36m", strlen("\033[36m"));
        	write(0, prompt, strlen(prompt));
			write(0, "\033[32m", strlen("\033[32m"));
                write(0, ">", 1);
				write(0, "\033[37m", strlen("\033[37m"));
                if (read_args(&argc, args, MAXARGS, &eof) && argc > 0)
                	countpipe(argc,args,args);
		        //write(pfd[1],args,strlen(args));
                        //execute(argc, args);
                if (eof) {
					free(savePath);
					exit(0);
				}

		if ((isGameOver != 0) || (time_left <= 0)) {
			gameOver();
			free(savePath);
			exit(0);
			}
		}
		free(savePath);
		/*
    default:
        close(pfd[1]);close(0);
        dup2(pfd[0],STDIN_FILENO);close(pfd[0]);
		char *path=strcat(function,"/StoreMoves");
        execlp(path,"StoreMoves",NULL);
    }
	*/
	pthread_exit(NULL);
}

void *beginning() {
	begin();
	pthread_exit(NULL);
}

int main() {
	pthread_t ptid;
	pthread_create(&ptid, NULL, &beginning, NULL);
	pthread_exit(NULL);
}
