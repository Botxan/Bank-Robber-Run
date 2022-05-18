
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


#include "defines.h"
#include "./function/view.h"
#include "./function/cd.h"
#include "./function/inv.h"
#include "./function/Leave.h"
#include "./function/talk.h"
#include "./function/newGame.h"

int eof;
char *prompt;
int dress;
char *home;
int id;
char *function;
char *root; // /Directories
char assets[PATH_MAX]; // /assets
char mapPath[PATH_MAX];
int table[100];
int visitedTimes;
char visitedTimesText[12];
int fd;
FILE *f;
int gameOver = 0;

//time
unsigned int hours=0;
unsigned int minutes=0;
unsigned int seconds=0;
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
 * Function: getTimesVisited
 * -------------------------
 * Opens the room description file for the given room, and returns the first
 * number found, which is the number of times the room has been accesed.
 * @param room the name of the room
 * @return the number of times the room has been accessed
 */
int getTimesVisited(char *room) {
	char path[PATH_MAX];
	FILE *f;
	int visitedTimes;

	// Current room
	if (room == NULL) strcpy(path, ".counter.txt"); // there's always a symlink in the current room
	// Any other room
	else {
		strncpy(path, root, PATH_MAX);
		strcat(path, "/../assets/roomVisitedCounter/");
		strncat(path, room, PATH_MAX);
		strncat(path, "Counter.txt", PATH_MAX-10);
	}


	if (!(f = fopen(path, "r"))) {
		perror("Error when opening room counter file");
		return -1;
	}

	fscanf(f, "%d", &visitedTimes);
	fclose(f);
	return visitedTimes;
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
	// Remove np (symlink) from current location
	char nameWithExtension[strlen(name) + 5];
	char find[PATH_MAX];
	sprintf(nameWithExtension, "%s.npc", name);
	sprintf(find, "find %s -type l -iname %s -delete 2>/dev/null", root, nameWithExtension);
	system(find);

	// Create new symlink on dest taking the file from assets/npc (if permissions are ok)
	char npcPath[PATH_MAX];
	char destPath[PATH_MAX];
	sprintf(npcPath, "%s/npc/%s", assets, nameWithExtension);
	sprintf(destPath, "%s/%s/%s", getcwd(NULL, 0), dest, nameWithExtension);
	symlink(npcPath, destPath);
}


/**
 * Function getNpcState
 * --------------------
 * Returns an integer indicating the state of the npc
 * Note: The state is always bounded by 0 and 9
 */
int getNpcState(char *name) {
	char npcPath[strlen(assets) + 40];
	char fd;
	char state[1]; // need to be string for read()

	sprintf(npcPath, "%s/npc/%s.npc", assets, name);
	fd = open(npcPath, O_RDONLY);
	read(fd, state, 1);
	close(fd);
	return state[0] - '0';
}


/**
 * Function getObjState
 * --------------------
 * Returns an integer indicating the state of the object
 * Note: The state is always bounded by 0 and 9
 */
int getObjState(char *name) {
	char objPath[strlen(assets) + 40];
	char fd;
	char state[1]; // need to be string for read

	sprintf(objPath, "%s/obj/%s.obj", assets, name);
	fd = open(objPath, O_RDONLY);
	read(fd, state, 1);
	close(fd);
	return state[0] - '0';
}

/**
 * Function: talk
 * --------------
 * This function is used for avoid repeating
 * the code for calling talk process
 *
 */
void talkTo(char *npc) {
	char commandPath[strlen(function) + 30];
	if (fork() == 0) {
		sprintf(commandPath, "%s/talk", function);
		execlp(commandPath, "talk", npc, NULL);
		// This will only be printed if execlp fails
		if (errno != 0) printf("Error on talk function: %s\n", strerror(errno));
		fprintf(stderr, "Unable not talk with %s.\n", npc);
	} else wait(NULL);
}

