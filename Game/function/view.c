
#include <sys/syscall.h> 
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "view.h"



void view(int argc,char *argv[])
{
	struct dirent *dir; 
	struct stat *buf = malloc(sizeof(struct stat));

	switch(argc)
	{
		case 1:;
			DIR *d =opendir(".");
			while((dir =readdir(d)) !=NULL)
			{
				if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~'){
					write(0,dir->d_name, strlen(dir->d_name));
					write(0,"\n", strlen("\n"));
					
				}
			}
			closedir(d);
			break;
		case 2:;
			if(strcmp(argv[1],"-r")==0 || strcmp(argv[1],"-room")==0)
			{
				DIR *d =opendir(".");
				while((dir =readdir(d)) !=NULL)
				{
					stat(dir->d_name,buf);
					if (S_ISDIR(buf->st_mode))
					{
						if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~')
						{
							write(0,dir->d_name, strlen(dir->d_name));
							write(0,"\n", strlen("\n"));
					
						}
					
					}
				
				}
				closedir(d);
			}
			else
			{
					
					DIR *d =opendir(argv[1]);
					if(d)
					{
						while((dir =readdir(d)) !=NULL)
						{
							if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~')
							{
								write(0,dir->d_name, strlen(dir->d_name));
								write(0,"\n", strlen("\n"));
					
							}	
						}
						closedir(d);
					}
					else
					{
						write(1,"this room doesn't exist\n", strlen("this room doesn't exist\n"));
					}
					
			}
			break;
			
	}
	free(buf);
	
}

