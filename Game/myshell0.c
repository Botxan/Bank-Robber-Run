
// myShell0
//////////////////////////////////////////////////


#include <sys/syscall.h> 
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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
int eof;

static idStruct lookuptable[] = {
    { "Van", VAN }, { "MainEntrance", ENTRANCE }, { "MainBankingHall", HALL }, { "LostAndFound", LANDF }
};

char *prompt;
int dress;
char *home;
int id;
char *function;
char *root;

/////////// reading commands:
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

	if(strcmp(argv[0], "view") == 0 || strcmp(argv[0], "ls") == 0)
	{
		int child1=fork();
		if(child1==0)
		{
			char *path=strcat(function,"/view");
			execlp(path,*argv,argv[1],argv[2]);
			if (errno != 0)
			{
            			printf("Error launching child process: %s\n", strerror(errno));
            			return 1;
			}

		}
		if(child1>0)
		{
			wait(NULL);
		}

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


	}

	else if(strcmp(argv[0], "inv") == 0 || strcmp(argv[0], "inventory") == 0)
	{
		int child=fork();
		if(child==0)
		{
			write(0, "\n", strlen("\n"));
			char *path=strcat(function,"/inv");
			execl(path,argv[0],root);
		} 
		if(child>0)
		{
			if (errno!=0) 
			write(0, "Unknown error", strlen("Unknown error"));
		}
	}
	if(strcmp(argv[0], "pickUp") == 0 || strcmp(argv[0], "pu") == 0)
	{
		if(argc==2)
		{
			if(pickUp(root,argv[1])==1){
				if(errno==1) write(0,"Unknown error\n",strlen("Unknown error\n"));
				else write(0,"The object doesn't exist\n",strlen("The object doesn't exist\n"));
			}
		}

	}
	else if(strcmp(argv[0], "talk") == 0)
	{
	talk(argv[1]);
	
	
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
	else
	{
		write(1, "this function doesn't exit\n", strlen("this function doesn't exit\n"));
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

/////////////////////////////////////////////////
int main() {
	// Load the main menu
        int opt = show_main_menu();
  	eof=0;
   	int argc;
	char *args[MAXARGS];

	// Perform the corresponding actiond depending on user selection
	switch(opt) {
		case NEW_GAME:
			system("clear");
			write(2, "Starting new game...\n\n", 22);
			function = getcwd(NULL, 0);
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
	// If the user decides to start the game
	if (opt == 1) {
	   	function = getcwd(NULL, 0);
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
	}
}
