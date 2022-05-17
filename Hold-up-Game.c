#include <limits.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>


int main()
{
		chdir("Game");
		int child=fork();
		if(child==0)
		{
			char* path=strcat(getcwd(NULL, 0),"/myshell0");
			execlp(path,"",NULL);
			if (errno!=0) write(0, "Unknown error\n", strlen("Unknown error\n"));
		}
		else
		{
			wait(NULL);
		}
}