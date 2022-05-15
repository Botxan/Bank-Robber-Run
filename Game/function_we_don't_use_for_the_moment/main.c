
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main(int argc, char *argv[]){

 if(argc > 2)
 {
  fprintf(stdout,"You don´t need to enter anything else\n");
  exit(1);
 }

 int room = 0;
 int menu = 0;
 int sel;int fd;
 char buffT[1];char player[20];

 fd = open("title.txt",O_RDONLY,0666);
 while(read(fd,buffT,1) > 0)
 {
  write(1,buffT,1);
 }
 close(fd);
 //MENU
 while(menu == 0)
 {
  fscanf(stdin,"%s",player);
  if(strcmp(player,"1") == 0)
  {
   menu = 1;
   memset(player,0,strlen(player));
  }
  else if(strcmp(player,"4") == 0)
  {
   menu = 4;
   memset(player,0,strlen(player));
  }else
  {
   fprintf(stdout,"Please select an option of the menu\n");
  }
  memset(player,0,strlen(player));
 }
 //GAME
 if(menu == 1)
 {
  while(1)
  {
   // Creatinf the log: ERROR ocurred always
   if(fork() == 0)
   {
    execlp("./StoreMoves","./StoreMoves",NULL);
   }else{
   fprintf(stdout,"Fatal error occurred closing game\n");
   exit(3);
   }

   while(room == 0)
   {
    fprintf(stdout,"Okey do you remember the plan? LET'S ROLL\n");
    room++;
   }
   while(room == 1)
   {
    fscanf(stdin,"%s",player);
    if(strcmp(player,"gato") == 0)
    {
     fprintf(stdout,"Miaum");
    }
    memset(player,0,strlen(player));
   }
  }
 }else if(menu == 4 )
 {
  exit(2);
 }
 exit(0);
}
