#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>




#include "inv.h"



char current[200];


#ifdef FUNCTION
int inv(char* home){
strcpy(current,getcwd(current,200));
#else

int main(int argc,char* argv[]) {
if (argc!=1||strcmp(argv[0],"inv")) return 1;
strcpy(current,getcwd(current,200));
chdir("..");
char home[]="Directories";
#endif
struct dirent *dir;
struct stat *buf = malloc(sizeof(struct stat));
char tool[100]="\nThis are the things on your inventory:\n";
int empty=1;
chdir(home);
chdir("..");
chdir("Inv");
DIR *d=opendir(".");
	while((dir =readdir(d))!=NULL)
	{
		stat(dir->d_name,buf);
		if(!S_ISDIR(buf->st_mode))
		{
			empty=0;
			strcat(tool,"  ");
			strcat(tool,dir->d_name);
			strcat(tool,"\n");	
		}
	}
	if(empty==1) write(0,"\nYour inventory is empty!\n",strlen("\nYour inventory is empty!\n"));
	else write(0,tool,strlen(tool));
	chdir(current);
	
return 0;
}
