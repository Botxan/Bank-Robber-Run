#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int main (int argc, char* argv[])
{

 int line =0;
 int bs = 0;
 int i = 0;
 char buffC[20];char *cmd;char* token;
 int fd =  open("moves.txt",O_RDWR|O_CREAT,0666);

while(1)
{

 lseek(fd,0,SEEK_END);
 memset(buffC,0,20);

 while(read(STDIN_FILENO,buffC,20)>0)
 {
    token = strtok(buffC," ");

    while(token != NULL)
    {
     write(fd,token,strlen(token));
     token = strtok(NULL," ");
     if(token != NULL)
     {
      write(fd," ",1);
     }
    }
    break;
  }
}
return 0;
}