
// myShell0
//////////////////////////////////////////////////

#include <limits.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


#include "defines.h"
#include "./function/view.h"
#include "./function/cd.h"
#include "./function/inv.h"
#include "./function/Leave.h"
#include "./function/pickUp.h"
#include "./function/talk.h"
#include "./function/NewGame.h"

int eof;
char *prompt;
int dress;
char *home;
int id;
char *function;
char *root;
char mapPath[PATH_MAX];

idStruct lookuptable[19] = {
        {"Van", VAN},
        {"MainEntrance", ENTRANCE},
        {"MainBanking Hll", HALL},
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
        {"Roof", ROOF},
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
//////////////////////////////////////////////
int execute(int argc, char *argv[])
{
	char path[100];
	int child;
	strcpy(path,function);
	if(strcmp(argv[0], "view") == 0 || strcmp(argv[0], "ls") == 0)
	{
		child=fork();
		if(child==0)
		{
			strcat(path,"/view");
			//write(0,path,strlen(path));
			execlp(path,*argv,argv[1],argv[2]);
			if (errno != 0)
			{
            			printf("Error launching child process: %s\n", strerror(errno));
            			return 1;
			}

		}
		if(child>0)
		{
			wait(NULL);
		}
		return 1;

	}
	else if(strcmp(argv[0], "access") == 0 || strcmp(argv[0], "cd") == 0)
	{
		char *roomText;
		if(cd(argc,argv,home,0)==1)
		{

		 	prompt=strrchr(getcwd(NULL, 0),'/')+1;
			id=idFromName(argv[1]);
			roomText="";
			switch(id)
			{
				case VAN :
				break;
				case ENTRANCE :
				roomText="nice bank!\n";
				break;
			}
			write(0,roomText,strlen(roomText));
		}
		return 1;

	}

	else if(strcmp(argv[0], "inv") == 0 || strcmp(argv[0], "inventory") == 0)
	{
		child=fork();
		if(child==0)
		{
			write(0, "\n", strlen("\n"));
			strcat(path,"/inv");
			execl(path,argv[0],root);
			if (errno!=0) write(0, "Unknown error\n", strlen("Unknown error\n"));
		}
		if(child>0)
		{
			wait(NULL);
			if (errno!=0) 
			write(0, "Unknown error", strlen("Unknown error"));
		}
	}
	if(strcmp(argv[0], "pickUp") == 0 || strcmp(argv[0], "pu") == 0)
	{
		if(argc==2)
		{
			child=fork();
			if(child==0)
			{
				write(0, "\n", strlen("\n"));
                        	strcat(path,"/pickUp");
                        	execl(path,root,argv[0]);
				if(errno!=1) write(0,"Unknown error\n",strlen("Unknown error\n"));
				else write(0,"The object doesn't exist\n",strlen("The object doesn't exist\n"));

			}
			if(child>0)
			{
				wait(NULL);
			}
		} else write(0,"You can only take an object at a time",strlen("You can only take an object at a time"));
	}
	else if(strcmp(argv[0], "talk") == 0)
	{
		if(argc==2)
		{
			child=fork();
			if(child==0)
			{
				write(0, "talk\n", strlen("talk\n"));
				strcat(path,"/talk");
				execl(path,argv[1], NULL);
				if (errno != 0) printf("Error on talk function: %s\n", strerror(errno));
			}
			if(child>0)
			{
				wait(NULL);
			}
		} else write(0,"You can only talk to a person at a time",strlen("You can only talk to a person at a time"));

	}
	else if(strcmp(argv[0], "Pause") == 0 || strcmp(argv[0], "P") == 0|| strcmp(argv[0], "quit") == 0|| strcmp(argv[0], "q") == 0)
	{
			int t =Leave();
			if(t==1)
			{
				eof=1;
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
                        execlp(path, root, getcwd(NULL, 0), argv[0], argv[1], argv[2], argv[3], NULL);
                        if (errno != 0) {
                                printf("Error launching child process: %s\n", strerror(errno));
                                return 1;
                        }
                } else wait(NULL);
        }
	else if (strcmp(argv[0], "map") == 0)
	{
		child = fork();
		if (child == 0) {
			execlp("/bin/cat", "/bin/cat", mapPath, (char *) NULL);
		} else wait(NULL);
	}
	else
	{
		write(1, "this function doesn't exist\n", strlen("this function doesn't exist\n"));
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
				case '3': // Options (not implemented yet)
			               	return (int)opt[0] - '0'; // convert to int
				case '4': // Quit
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

/*
 * Function: main
 * --------------
 * Displays the main menu and executes the action selected by the user
 */

int main() {
	// Load the main menu
        int opt = show_main_menu();
  	eof=0;
   	int argc;
	char *args[MAXARGS];
	function = getcwd(NULL, 0);

	// Obtain map file path
	strncpy(mapPath, function, PATH_MAX);
	strncat(mapPath, "/assets/map.txt", PATH_MAX-100);



	// Perform the corresponding actiond depending on user selection
	switch(opt) {
		case NEW_GAME:
			system("clear");
			NewGame();
			printf("function: %s\n", mapPath);
			write(2, "Starting new game...\n\n", 22);
	                chdir("Directories");
	                root = getcwd(NULL, 0);
	                chdir("Van");
	                home = getcwd(NULL, 0);
	                prompt="Van";
        	        while (1) {
                	        write(0, prompt, strlen(prompt));
                        	write(0, ">", 1);
	                        if (read_args(&argc, args, MAXARGS, &eof) && argc > 0)
	                                execute(argc, args);
	                        if (eof) exit(0);
	                }
			break;
		case LOAD_GAME:
			write(1, "Not implemented yet\n", 20);
			break;
		case OPTIONS:
			write(1, "Not implemented yet\n", 20);
			break;
		default:
			exit(1);
	}
}
