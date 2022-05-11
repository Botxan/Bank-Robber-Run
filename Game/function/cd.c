#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cd.h"

int verif1(char argv[])
{
	
	char test[2]="/";
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

int verif2(char argv[],char home[])
{
	if(strcoll(argv,home)>=0)
	{
		
		return 1;
	}
	return 0;
}

#ifdef FUNCTION
int cd(int argc,char *argv[],char home[],int verif){
#else
int main(int argc,char *argv[])
{
int verif=1;
char home[]="";
#endif
	
	if (argc > 2) {
			printf("%s: Too many operands \nUsage: %s <pathname>\n", (char *) argv[0], (char *) argv[0]);
			return 0;
	}
	if(argc == 2) {
		//printf("argc is 2\n");
		
		char* homemain = getcwd(NULL, 0);
		
		if((verif==1) ||verif1(argv[1]))
		{
		int i = chdir(argv[1]);
		
		char* home1 = getcwd(NULL, 0);
		
		if((verif==1) || verif2(home1,home) )
		{
		
		if(i < 0)
		{
			FILE *fp=malloc(sizeof(FILE));
			char format[100]="";
			
			//char *part2=strcpy(strcat(homemain,"/"),argv[1]);
			
			fp = fopen(argv[1], "r");
			
			if(fp==NULL)
			{
				printf("You need to use object for open this door or This room doen't exist\n");	
				
				return 0;
			}
			else
			{
				fgets(format,100,fp);
				int t=chdir(format);
				if(t>=0)
				{
					printf("Room changed\n");
				}
				else
				{
					write(1,"You need to use object for open this door\n",strlen("You need to use object for open this door\n"));
				}
				
				fclose(fp);
				free(fp);
				return 1;
			}
				
			
		}
		else{
			#ifdef FUNCTION
			printf("Room changed\n");
			#else
			printf("PATH=%s",home1);
			#endif
			//printf("home = %s\n", home1);
			//printf("%d\n",strcmp(home,home1));
			return 1;
		}
		}
		else
		{
			printf("You can't exit of the game like that.\n");
			chdir(homemain);
			return 0;
		}
		}
}	
	return 0;
}


