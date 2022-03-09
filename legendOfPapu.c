#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//toDO: Hacer un sistema de enrgia para limitar los movimientos y hacer las funciones procesos


int main(int argc, char *argv[]){

 if(argc < 2){
  fprintf(stderr, "You must enter your name in order to begin with the adventure \n");
  exit(1);
 }

 if(argc > 2){
   fprintf(stderr,"We just need one name, try it again\n");
   exit(2);
 }
/*
 //Easter Egg
 if(strcmp(argv[1],"Pipo") == 0 || strcmp(argv[1],"pipo") == 0 ){
  fprintf(stdout, "Pipo era un buen perro ... Pipo ha muerto ... OH NO!\n~Alexelcapo~\nGAME OVER \n");
  exit(3);
 }else if(strcmp(argv[1],"Link") == 0 || strcmp(argv[1],"link") == 0 ){
  fprintf(stdout, "HEY! Listen \nWe're far from the Deku tree \n");
 }else if(strcmp(argv[1],"samus") == 0 ||strcmp(argv[2],"Samus") == 0){
  fprintf(stdout, "You mean Metroid? \n" );
 }
*/
 char map[10] = "./map/mr";
 char player[10];
 int room = 0;

 void help(){
  fprintf(stdout,"You can do the following actions: \n -go'n' , you go to the selected room, example go1 \n -view, it tolds you the rooms available \n -back, you go back to the last room \n -begining, you go back to the main room \n");
 }

 void root(){
  fprintf(stdout,"You're being teletransportated to the Main Room \n");
  chdir(map);
  room = 0;
 }

 int back(int r){
 fprintf(stdout,"You go back to the last room \n");
    chdir("..");
    r = r-1;
    return r;
 }

 void view(int r){

  int pfd[2];int cont;

   pipe(pfd);
   if(fork() == 0){
    close(pfd[0]); dup2(pfd[1],STDOUT_FILENO); close(pfd[1]);
    execlp("ls","ls",NULL);
   }if(fork() == 0){
    dup2(pfd[0],STDIN_FILENO); close(pfd[0]);
    dup2(pfd[1],STDOUT_FILENO); close(pfd[1]);
    execlp("wc","wc","-l",NULL);
   }
    close(pfd[0]);
    fprintf(stdout,"There're %d rooms available\n",pfd[1]);
 }


 void closeL3(){

  if(access("./1/13/132", X_OK) == 0){
   if(fork() == 0) execlp("chmod","chmod","0666","./1/13/132",NULL);
  }
  if(access("./2/22/222", X_OK) == 0){
   if(fork() == 0) execlp("chmod","chmod","0666","./2/22/222",NULL);
  }
  if(access("./2/22/223", X_OK) == 0){
   if(fork() == 0) execlp("chmod","chmod","0666","./2/22/223",NULL);
  }
  if(access("./3/31/311", X_OK) == 0){
   if(fork() == 0) execlp("chmod","chmod","0666","./3/31/311",NULL);
  }
 }

 void closeL2(){
  if(access("./2/22", X_OK) == 0){
   if(fork() == 0) execlp("chmod","chmod","0666","./2/22",NULL);
  }
 }

 void closeL1(){
  if(access("./3", X_OK) == 0){
   if(fork() == 0) execlp("chmod","chmod","0666","./3",NULL);
  }
 }

 fprintf(stdout,"Hello %s,\nWelcome to my labyrinth, if you find my treasure is all your's! \nGood luck !\n [Type 'help' to see the available commands] \n",argv[1]);
 chdir(map);
 closeL3();
 closeL2();
 closeL1();

 while(1){
  while(room == 0){
   fscanf(stdin,"%s",player);

   if(strcmp(player,"help")==0){
    help();
    memset(player,0,strlen(player));
    room = 0;
   }

   if(strcmp(player,"go1") == 0){
    if(chdir("./1") == 0){
     fprintf(stdout,"The door opens and you go to the next room\n");
     chdir("./1");
     room = 1;
    }else{
     fprintf(stdout,"The door is lock\n");
    }
   }else if(strcmp(player,"go2") == 0){
    if(chdir("./2") == 0){
     fprintf(stdout,"The door opens and you go to the next room\n");
     chdir("./2");
     room = 2;
    }else{
     fprintf(stdout,"The door is lock\n");
    }
   }else if(strcmp(player, "go3") == 0){
    if(chdir("./3") == 0){
     fprintf(stdout,"The door opens and you go to the next room\n");
     chdir("./3");
     room = 3;
    }else{
     fprintf(stdout,"The door is lock\n");
    }
   }

   if(strcmp(player,"view") == 0){
    view(room);
    memset(player,0,strlen(player));
   }

   if(strcmp(player,"back") == 0){
    fprintf(stdout,"You can't go back you're already in the Main Room\n");
    memset(player,0,strlen(player));
   }

   if(strcmp(player,"begining") == 0){
    fprintf(stdout,"You're already in the Main Room\n");
    memset(player,0,strlen(player));
   }
   memset(player,0,strlen(player));
  }

  fscanf(stdin,"%s",player);

 if(strcmp(player,"go1") == 0){
    if(chdir("./21") == 0){
     fprintf(stdout,"The door opens and you go to the next room\n");
     chdir("./21");
     room = 21;
    }else{
     fprintf(stdout,"The door is lock\n");
    }
   }else if(strcmp(player,"go2") == 0){
    if(chdir("./22") == 0){
     fprintf(stdout,"The door opens and you go to the next room\n");
     chdir("./22");
     room = 22;
    }else{
     fprintf(stdout,"The door is lock\n");
    }
  }

  if(strcmp(player,"help")==0){
   help();
   memset(player,0,strlen(player));
  }

  if(strcmp(player,"view") == 0){
   view(room);
   memset(player,0,strlen(player));
  }

  if(strcmp(player,"back") == 0){
   room = back(room);
  }

  if(strcmp(player,"begining") == 0){
   root();
  }
   memset(player,0,strlen(player));
  }

  while(room == 3){
  fscanf(stdin,"%s",player);

  if(strcmp(player,"go1") == 0){
     if(chdir("./31") == 0){
      fprintf(stdout,"The door opens and you go to the next room\n");
      chdir("./31");
      room = 31;
     }else{
      fprintf(stdout,"The door is lock\n");
     }
    }else if(strcmp(player,"go2") == 0){
     if(chdir("./32") == 0){
      fprintf(stdout,"The door opens and you go to the next room\n");
      chdir("./32");
      room = 32;
     }else{
      fprintf(stdout,"The door is lock\n");
     }
    }
   if(strcmp(player,"help")==0){
    help();
    memset(player,0,strlen(player));
   }

   if(strcmp(player,"view") == 0){
    view(room);
    memset(player,0,strlen(player));
   }

   if(strcmp(player,"back") == 0){
    room = back(room);
   }

   if(strcmp(player,"begining") == 0){
    root();
   }
    memset(player,0,strlen(player));
   }

   while(room == 311){
    fprintf(stdout,"You found my treasure!Enjoy it!\n CONGRATULATIONS!\n");
    exit(0);
   }
  }
 }
