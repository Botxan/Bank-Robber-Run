#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

int cd(int argc,char *argv[],char home[]){
	
	if (argc > 2) {
			printf("%s: Too many operands \nUsage: %s <pathname>\n", (char *) argv[0], (char *) argv[0]);
			exit(1);
	}
	if(argc == 2) {
		//printf("argc is 2\n");
		char* homemain = getcwd(NULL, 0);
		if(verif1(argv[1]))
		{
		
		int i = chdir(argv[1]);
		
		char* home1 = getcwd(NULL, 0);
		if(verif2(home1,home))
		{
		
		if(i < 0)
		{
			printf("Room couldn't be changed\n");
			return 0;
		}
		else{
			printf("Room changed\n");
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


