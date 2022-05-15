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


#ifdef FUNCTION
#include "chmod.h"
int chmodfunction(int argc,char *argv[])
#else
int main(int argc,char *argv[])
#endif
{
if(argc <3 && argc>4)
{
	write(1,"You need to use three argument (path of the file,permission)\n", strlen("You need to use three argument (path of the file,permission)\n"));
}else{
	//const char *perm = "rwxrwxrwx";
	char *ptr;
	strtol(argv[2],&ptr, 10);
	if (strlen(ptr)==0)
	{
		
		int i;
        i = strtol(argv[2], 0, 8);
		if(chmod(argv[1],i)<0)
		{
			//write(1,"The permission of the file has not changed.\n", strlen("The permission of the file has not changed.\n"));
			return 0;
		}
		else
		{
			//write(0,"The permission of the file has changed.\n", strlen("The permission of the file has changed.\n"));
			return 1;
		}
		
	}
	else
	{
		const char *perm = argv[2];
	mode_t mode = (((perm[0] == 'r') * 4 | (perm[1] == 'w') * 2 | (perm[2] == 'x')) << 6) |
              (((perm[3] == 'r') * 4 | (perm[4] == 'w') * 2 | (perm[5] == 'x')) << 3) |
              (((perm[6] == 'r') * 4 | (perm[7] == 'w') * 2 | (perm[8] == 'x')));
	if(chmod(argv[1],mode)<0)
		{
			//write(1,"The permission of the file has not changed.\n", strlen("The permission of the file has not changed.\n"));
			return 0;
		}
		else
		{
			//write(0,"The permission of the file has changed.\n", strlen("The permission of the file has changed.\n"));
			return 1;
		}
	}
	
}

return 1;
}
