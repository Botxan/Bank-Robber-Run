#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char * Prompt=strrchr(getcwd(NULL, 0),'/')+1;
	write(0,"You are in this room : ",strlen("You are in this room : "));
	write(0,Prompt, strlen(Prompt));
	write(0,"\n",strlen("\n"));
}