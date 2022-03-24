
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



#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20
///////ROOMS
#define VAN 0
#define ENTRANCE 1
#define HALL 2
#define LANDF 3
#define ELECPANEL 4
#define CORRIDOR 5
#define OFF1 6
#define OFF2 7
#define SEC 8

#define NKEYS 4
///////DRESS
#define SUIT 0
#define ELEC 1

#include "./function/view.h"
#include "./function/cd.h"

typedef struct { char *name; int id; } idStruct;

static idStruct lookuptable[] = {
    { "Van", VAN }, { "MainEntrance", ENTRANCE }, { "MainBankingHall", HALL }, { "LostAndFound", LANDF }
};

char * Prompt;
int dress;
char *home;
int id;
char *function;

/////////// reading commands:

int read_args(int* argcp, char* args[], int max, int* eofp)
{
   static char cmd[MAXLINE];
   char* cmdp;
   int ret,i;

   *argcp = 0;
   *eofp = 0;
   i=0;
   while ((ret=read(0,cmd+i,1)) == 1) {
      if (cmd[i]=='\n') break;  // correct line
      i++;
      if (i>=MAXLINE) {
         ret=-2;        // line too long
         break;
      }
   }
   switch (ret)
   {
     case 1 : cmd[i+1]='\0';    // correct reading 
              break;
     case 0 : *eofp = 1;        // end of file
              return 0;
              break;
     case -1 : *argcp = -1;     // reading failure
              fprintf(stderr,"Reading failure \n");
              return 0;
              break;
     case -2 : *argcp = -1;     // line too long
              fprintf(stderr,"Line too long -- removed command\n");
              return 0;
              break;
   }
   // Analyzing the line
   cmdp= cmd;
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
		if(fork()==0)
		{
			write(0,"\n", strlen("\n"));
			char *path=strcat(function,"/view");
			execl(path,*argv,argv[1],argv[2]);
			
			if (errno != 0)
			{
            			printf("Error launching child process: %s\n", strerror(errno));
            			return 1;
			}
		
		}

	}
	if(strcmp(argv[0], "access") == 0 || strcmp(argv[0], "cd") == 0)
	{
		char *roomText;
		if(cd(argc,argv,home,0)==1)
		{
			
		 	Prompt=strrchr(getcwd(NULL, 0),'/')+1;
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
	return 1;
}
/////////////////////////////////////////////////
int main ()
{
   int eof= 0;
   int argc;
   char *args[MAXARGS];
   function = getcwd(NULL, 0);
   chdir("Directories/Van");
   home = getcwd(NULL, 0);
   Prompt="Van";
   while (1) {

      write(0,Prompt, strlen(Prompt));
      write(0,">",strlen(">"));
      if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
         execute(argc, args);
      }
      if (eof) exit(0);
   }

}
