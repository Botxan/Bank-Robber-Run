
#include <stdio.h>
#include <unistd.h>
#define PATH "/users/alumnos/acaf/acaf0214/manpages/"

int main(void) {

  char ch;

      printf("Welcome to the man pages!\n");
      printf("-------------------------\n");
      printf("Select the man page you want to open: \n");
      printf("1: cd\n");
      printf("2: less\n");
      printf("3: man\n");
      printf("4: ls\n");
      printf("5: \n");
      printf("6: \n");
      printf("7: \n");
      printf("8: \n");
      printf("9: \n");
      printf("10: \n");
      printf("c: CANCEL\n");
      scanf("%c",&ch);
 
if (ch=='1') {
  FILE* cd; //pointer to file
  int display;
	cd = fopen(PATH "cd.8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(cd); //read the file
	if (feof(cd)) //end of file indicator
     break;
      printf("%c", display); //displaying content
  	}
fclose(cd); //close file
return 0;
  }
else if (ch=='2') {
  FILE* cp; //pointer to file
  int display;
	cp = fopen(PATH "less.8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(cp); //read the file
	if (feof(cp)) //end of file indicator
  break;
  printf("%c", display); //displaying content
  	}
fclose(cp); //close file
return 0;
  }
else if (ch=='3') {
  FILE* grep; //pointer to file
  int display;
	grep = fopen(PATH "man.8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(grep); //read the file
	if (feof(grep)) //end of file indicator
  break;
  printf("%c", display); //displaying content
  	}
fclose(grep); //close file
return 0;
  }
else if (ch=='4') {
  FILE* less; //pointer to file
  int display;
	less = fopen(PATH "ls.8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(less); //read the file
	if (feof(less)) //end of file indicator
  break;
  printf("%c", display); //displaying content
  	}
fclose(less); //close file
return 0;
  }
else if (ch=='5') {
  FILE* ls; //pointer to file
  int display;
	ls = fopen(PATH ".8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(ls); //read the file
	if (feof(ls)) //end of file indicator
  break;
  printf("%c", display); //displaying content
  	}
fclose(ls); //close file
return 0;
  }
else if (ch=='6') {
  FILE* man; //pointer to file
  int display;
	man = fopen(PATH ".8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(man); //read the file
	if (feof(man)) //end of file indicator
  break;
  printf("%c", display); //displaying content
  	}
fclose(man); //close file
return 0;
  }
else if (ch=='7') {
  FILE* mv; //pointer to file
  int display;
	mv = fopen(PATH ".8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(mv); //read the file
	if (feof(mv)) //end of file indicator
  break;
  printf("%c", display); //displaying content
  	}
fclose(mv); //close file
return 0;
  }
else if (ch=='8') {
  FILE* pwd; //pointer to file
  int display;
	pwd = fopen(PATH ".8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(pwd); //read the file
	if (feof(pwd)) //end of file indicator
  break;
  printf("%c", display); //displaying content
  	}
fclose(pwd); //close file
return 0;
  }
else if (ch=='9') {
  FILE* touch; //pointer to file
  int display;
	touch = fopen(PATH ".8", "r"); //file access in read mode
  	while (1) { //loop to extract characters
  	display = fgetc(touch); //read the file
	if (feof(touch)) //end of file indicator
  break;
  printf("%c", display); //displaying content
  	}
fclose(touch); //close file
return 0;
  }
else if (ch=='c' || ch=='C') {
return;
  }
else {
  printf("\nInvalid input! Please type a number between 1 and x for man pages or c to cancel.");
}
return 0;
}
