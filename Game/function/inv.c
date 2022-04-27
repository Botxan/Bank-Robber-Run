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

void deletextensionTool(char argv[])
{
	char *pDot = strchr(argv, '.');
	if(pDot!=NULL)
	*pDot = '\0';
}

#ifdef FUNCTION
int inv(int argc,char* argv[]) {
#else
int main(int argc,char* argv[]) {
#endif

	if (argc!=4||strcmp(argv[0],"inv"))
	{
	 	return 1;
	}
	char *root=argv[1];
	strcpy(current,getcwd(current,200));
	struct dirent *dir;
	struct stat *buf = malloc(sizeof(struct stat));
	char tool[100]="\nThis are the things on your inventory:\n";
	int empty=1;
	int chdirError=0;
	chdirError+=chdir(root);
	chdirError+=chdir("Inv");
	if(chdirError<0)
	{
		return 1;
	}
	DIR *d=opendir(".");
	while((dir =readdir(d))!=NULL)
	{
	 	stat(dir->d_name,buf);
		if(!S_ISDIR(buf->st_mode))
		{
			empty=0;
			strcat(tool,"  ");
			deletextensionTool(dir->d_name);
			strcat(tool,dir->d_name);
			strcat(tool,"\n");	
		}
	}
	if(empty==1)
	{
		write(0,"Inventory empty!",strlen("Inventory empty!"));
		

	}
	else write(0,tool,strlen(tool));
	chdir(current);
return 0;
}
