/*
 * Function: checkLaptop
 * ---------------------
 * Displays the laptop of the officer 2
 */
int checkLaptop() {
	char line[256];
	char opt;
	system("cat ../../../../../../assets/laptop-ascii.txt");

	while (1) {
		printf("What do you want to check?\n\n");
		printf("A: MySQL Database\n");
		printf("B: Email\n");
		printf("C: Pass.txt\n");
		printf("D: Exit\n");

		fgets(line, sizeof(line), stdin);
		opt = line[0];

		switch(opt) {
			case 'A': // DB
				printf("Password: ");
				fgets(line, sizeof(line), stdin);
				if (strcmp(line, "8522\n") == 0) {
					printf("Correct password.\n");
					printf("Displaying database...\n");
					system("cat ../../../../../../assets/database-ascii.txt");
				} else printf("Incorrect password\n");
				break;
			case 'B': // Email
				printf("There is only one email in the inbox.\n\n");
				printf("To: Office\nSubject: Lost office key card.\n\nHello, the employee with the name Anton, please stop by the lost and found department when you can so that you can recover your office access card.\n\nLost and Found department.\n\n");

				// Change veronica state in lost and found so player can get the office key card
				setObjStateInRoom("laptop", 2);
				break;
			case 'C': // Pass.txt
				printf("VGhlIHBhc3N3b3JkIGZvciB0aGUgZGF0YWJhc2UgaXM6IGVpZ2h0IGZpdmUgdHdvIHR3bw==\n\n");
				break;
			case 'D': // Exit
				return 0;
		}
	}


	return 0;
}
