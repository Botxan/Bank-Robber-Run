
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define PATH "./assets/Man/"

int main(int argc,char *argv[]) {
	int ch =0;
	if(argc ==1)
	{
		if(strcmp(argv[0], "access") == 0 || strcmp(argv[0], "cd") == 0)
		{
			ch=1;
		}
		if(strcmp(argv[0], "view") == 0 || strcmp(argv[0], "ls") == 0)
		{
			ch=2;
		}
		if(strcmp(argv[0], "inv") == 0 || strcmp(argv[0], "inventory") == 0)
		{
			ch=3;
		}
		if(strcmp(argv[0], "man") == 0)
		{
			ch=4;
		}
		if(strcmp(argv[0], "pickup") == 0 || strcmp(argv[0], "pu") == 0)
		{
			ch=5;
		}
		if(strcmp(argv[0], "talk") == 0)
		{
			ch=6;
		}
		if(strcmp(argv[0], "Pause") == 0 || strcmp(argv[0], "P") == 0|| strcmp(argv[0], "quit") == 0|| strcmp(argv[0], "q") == 0 || strcmp(argv[0], "leave") == 0)
		{
			ch=7;
		}
		if (strcmp(argv[0], "Pwd") == 0 || strcmp(argv[0], "pwd") == 0 || strcmp(argv[0], "Room") == 0 || strcmp(argv[0], "r") == 0|| strcmp(argv[0], "R") == 0 || strcmp(argv[0], "room") == 0 )
		{
			ch=8;
		}
		if (strcmp(argv[0], "use") == 0)
		{
			ch=9;
		}
		if (strcmp(argv[0], "map") == 0)
		{
			ch=10;
		}
		if (strcmp(argv[0], "Time") == 0 || strcmp(argv[0], "time")==0)
		{
			ch=11;
		}
		if (strcmp(argv[0], "check") == 0)
		{
			ch=12;
		}
		if (strcmp(argv[0], "log") == 0)
		{
			ch=13;
		}
		else if (ch==0)
		{
			ch=15;
		}
	}
	else
	{


		printf("Welcome to the man pages!\n");
		printf("-------------------------\n");
		printf("Select the man page you want to open: \n");
		printf("1: cd\n");
		printf("2: ls\n");
		printf("3: inv\n");
		printf("4: man\n");
		printf("5: pickup\n");
		printf("6: talk\n");
		printf("7: Pause\n");
		printf("8: Room\n");
		printf("9: use\n");
		printf("10: map\n");
		printf("11: Time\n");
		printf("12: check\n");
		printf("13: log\n");
		printf("14: Leave\n");
		printf("All letters: CANCEL\n");
		scanf("%d",&ch);
	}

	if (ch==1) {
		FILE* cd; //pointer to file
		int display;
		cd = fopen(PATH "Cd.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(cd); //read the file
			if (feof(cd)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(cd); //close file
		return 0;
	}
	else if (ch==2) {
		FILE* cp; //pointer to file
		int display;
		cp = fopen(PATH "ls.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(cp); //read the file
			if (feof(cp)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(cp); //close file
		return 0;
	}
	else if (ch==3) {
		FILE* grep; //pointer to file
		int display;
		grep = fopen(PATH "inv.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(grep); //read the file
			if (feof(grep)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(grep); //close file
		return 0;
	}
	else if (ch==4) {
		FILE* less; //pointer to file
		int display;
		less = fopen(PATH "man.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(less); //read the file
			if (feof(less)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(less); //close file
		return 0;
	}
	else if (ch==5) {
		FILE* ls; //pointer to file
		int display;
		ls = fopen(PATH "pickup.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(ls); //read the file
			if (feof(ls)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(ls); //close file
		return 0;
	}
	else if (ch==6) {
		FILE* man; //pointer to file
		int display;
		man = fopen(PATH "talk.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(man); //read the file
			if (feof(man)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(man); //close file
		return 0;
	}
	else if (ch==7 || ch==14) {
		FILE* mv; //pointer to file
		int display;
		mv = fopen(PATH "Pause.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(mv); //read the file
			if (feof(mv)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(mv); //close file
		return 0;
	}
	else if (ch==8) {
		FILE* pwd; //pointer to file
		int display;
		pwd = fopen(PATH "Room.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(pwd); //read the file
			if (feof(pwd)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(pwd); //close file
		return 0;
	}
	else if (ch==9) {
		FILE* touch; //pointer to file
		int display;
		touch = fopen(PATH "use.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(touch); //read the file
			if (feof(touch)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(touch); //close file
		return 0;
	}
	else if (ch==10) {
		FILE* touch; //pointer to file
		int display;
		touch = fopen(PATH "map.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(touch); //read the file
			if (feof(touch)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(touch); //close file
		return 0;
	}
	else if (ch==11) {
		FILE* touch; //pointer to file
		int display;
		touch = fopen(PATH "Time.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(touch); //read the file
			if (feof(touch)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(touch); //close file
		return 0;
	}
	else if (ch==12) {
		FILE* touch; //pointer to file
		int display;
		touch = fopen(PATH "check.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(touch); //read the file
			if (feof(touch)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(touch); //close file
		return 0;
	}
	else if (ch==13) {
		FILE* touch; //pointer to file
		int display;
		touch = fopen(PATH "log.l", "r"); //file access in read mode
		while (1) { //loop to extract characters
			display = fgetc(touch); //read the file
			if (feof(touch)) //end of file indicator
			break;
			printf("%c", display); //displaying content
		}
		fclose(touch); //close file
		return 0;
	}
	else if (ch==15) {
		printf("\033[31m\nInvalid input!\n\033[37m");
		return 0;
	}
	else if (ch==0) {
		return 0;
	}
	else {
		printf("\033[31m\nInvalid input! you need to choose a number between 0 and 13.\n\033[37m");
		main(argc,argv);
	}
	return 0;
}
