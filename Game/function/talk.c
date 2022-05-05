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

int searchTalk(char *word , int file){
   char c;
   int i = 1;
   int readed;
   readed=read(file,&c,1);
   while(readed==1){
	if(c==word[0]){
		readed=read(file,&c,1);
		if(c==word[1]) return i+2;
	} else{
		readed=read(file,&c,1);
		i++;
	}

   }
   return -1;
}

	char *current;
	struct stat file;
	int i=0;
	char var='A';
	char branch[2]="  ";
	int end=0;
	char text[200];
	int readChars=0;
	char continues;


#ifdef FUNCTION
int talk(char* npc){
#else
int main(int argc,char* argv[]) {
	printf("Hello\n");

	if (argc<1 && argc>5)return -1;
	char *npc=argv[0];
#endif
	end=0;
	var='A';
	readChars=0;
	continues='1';
	current=getcwd(NULL,200);
	strcat(current,"/");
	strcat(current,npc);
	strcat(current,".npc");
	if(stat(current,&file)==-1) return 1;
	int fd=open(current,O_RDWR);
	if(fd==-1) return 1;
	lseek(fd,0,SEEK_SET);
	if(i!=0){
		write(0,"\nWe have nothing else to talk about\n",strlen("\nWe have nothing else to talk about\n"));
		return 0;
	}
	while(continues=='1'){
		write(0,npc,strlen(npc));
		write(0,":\n",strlen(":\n"));
            	branch[0]=i+'0';
		branch[1]=var;
		lseek(fd,0,SEEK_SET);
		searchTalk(branch,fd);
		end=searchTalk("--",fd)-4;
		read(fd,&continues,1);
		lseek(fd,-end-3,SEEK_CUR);
		read(fd,&text,end);
		write(0,text,end);
 		text[0]='\0';
		if(continues=='1'){
			write(0,"\n-----Choose one-----\n",strlen("\n-----Choose one-----\n"));
			readChars=scanf(" %[^\n]%*c", &var);
		}
		while(readChars!=1){
			write(0,"\nThere was an error, please choose an option again\n",strlen("\nThere was an error, please choose an option again\n"));
               		readChars=scanf(" %[^\n]%*c", &var);
		}
		i++;
	}
write(0,"\n",strlen("\n"));
return 0;
}
