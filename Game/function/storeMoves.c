#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "storeMoves.h"
#include "../defines.h"

/**
 * Function: storeMoves
 * --------------------
 * Stores all the commands introduced by the user in a file
 */
int storeMoves(char *root, char* argv[])
{
	int i = 0;
	char *movesPath = malloc(PATH_MAX);
	int fd;

	strcpy(movesPath, root);
	strcat(movesPath, "/../moves.txt");
 	fd =  open(movesPath, O_RDWR|O_CREAT, 0666);

	lseek(fd, 0, SEEK_END);

	while(argv[i] != NULL)
	{
      		write(fd, argv[i], strlen(argv[i]));
      		write(fd, " ", 1);
	  	i++;
	}

	write(fd, "\n", 1);
	close(fd);
	free(movesPath);
	return 0;

}


