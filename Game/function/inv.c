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
	if(pDot != NULL) *pDot = '\0';
}

int main(int argc,char* argv[]) {
	if (argc > 2)
	{
<<<<<<< HEAD
		write(0,"No arguments needed.\n",strlen("No arguments needed.\n"));
=======
		write(0,"\033[31mWe don't need argument for this function!\n\n\033[37m",strlen("\033[31mWe don't need argument for this function!\n\n\033[37m"));
>>>>>>> 8f53178a0c98d03465e88d24439767ab6b6f6d39
	 	return 1;
	}

	char *root = argv[1];
	strcpy(current, getcwd(current, 200));
	struct dirent *dir;
	struct stat *buf = malloc(sizeof(struct stat));
	char tool[1024]="------ Inventory ------\n";
	int chdirError = 0;
	char skin[20];

	chdirError += chdir(root);
	chdirError += chdir("Inv");

	if (chdirError < 0) return 1;

	DIR *d = opendir(".");
	while ((dir = readdir(d)) != NULL)
	{
	 	stat(dir -> d_name,buf);
		if (!S_ISDIR(buf -> st_mode))
		{
			if (strcmp(strrchr(dir->d_name, '.'), ".tool") == 0)
			{
				strcat(tool, "  ");
				deletextensionTool(dir->d_name);
				strcat(tool,dir->d_name);
				strcat(tool, "\n");
			}
			else if (strcmp(strrchr(dir->d_name, '.'), ".skin") == 0) {
				deletextensionTool(dir->d_name);
				strcpy(skin, dir->d_name);
		        }
		}
	}
	closedir(d);

	// Print tools
	strcat(tool,"\n");
	write(1, tool, strlen(tool));

	// Print skin
	write(1, "  Skin: ", 8);
	write(1, skin, strlen(skin));
	write(1, "\n", 1);

	chdir(current);
	return 0;
}
