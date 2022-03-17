#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void use(int argc, char *argv[]){

 if(argc  < 2){
  fprintf(stdout, "You can only use an object at onces");
  exit(1);
 }

 if(argc < 2){
  fprintf(stdout,"You must select an item to use");
  exit(2);
 }

 int fdp[2];
 char recovery[30];
 if(fork() == 0){
 lose(pfd[0]); dup2(pfd[1],STDOUT_FILENO); close(pfd[1]);
  execlp("pwd","pwd",NULL);
 }else{
 close(pfd[0]);
 recovery = pfd[1];
 }

 chdir("../inv/");

 int item = open(argv[1],O_RDONLY,0600);
 char uses[5];
 char cmd[5];char arg1[5] ; char arg2[];
 read(item,uses,lseek(item,4,1));
 read(item,cmd,lseek(item,9,5));
 read(item,arg1,lseek(item,19,5));
 read(item,arg2,lseek(item,28,15));
 int u = atoi(uses);
 u = u-1;

 char* blank(char* input) //font:https://stackoverflow.com/questions/13084236/function-to-remove-spaces-from-string-char-array-in-c
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)
    {
     	if (input[i]!=' ')
            output[j]=input[i];
        else
            j--;
    }
    output[j]=0;
    return output;
}

 if(arg2[1] != NULL ){
  if(fork() == 0) execlp(blank(cmd),blank(cmd),blank(arg1),blank(arg2),NULL);
  }else(arg1[1] != NULL){
   if(fork() == 0) execlp(blank(cmd),blank(cmd),blank(arg1),NULL);
  }else{
   if(fork() == 0) execlp(blank(cmd),blank(cmd),NULL);
  }

 if(u == 0){
  fprint(stdout,"*I don't think  I'll nedd this anymore*");
  unlink(argv[1]);
 }else{
   char newu[2] = u +'0';
   write(item,4,newu);
 }

 chdir(recovery);
 exit(0);
}
