#include <stdlib.h>

// Check computer in office 1
int checkComputer() {
	char opt;
	system("cat ../../../../../../assets/computer-ascii.txt");

	while(1) {
		printf("A: Read Jade's email.\n");
	        printf("B: Read Ignacio's email.\n");
	        printf("C: Read Mom's email.\n");
	        printf("D: Read Avast email.\n");
	        printf("E: Exit\n");
	        printf("---------------------------\n");

                scanf(" %[^\n]%*c", &opt);

		switch (opt) {
			case 'A':
				printf("Hello everyone,\n\nI remind you that tomorrow's meeting is postponed to Tuesday next week because I have been given an appointment for the colonoscopy for tomorrow at 10:00.\n\nHave a good afternoon my dears,\nJade\n\n");
				break;
			case 'B':
				printf("Hi Mat.\n\nAs soon as I finish signing some reports and send a couple of emails I'll stop by the coffee machine, come by and we'll talk for a while. I'll be taking a coffee.\n\nRegards,\nIngnacio, the director\n\n");
				break;
			case 'C':
				printf("Mat!!\n\nOnce again you have left your bed unmade! Do you think I can walk behind you with 35 years that you are already 35 years old?\n\nLet this be the last time Mat, next time I'll cut your internet cable!\n\nMom\n\nPS: you have anchovies for dinner.\n\n");
				break;
			case 'D':
				printf("Avast virus database has been updated!\n\n");
				break;
			case 'E':
				return 0;
		}
	}

	return 0;
}
