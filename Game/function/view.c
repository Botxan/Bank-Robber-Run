
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
#include "cd.h"




int verif(char argv[])
{
	char test[2]="/.";
	for(size_t i=0;i<strlen(argv);i++)
		for(size_t t=0;t<strlen(test);t++)
			if(argv[i]==test[t])
			{
				write(1,"\033[31mYou can't use '/'.\n\n\033[37m", strlen("\033[31mYou can't use '/'.\n\n\033[37m"));
				return 0;
			}
	return 1;
}

int extension(char argv[])
{
	if(strcmp(strrchr(argv, '.'),".tool")==0) return 1;
	if(strcmp(strrchr(argv, '.'),".obj")==0) return 2;
	if(strcmp(strrchr(argv, '.'),".npc")==0) return 3;
	return 0;
}

int verifcd(char argv[])
{
	if(strcmp(argv,"..")==0)
	{
		char *home1=getcwd(NULL, 0);
		chdir("Directories/Van");
		char *main1=getcwd(NULL, 0);
		chdir(home1);
		if(strcoll(home1,main1)>=0)
		{
			return 1;
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
	char room[100]="Places:\n";
	char tool[100]="Tools:\n";
	char object[100]="Objects:\n";
	char npc[100]="People:\n";

	switch(argc)
	{
	case 1:
	{
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
						default:
							strcat(room,"  ");
							strcat(room,dir->d_name);
							strcat(room,"\n");
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
	}
	case 2:
	{
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
				else
				{
					if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~'){
						int info=extension(dir->d_name);
						switch(info)
						{
						case 1:
							break;
						case 2:
							break;
						case 3:
							break;
						default:
							write(0,dir->d_name, strlen(dir->d_name));
							write(0,"\n", strlen("\n"));
							break;
						}
					}
				}
			}
			closedir(d);
		}
		else
		{
			int result=1;
			if(verifcd(argv[1])||verif(argv[1]))
			{
				DIR *d =opendir(argv[1]);
				char* home = getcwd(NULL, 0);
				char *part1=strcpy(strcat(home,"/"),argv[1]);
				result = access (part1, R_OK);

				if(!d && result==0)
				{
					FILE *fp;
					char format[200]="";
					fp = fopen(part1, "r");

					if(fp==NULL)
					{
						printf("\033[31mIt's not a directory\n\033[37m");
						return 0;
					}
					else
					{
						fgets(format,200,fp);
						chdir(format);
						DIR *d =opendir(".");
						fclose(fp);
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
										default:
											strcat(room,"  ");
											strcat(room,dir->d_name);
											strcat(room,"\n");
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
				}
				else if(d && result==0 )
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
								default:
									strcat(room,"  ");
									strcat(room,dir->d_name);
									strcat(room,"\n");
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
					write(1,"\033[31mYou need to use some object to open this door or this room doesn't exist \n\033[37m", strlen("\033[31mYou need to use some object to open this door or this room doesn't exist \n\n\033[37m"));
				}
			}
		}
		break;
	}
	case 3:
	{
		int result=1;
		if(strcmp(argv[1],"-r")==0 || strcmp(argv[1],"-room")==0)
		{
			if(verifcd(argv[1]) || verif(argv[1]))
			{
				char* home = getcwd(NULL, 0);
				DIR *d =opendir(argv[2]);
				char *part1=strcpy(strcat(home,"/"),argv[2]);
				result = access (part1, R_OK);

				if(!d && result==0)
				{
					FILE *fp;
					char format[200]="";
					fp = fopen(part1, "r");

					if(fp==NULL)
					{
						printf("\033[31mIt's not a directory\n\033[37m");
						return 0;
					}
					else
					{
						fgets(format,200,fp);
						chdir(format);
						DIR *d =opendir(".");
						fclose(fp);
						while((dir =readdir(d)) !=NULL)
						{
							stat(dir->d_name,buf);
							if(S_ISREG(buf->st_mode))
							{
								if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~'){
									int info=extension(dir->d_name);
									switch(info)
										{
										case 1:
											break;
										case 2:
											break;
										case 3:
											break;
										default:
											write(0,dir->d_name, strlen(dir->d_name));
											write(0,"\n", strlen("\n"));
											break;
									}
								}
							}
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
				}
				else if(d && result ==0)
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
					write(1,"\033[31mYou need to use some object to open this door or this room doesn't exist \n\033[37m", strlen("\033[31mYou need to use some object to open this door or this room doesn't exist \n\n\033[37m"));
				}
			}
		}
		else if (strcmp(argv[2],"-r")==0 || strcmp(argv[2],"-room")==0)
		{
			if(verifcd(argv[1]) || verif(argv[1]))
			{
				char* home = getcwd(NULL, 0);
				DIR *d =opendir(argv[1]);
				char *part1=strcpy(strcat(home,"/"),argv[1]);
				result = access (part1, R_OK);
				if(!d && result==0)
				{
					FILE *fp;
					char format[200]="";
					fp = fopen(part1, "r");
					if(fp==NULL)
					{
						printf("\033[31mIt's not a directory\n\033[37m");
						return 0;
					}
					else
					{
						fgets(format,200,fp);
						chdir(format);
						DIR *d =opendir(".");
						fclose(fp);
						while((dir =readdir(d)) !=NULL)
						{
							stat(dir->d_name,buf);
							if(S_ISREG(buf->st_mode))
							{
								if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~'){
									int info=extension(dir->d_name);
									switch(info)
									{
										case 1:
											break;
										case 2:
											break;
										case 3:
											break;
										default:
											write(0,dir->d_name, strlen(dir->d_name));
											write(0,"\n", strlen("\n"));
											break;
									}
								}
							}
							if (S_ISDIR(buf->st_mode))
							{
								if (dir->d_name[0] != '.' && dir->d_name[strlen(dir->d_name)-1] != '~')
								{
									write(0,dir->d_name, strlen(dir->d_name));
									write(0,"\n", strlen("\n"));
								}
							}
							chdir(home);
							closedir(d);
						}
					}
				}
				else if(d && result ==0)
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
					write(1,"\033[31mYou need to use some object to open this door or this room doesn't exist \n\033[37m", strlen("\033[31mYou need to use some object to open this door or this room doesn't exist \n\n\033[37m"));
				}
			}
		}
		else
		{
			int result=1;
			if(verifcd(argv[1]) || verif(argv[1]))
			{
				DIR *d =opendir(argv[1]);
				char* home = getcwd(NULL, 0);
				char *part1=strcpy(strcat(home,"/"),argv[1]);
				result = access (part1, R_OK);

				if(!d && result==0)
				{
					FILE *fp;
					char format[100]="";
					fp = fopen(part1, "r");

					if(fp==NULL)
					{
						printf("\033[31mIt's not a directory\n\033[37m");
						return 0;
					}
					else
					{
						fgets(format,100,fp);
						chdir(format);
						DIR *d =opendir(".");
						fclose(fp);
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
									default:
										strcat(room,"  ");
										strcat(room,dir->d_name);
										strcat(room,"\n");
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
				}
				else if(d && result ==0)
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
								default:
									strcat(room,"  ");
									strcat(room,dir->d_name);
									strcat(room,"\n");
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
				} else write(1,"\033[31mYou need to use some object to open this door or this room doesn't exist \n\033[37m", strlen("\033[31mYou need to use some object to open this door or this room doesn't exist \n\n\033[37m"));
			}
		}
		break;
	}
	}

	free(buf);
	return 0;
}



