
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
	int chmod1=fork();
	if(chmod1==0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/Parking","0066");
	}
	else
	{
		wait(NULL);
	}
	
}
