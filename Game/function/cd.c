#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){

	int result = 0;

	if (argc > 2) {
			printf("%s: Too many operands \nUsage: %s <pathname>\n", (char *) argv[0], (char *) argv[0]);
			exit(1);
}
	if(argc == 2) {
			printf("argc is 2\n");
			int i = chdir(argv[1]);
			const char* home = getcwd(NULL, 0);
		if(i < 0)
			printf("directory couldn't be changed\n");
		else{
			printf("directory changed\n");
			printf("home = %s\n", home);
}
		exit(0);
}	
	return 0;
}