//////////////////////////////////////////////
int execute(int argc, char *argv[])
{
	char path[100];
	char path2[100];
	char auxPath[strlen(function) + 50];
	int child;
	strcpy(path,function);
	strcpy(path2,function);

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
		prompt=strrchr(getcwd(NULL, 0),'/')+1;
		if(strcmp(prompt, "VentilationDucts") == 0)
		{
			if(argv[2] !=NULL)
			{
				if(strcmp(argv[1], "SecurityRoom") == 0 || strcmp(argv[2], "SecurityRoom") == 0)
				{
					strcat(path,"/chmod");
					strcat(path2,"/Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom");
					int chmod7 = fork();
					if(chmod7 == 0) execlp(path,"./chmod",path2,"rwxrwxrwx", NULL);
					else wait(NULL);
				}
			}
			else
			{
				if(strcmp(argv[1], "SecurityRoom") == 0)
				{
					strcat(path,"/chmod");
					strcat(path2,"/Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom");
					int chmod7 = fork();

					if(chmod7 == 0) execlp(path,"./chmod",path2,"rwxrwxrwx", NULL);
					else wait(NULL);
				}
			}
		}

		// Main banking hall lights are on and guard is there and electrician skin is wear => guard prohibits entrance to coridor
		if ((strcmp(prompt, "MainBankingHall") == 0) && (strcmp(argv[1], "Corridor") == 0) && (access("Ramon.npc", R_OK) == 0) && ((getObjState("electrical-panel") == 2) || (getObjState("electrical-panel") == 3))) {
			if (fork() == 0) {
				strcpy(auxPath, function);
				strcat(auxPath, "/talk");
				execlp(auxPath, "talk", "Ramon", NULL);
				if (errno != 0) printf("Error, Ramon cannot talk right now: %s.\n", strerror(errno));
			} else wait(0);
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

			// Display prompt
                        prompt=strrchr(getcwd(NULL, 0),'/')+1;

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
					break;
				case ELECPANEL:
					if (visitedTimes == 1) {
						// Manager describes the electrical panel room
						talkTo("Edurne");

						// Move bank manager to electrical panel room
						moveNpc("Edurne", "MainBankingHall");
					}

					// view the janitors key card
					if (visitedTimes == 2) {
						sprintf(auxPath, "%s/assets/tool/janitor-card.tool", function);
						symlink(auxPath, "janitor-card.tool");
					}
					break;
				case CORRIDOR:
					if (visitedTimes == 1) {
						talkTo("Matt");
						moveNpc("Matt", "WC");
					}
			}
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
				if(errno!=1) write(0,"Unknown error\n",strlen("Unknown error\n"));
				else write(0,"The object doesn't exist\n",strlen("The object doesn't exist\n"));

			} else wait(NULL);
		} else write(0,"You can only take an object at a time",strlen("You can only take an object at a time"));
	}

	else if(strcmp(argv[0], "talk") == 0)
	{
		if(argc == 2)
		{
			if(fork() == 0)
			{
				strcat(path,"/talk");
				execl(path,argv[0],argv[1], NULL);
				if (errno != 0) printf("Error on talk function: %s\n", strerror(errno));
			} wait(NULL);
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
		if (child == 0) execlp("/bin/cat", "/bin/cat", mapPath, (char *) NULL);
		else wait(NULL);
	}
	else if (strcmp(argv[0], "Time") == 0)
	{
		char times2[100]="You have ";
		int temphour = time_left/3600;
		int tempminute = (time_left -(3600*temphour))/60;
		int tempseconds=(time_left -(3600*temphour)-(tempminute*60));
		char second[5];
		char Minute[5];
		char Hour[5];
		sprintf(second, "%d",  tempseconds);
		sprintf(Minute, "%d", tempminute);
		sprintf(Hour, "%d", temphour);
		strcat(times2,Hour),strcat(times2," h:"),strcat(times2,Minute),strcat(times2," m:"),strcat(times2,second),strcat(times2,"s left to finish the game \n");
		converttimeprint();
		write(0, times2, strlen(times2));
	}

	else if (strcmp(argv[0], "check") == 0)
	{
		if (fork() == 0) {
			char *path = strcat(function, "/check");
			execlp(path, argv[0], argv[1], function, NULL);
			if (errno != 0) {
				printf("Error launching child process: %s\n", strerror(errno));
				return 1;
			}
		} else wait(NULL);
	}

	else write(1, "this function doesn't exist\n", strlen("this function doesn't exist\n"));



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

	int compare=0;
	if(result==0)
	{
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

void printGameOver() {
	char command[strlen(assets) + 30];
	sprintf(command, "cat %s/gameOver.txt", assets);
	system(command);
}

//thread for time
void* Time1(){
	while(1){
		pthread_detach(pthread_self());
		countTime=clock();
		milliseconds=countTime-startTime;
		seconds=(milliseconds/(CLOCKS_PER_SEC))-(minutes*60);
		minutes=(milliseconds/(CLOCKS_PER_SEC))/60;
		hours=minutes/60;
		time_left=count_down_time_in_secs-seconds;
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
	char millisecond[5];
	char second[5];
	char Minute[5];
	char Hour[5];
	sprintf(millisecond, "%d", milliseconds);
	sprintf(second, "%d", seconds);
	sprintf(Minute, "%d", minutes);
	sprintf(Hour, "%d", hours);
	char times1[100]="Time :";
	strcat(times1,Hour),strcat(times1," h:"),strcat(times1,Minute),strcat(times1," m:"),strcat(times1,second),strcat(times1,"s \n");
	write(0, times1, strlen(times1));
}


/*
* Function: main
* --------------
* Displays the main menu and executes the action selected by the user
*/


int begin() {
	pthread_detach(pthread_self());
	// Load the main menu
	int opt = show_main_menu();
	eof=0;
	int argc;
	char *args[MAXARGS];
	function = getcwd(NULL, 0);
	// Obtain map file path
	strncpy(mapPath, function, PATH_MAX);
	strncat(mapPath, "/assets/map.txt", PATH_MAX-100);

	// Perform the corresponding action depending on user selection
	switch(opt) {
	case NEW_GAME:
		system("clear");
		newGame();
		sprintf(assets, "%s/assets", getcwd(NULL, 0));
		chdir("Directories");
		root = getcwd(NULL, 0);
		chdir("Van");
		home = getcwd(NULL, 0);
		prompt="Van";
		count_down_time_in_secs=7200;  // 1 minute is 60, 1 hour is 3600

		// Starting dialog
		if (fork() == 0) {
			execlp("../../talk", "talk", "Robert", NULL);
			if (errno != 0) {
                                printf("Error displaying starting dialogue: %s\n", strerror(errno));
                                return 1;
                        }
		} else wait(NULL);
		break;
	case LOAD_GAME:
		write(1, "Not implemented yet\n", 20);
		break;
	default:
		exit(1);
	}

	startTime=clock();  // start clock
        time_left=count_down_time_in_secs-seconds;   // update timer
        Time();

	while (1) {
        	write(0, prompt, strlen(prompt));
                write(0, ">", 1);
                if (read_args(&argc, args, MAXARGS, &eof) && argc > 0)
                	countpipe(argc,args,args);
                        //execute(argc, args);
                if (eof) exit(0);

		if ((gameOver == 1) || (time_left <= 0)) {
			printGameOver();
			exit(0);
		}
        }

	pthread_exit(NULL);
}

void *beginning()
{
	begin();
	pthread_exit(NULL);
}

int main() {
	pthread_t ptid;
  
    pthread_create(&ptid, NULL, &beginning, NULL);
	
	pthread_exit(NULL);
}
