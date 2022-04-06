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




#include "pickUp.h"

int searchAmount(char *word , int file){
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
int pickUp(char* root, char* tool){
strcpy(current,getcwd(current,200));
#else

int main(int argc,char* argv[]) {
	if (argc!=2||strcmp(argv[0],"pickUp")) return 1;
	strcpy(current,getcwd(current,200));
	chdir("..");
	char root[]="Directories";
	char *tool=argv[1];
#endif
	strcat(current,"/");
	strcat(current,tool);
	strcat(current,".tool");
	struct stat file;
	if(stat(current,&file)==-1) return 1;
	char* invPath = malloc(sizeof(root)+sizeof(tool)+sizeof("/Inv/")+sizeof(".tool")+1);
	strcat(invPath,root);
	strcat(invPath,"/Inv/");
	strcat(invPath,tool);
	strcat(invPath,".tool");
	if(stat(invPath,&file)==0)
	{
		//read file and search for amount keyword
		int fd=open(invPath,O_RDONLY);
		if(fd==-1) return 1;
		int pos=searchAmount("amount:",fd);
		int toff=lseek(fd,pos,SEEK_CUR);
		char amount='0';
		read(fd,&amount,1);
		char newAmount= amount + 1;
		lseek(fd,toff,SEEK_SET);
		write(fd,&newAmount, 1);
		write(0,"You now have ", strlen("You now have "));
		write(0,&newAmount, 1);
		write(0," ", strlen(" "));
		write(0,tool,strlen(tool));
		write(0,"(s)\n", strlen("(s)\n"));
	}
	else
	{
		link(current,invPath);
		unlink(current);
	}

return 0;
}
