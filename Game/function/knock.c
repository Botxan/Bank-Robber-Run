/*
 * Function: knock
 * -------------
 * It simulates knocking on a door
 * @param argc (in) number of arguments
 * @param argv the arguments for the program:
        - (in) command to be executed (knock)
        - (in) target door
 */
int knock(int argc, int *argv[]) {
	// Print args
        for (int i = 0; i < argc; i++)
                printf("%d: %s\n", i, argv[i]);

	if (argc != 2) {
		write(2, "Usage: knock <door>\n", strlen("Usage: knock door>"));
		return 1;
	}

	char *door[strlen[argv[1]];

	if (access(door, R_OK) == -1) {
		write(2, "No such door exists at the current location.\n",strlen("No such door exists at the current location"));
		return 1;
	}

	// Change message depending on the door
	if (strcmp(door, "LostAndFound") == 0) {
		write("1", "??: Come in.", strlen("??: Come in."));
	} else if strcmp("door, "??: Sorry, I have a lot of work now, come back later!");


}
