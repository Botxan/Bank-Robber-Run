#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/syscall.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>




#include "talk.h"

int searchText(char *word , int file){
   char line[4] ;
   int i = 4; int e = 0;
while(e == 0){
   while(read(file,line,4) > 0){
    if(strcmp(line, word)){
     e = 1;
     return i;
    }
   memset(line,0,strlen(line));
   i = i+4;
   }
  return i;
 }
 return i;
}

char current[200];


#ifdef FUNCTION
int talk(char* npc){
#else
int main(int argc,char* argv[]) {
	if (argc!=2||strcmp(argv[0],"pickUp")) return 1;
	char *npc=argv[1];
#endif
	strcat(current,"/");
	strcat(current,npc);
	strcat(current,".npc");
	struct stat file;
	if(stat(current,&file)==-1) return 1;
		//read file and search for amount keyword
		int fd=open(current,O_RDWR);
		if(fd==-1)
		{
		return 1;
		}
		int interactions;
		read(fd,interactions,1):
		int pos;
		char[] i="0";
		char[] var="";
		char[] branch;
		int end;
		char[] text;
		while(i<interactions){
		branch="";
		strcat(branch,i);
		strcat(branch,var);
		pos=searchText(branch,fd);
		lseek(fd,pos,SEEK_CUR);
		end=searchText("--",fd);
		read(fd,&text,end-pos-2);
		write(0,text,strlen(text));
		while(scanf("%[^\n]", var)=!1) write(0,"\nThere was an error, please choose an option again",strlen("\nThere was an error, please choose an option again"));
		i++;

		}
return 0;
}
