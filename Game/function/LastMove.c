#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>


int main(int argc, char* argv[]){

 if(argc > 1){
  write(1,"\033[31mYou don't need to type anything more that the command\n\033[37m",54);
  exit(1);
 }
execlp("/bin/tail","tail","moves.txt",NULL);
fprintf(stdout,"\n");
}
