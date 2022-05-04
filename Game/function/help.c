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

 if(argc > 2)
 {
  fprintf(stdout,"You don't need any arguments for this command\n");
 }

 char *buff;
 int fd = open("help.txt",O_RDONLY,0666);

 while(read(fd,buff,1) > 0)
 {
  write(1,buff,1);
 }

 exit(0);
}
