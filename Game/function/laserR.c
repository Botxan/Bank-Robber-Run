
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMBER_OF_STRING 4
#define MAX_STRING_SIZE 3


int main(int argc, char *argv[])
{

 char player[MAX_STRING_SIZE]; char movesIn[NUMBER_OF_STRING][MAX_STRING_SIZE]; char movesOut[NUMBER_OF_STRING][MAX_STRING_SIZE];
 char buff[1];
 int i = 0; int o = 0; int b = 0;

  while(i < NUMBER_OF_STRING)
  {
   fscanf(stdin,"%s",player);
   strcpy(movesIn[i],player);
   memset(player,0,strlen(player));
   i++;
  }

  fprintf(stdout,"Hora de volver : %s %s %s\n",movesIn[0],movesIn[1],movesIn[2]);
  memset(player,0,strlen(player));
  i--;

  while(o < NUMBER_OF_STRING)
  {
   fscanf(stdin,"%s",player);
   strcpy(movesOut[o],player);
   if(strcmp(movesOut[o],movesIn[i]) != 0)
   {
    b =1;
    break;
   }
   memset(player,0,strlen(player));
   i--;
   o++;
  }
  if(b == 0)printf("Wuena\n");
  else printf("Cagastes\n");
  exit(0);
}

