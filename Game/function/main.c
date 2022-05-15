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

 int aux = 0;
 int pfd[2];

 if(pipe(pfd))
 {
  fprintf(stdout,"Pipe ERROR");
  exit(1);
 }


 switch(fork())
 {
  case -1:
   fprintf(stdout,"Fork ERROR");
   exit(2);
  case 0:
   close(pfd[0]);//close(1);
   //close(pfd[1]);

   if(menu == 1)
   {
     while(1)
     {
      while(room == 0)
      {
       printf("Okey do you remember the plan? LET'S ROLL\n *You go in to the bank*\n");
       room++;
       printf("-- Type 'help' to enlist the available commands --\n");
      }
      while(room == 1)
      {
       fscanf(stdin,"%s",player);
       write(pfd[1],player,strlen(player));

       if(strcmp(player,"help") == 0 ||strcmp(player,"Help") == 0)
       {
        if(fork() == 0){ write(1,"\n",1);/*write(1) only in comands that use the stdout*/ execlp("./help","./help",NULL);}
       }

       if(strcmp(player,"LastMoves") == 0 ||strcmp(player,"lastmoves") == 0)
       {
        if(fork() == 0){ execlp("LastMoves","LastMoves",NULL);}
       }


       if(strcmp(player,"exit") == 0 )
       {
        //write(1,"\n",1);
        kill(getppid(),9);
        close(pfd[1]);
        exit(3);
       }
       memset(player,0,strlen(player));
      }
     }
   }else if(menu == 4 ) exit(4);

  default:
   close(pfd[1]);close(0);
   dup2(pfd[0],STDIN_FILENO); close(pfd[0]);
   execlp("StoreMoves","StoreMoves",NULL);
  // close(pfd[1]);
 }
 close(pfd[1]);
 exit(0);
}

