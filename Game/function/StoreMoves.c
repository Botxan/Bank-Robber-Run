
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "StoreMoves.h"

int Storemoves(char* argv[])
{

 //int line =0;
 //int bs = 0;
 int i = 0;
 /*char buffC[20];char buffIn[20];char *cmd;char* token;*/
 int fd =  open("moves.txt",O_RDWR|O_CREAT,0666);


 lseek(fd,0,SEEK_END);
 //memset(buffC,0,20);
 //printf("BATATA\n");
 /*while(1)
 {
  while(read(STDIN_FILENO,buffC,20)>0)
  {
     token = strtok(buffC," ");

     while(token != NULL)
     {*/
	while(argv[i] !=NULL)
	{
      write(fd,argv[i],strlen(argv[i]));
      write(fd," ",1);
	  i++;
	}
	write(fd,"\n",1);
      /*//write(STDOUT_FILENO, token, strlen(token));
      token = strtok(NULL," ");
      if(token != NULL)
      {
       write(fd," ",1);
      }
     }
   break;
  }
 }*/
 return 0;
}


