

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

#include "Leave.h"


#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20


char * Prompt1;


/////////// reading commands:

int read_args1(int* argcp, char* args[], int max, int* eofp)
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
              fprintf(stderr,"\033[31mReading failure \n\033[37m");
              return 0;
              break;
     case -2 : *argcp = -1;     // line too long
              fprintf(stderr,"\033[31mLine too long -- removed command\n\033[37m");
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
      fprintf(stderr,"\033[31mToo many arguments -- removed command\n\033[37m");
      return 0;
   }
   *argcp= i;
   return 1;
}



int execute1(int argc, char *argv[])
{
	argc=argc;
	if(strcmp(argv[0], "Q") == 0 || strcmp(argv[0], "quit") == 0)
	{
		return 1;

	}
	if(strcmp(argv[0], "R") == 0 || strcmp(argv[0], "Resume") == 0)
	{
		return 0;
	}
	return 3;
		
}
/////////////////////////////////////////////////
#ifdef FUNCTION
int Leave()
#else
int main()
#endif
{
   int eof= 0;
   int argc;
   char *args[MAXARGS];
   Prompt1="PAUSE :\n";
   int t;
   while (1) {

      write(0,Prompt1, strlen(Prompt1));
	  write(0,"R : Resume\n",strlen("R : Resume\n"));
	  write(0,"Q : Quit\n",strlen("Q : Quit\n"));
	  write(0,"> ",strlen("> "));
      if (read_args1(&argc, args, MAXARGS, &eof) && argc > 0) {
          t=execute1(argc, args);
      }
      if(t == 0)
	  {
		  return 0;
	  }
	  else if(t ==1)
	  {
		  return 1;
	  }
   }

}
