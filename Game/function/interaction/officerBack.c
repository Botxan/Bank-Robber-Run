// Special interaction when player leaves office1 for first time
// and the officer is coming back
char officerBack() {
	char line[256];
	char opt;

	printf("WARNING! You hear footsteps outside... The officer is coming back, you have to hide!!!\n");
	while(1) {
		printf("Where are you hiding?\n\n");
		printf("A: Inside the cabinet\n");
		printf("B: Behind the curtains\n");
		printf("C: Wait behind the door and try to knock out the employee\n");
		fgets(line, sizeof(line), stdin);
		opt = line[0];
		if (opt == 'A' || opt == 'B' || opt == 'C') break;
	}

	return opt;
}
