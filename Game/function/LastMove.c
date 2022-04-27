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
  write(1,"You don't need to type anything more that the command\n",54);
  exit(1);
 }
execlp("/bin/tail","tail","moves.txt",NULL);
}
