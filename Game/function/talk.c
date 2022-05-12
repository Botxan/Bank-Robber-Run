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
	char text[4096];
	int readChars=0;
	char continues;
	char state[2];
	char initialState;
	int error;
	char returnV;

#ifdef FUNCTION
int talk(char* npc){
#else
int main(int argc,char* argv[]) {

	if (argc<1&&strcmp(argv[0],"talk")!=0) return -1;
	char *npc=argv[1];
#endif
	end=0;
	var='A';
	error=0;
	readChars=0;
	current=getcwd(NULL,200);
	strcat(current,"/");
	strcat(current,npc);
	strcat(current,".npc");
	if(stat(current,&file)==-1) return 1;
	int fd=open(current,O_RDWR);
	if(fd==-1) return 1;
	lseek(fd,0,SEEK_SET);
	read(fd,&state[0],1);
	continues='y';
	state[1]='#';
	initialState=state[0];
	searchTalk(state,fd);
	while(continues=='y'&&error==0){
		write(0,npc,strlen(npc));
		write(0,":\n",strlen(":\n"));
            	branch[0]=i+'0';
		branch[1]=var;
		searchTalk(branch,fd);
		end=searchTalk("--",fd)-4;
		if(end>0){
			read(fd,&continues,1);
			lseek(fd,-end-3,SEEK_CUR);
			read(fd,&text,end);
			write(0,text,end);
 			text[0]='\0';
			if(continues=='y'){
				write(0,"\n-----Choose one-----\n",strlen("\n-----Choose one-----\n"));
				readChars=scanf(" %[^\n]%*c", &var);
		
				while(readChars!=1){
					write(0,"\nThere was an error, please choose an option again\n",strlen("\nThere was an error, please choose an option again\n"));
               				readChars=scanf(" %[^\n]%*c", &var);
				}
			}
			i++;
		} else{ 
			error=1;
			printf("The answer given wasn't an option restart conversation\n");
			}
	}
lseek(fd,3,SEEK_CUR);
read(fd,&returnV,1);
lseek(fd,0,SEEK_SET);
if(error==0) write(fd,&continues,1);
else write(fd,&initialState,1);
write(0,"\n",strlen("\n"));
return returnV+0;
}
