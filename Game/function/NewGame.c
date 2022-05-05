
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
#include <sys/wait.h>
#include "NewGame.h"




void NewGame()
{
	int chmod=fork();
	if(chmod==0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom","0066");
	}
	else
	{
		wait(NULL);
	}
	
	int chmod2=fork();
	if(chmod2==0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/Parking/Basement","0066");
	}
	else
	{
		wait(NULL);
	}
	
	int chmod3=fork();
	if(chmod3==0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice","0066");	
	}
	else
	{
		wait(NULL);
	}
	
	int chmod4=fork();
	if(chmod4==0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/JanitorRoom","0066");
	}
	else
	{
		wait(NULL);
	}
	
	int chmod5=fork();
	if(chmod5==0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2","0066");
	}
	else
	{
		wait(NULL);
	}
	
	int chmod6=fork();
	if(chmod6==0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1","0066");

	}
	else
	{
		wait(NULL);
	}
	int chmod7=fork();
	if(chmod7==0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom","0066");
	}
	else
	{
		wait(NULL);
	}
	
}
