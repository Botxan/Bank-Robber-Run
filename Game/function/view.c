
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
//#include "cd.h"




int verif(char argv[])
{
	char test[2]="/.";
	for(size_t i=0;i<strlen(argv);i++)
	{
		for(size_t t=0;t<strlen(test);t++)
		{
			if(argv[i]==test[t])
			{
				write(1,"You can't use this atribut.\n\n", strlen("You can't use this atribut\n\n"));
				return 0;
			}
		}
	}
	return 1;
}

int extension(char argv[])
{
	if (!strrchr(argv, '.')) {
		return 0;
	} else {
		if(strcmp(strrchr(argv, '.'),".tool")==0)
		{
			
			return 1;
		}
		if(strcmp(strrchr(argv, '.'),".obj")==0)
		{
			return 2;
		}
		if(strcmp(strrchr(argv, '.'),".npc")==0)
		{
			return 3;
		}
	}
	return 0;
}

void deletextension(char argv[])
{
	char *pDot = strchr(argv, '.');
	if(pDot!=NULL)
	*pDot = '\0';
}


#ifdef FUNCTION
int view(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
	
	struct dirent *dir; 
	struct stat *buf = malloc(sizeof(struct stat));
	char room[100]="Room:\n";
	char tool[100]="tool:\n";
	char object[100]="object:\n";
	char npc[100]="npc:\n";

	switch(argc)
	{
	case 1:;
		DIR *d =opendir(".");
		while((dir =readdir(d)) !=NULL)
		{
			stat(dir->d_name,buf);
			if (S_ISDIR(buf->st_mode))
			{
				if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~')
				{
					strcat(room,"  ");
					strcat(room,dir->d_name);
					strcat(room,"\n");
				}
			}
			else
			{
				if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~'){
					int info=extension(dir->d_name);
					switch(info)
					{
					case 1:
						deletextension(dir->d_name);
						strcat(tool,"  ");
						strcat(tool,dir->d_name);
						strcat(tool,"\n");
						break;
					case 2:
						deletextension(dir->d_name);
						strcat(object,"  ");
						strcat(object,dir->d_name);
						strcat(object,"\n");
						break;
					case 3:
						deletextension(dir->d_name);
						strcat(npc,"  ");
						strcat(npc,dir->d_name);
						strcat(npc,"\n");
						break;
					}
					
					
				}
			}
			
		}
		write(0,room, strlen(room));
		write(0,"\n", strlen("\n\n"));
		write(0,tool, strlen(tool));
		write(0,"\n", strlen("\n\n"));
		write(0,object, strlen(object));
		write(0,"\n", strlen("\n\n"));
		write(0,npc, strlen(npc));
		write(0,"\n", strlen("\n\n"));
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

			
			if(verif(argv[1]))
			{
				DIR *d =opendir(argv[1]);
				const char* home = getcwd(NULL, 0);
				if(d)
				{
					chdir(argv[1]);
					
					while((dir =readdir(d)) !=NULL)
					{
						stat(dir->d_name,buf);
						if (S_ISDIR(buf->st_mode))
						{
							if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~')
							{
								strcat(room,"  ");
								strcat(room,dir->d_name);
								strcat(room,"\n");
							}
						}
						else
						{
							if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~'){
								int info=extension(dir->d_name);
								switch(info)
								{
								case 1:
									deletextension(dir->d_name);
									strcat(tool,"  ");
									strcat(tool,dir->d_name);
									strcat(tool,"\n");
									break;
								case 2:
									deletextension(dir->d_name);
									strcat(object,"  ");
									strcat(object,dir->d_name);
									strcat(object,"\n");
									break;
								case 3:
									deletextension(dir->d_name);
									strcat(npc,"  ");
									strcat(npc,dir->d_name);
									strcat(npc,"\n");
									break;
								}
								
								
							}
						}
						
					}
					write(0,room, strlen(room));
					write(0,"\n", strlen("\n\n"));
					write(0,tool, strlen(tool));
					write(0,"\n", strlen("\n\n"));
					write(0,object, strlen(object));
					write(0,"\n", strlen("\n\n"));
					write(0,npc, strlen(npc));
					write(0,"\n", strlen("\n\n"));
					
					chdir(home);
					closedir(d);
				}
				else
				{
					
					write(1,"this room doesn't exist\n", strlen("this room doesn't exist\n"));
				}
			}
			
			
		}
		break;
	case 3:;
		if(verif(argv[1]) && verif(argv[2]))
		{
			if(strcmp(argv[1],"-r")==0 || strcmp(argv[1],"-room")==0)
			{					
				
				const char* home = getcwd(NULL, 0);
				DIR *d =opendir(argv[2]);
				if(d)
				{
					chdir(argv[2]);
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
					chdir(home);
					closedir(d);
					
				}
				else
				{

					write(1,"this room doesn't exist\n", strlen("this room doesn't exist\n"));
				}
			}
			else if (strcmp(argv[2],"-r")==0 || strcmp(argv[2],"-room")==0)
			{
				const char* home = getcwd(NULL, 0);
				DIR *d =opendir(argv[1]);
				if(d)
				{
					chdir(argv[1]);
					while((dir =readdir(d)) !=NULL)
					{
						stat(dir->d_name,buf);
						if(S_ISREG(buf->st_mode))
						{
							break;
						}
						else if (S_ISDIR(buf->st_mode))
						{
							
							if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~')
							{
								write(0,dir->d_name, strlen(dir->d_name));
								write(0,"\n", strlen("\n"));
							}
							
							
						}
					}
					chdir(home);
					closedir(d);
					
				}
				else
				{
					write(1,"this room doesn't exist\n", strlen("this room doesn't exist\n"));
				}
			}
			else
			{
				DIR *d =opendir(argv[1]);
				const char* home = getcwd(NULL, 0);
				if(d)
				{
					chdir(argv[1]);
					while((dir =readdir(d)) !=NULL)
					{
						stat(dir->d_name,buf);
						if (S_ISDIR(buf->st_mode))
						{
							if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~')
							{
								strcat(room,"  ");
								strcat(room,dir->d_name);
								strcat(room,"\n");
							}
						}
						else
						{
							if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~'){
								int info=extension(dir->d_name);
								switch(info)
								{
								case 1:
									deletextension(dir->d_name);
									strcat(tool,"  ");
									strcat(tool,dir->d_name);
									strcat(tool,"\n");
									break;
								case 2:
									deletextension(dir->d_name);
									strcat(object,"  ");
									strcat(object,dir->d_name);
									strcat(object,"\n");
									break;
								case 3:
									deletextension(dir->d_name);
									strcat(npc,"  ");
									strcat(npc,dir->d_name);
									strcat(npc,"\n");
									break;
								}
								
								
							}
						}
						
					}
					write(0,room, strlen(room));
					write(0,"\n", strlen("\n\n"));
					write(0,tool, strlen(tool));
					write(0,"\n", strlen("\n\n"));
					write(0,object, strlen(object));
					write(0,"\n", strlen("\n\n"));
					write(0,npc, strlen(npc));
					write(0,"\n", strlen("\n\n"));
					
					chdir(home);
					closedir(d);
				}
				else
				{
					
					write(1,"this room doesn't exist\n", strlen("this room doesn't exist\n"));
				}
			}
			
			
		}
		break;
	}
	free(buf);
	return 0;
	
}



