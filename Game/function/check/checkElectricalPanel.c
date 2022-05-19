#include <errno.h>
#include <sys/wait.h>

#include "../util.c"

/**
 * Auxiliar function to change a pixel
 *
 */
void p(int fd, int absOffset, char *symbol) {
	lseek(fd, absOffset, SEEK_SET);
	write(fd, symbol, 1);
	lseek(fd, 0, SEEK_SET);
}

/**
 * Switches the state of the given switcher
 */
void switcher(char switcher, int oldState) {
	int offset = 0;
	int isOn = -1;
	int fd = open("../../../../../assets/electrical-panel-ascii.txt", O_RDWR);

        if (fd == -1) {
                write(2, "electrical-panel-ascii.txt not found.\n", strlen("electrical-panel-ascii.txt not found.\n"));
                exit(1);
        }

	switch(switcher) {
		case 'A': // Main hall
			offset = 4;
			if ((oldState & 2) == 2) isOn = 1;
			else isOn = 0;
			setObjStateInRoom("electrical-panel", oldState ^ 2);
			break;
		case 'D': // Office 2
			offset = 43;
			if ((oldState & 1) == 1) isOn = 1;
			else isOn = 0;
			setObjStateInRoom("electrical-panel", oldState ^ 1);
			break;
	}

	if (isOn) {
		p(fd, offset+115, "_");
                p(fd, offset+116, "_");

                p(fd, offset+169, "*");
                p(fd, offset+170, "-");
                p(fd, offset+171, "-");
                p(fd, offset+172, "*");
                p(fd, offset+173, "|");

                p(fd, offset+224, "|");
                p(fd, offset+226, " ");
                p(fd, offset+227, "|");
                p(fd, offset+228, "|");

                p(fd, offset+280, "_");
                p(fd, offset+282, "|");

                p(fd, offset+335, "\\");
                p(fd, offset+337, "\\");

                p(fd, offset+389, "\\");
                p(fd, offset+390, " ");
                p(fd, offset+391, "\\");
                p(fd, offset+392, "_");
                p(fd, offset+393, "\\");

                p(fd, offset+445, "\\");
                p(fd, offset+446, "[");
                p(fd, offset+448, "]");
	} else {
		p(fd, offset+115, " ");
                p(fd, offset+116, " ");

                p(fd, offset+169, " ");
                p(fd, offset+170, "/");
                p(fd, offset+171, "[");
                p(fd, offset+172, "_");
                p(fd, offset+173, "]");

                p(fd, offset+224, "/");
                p(fd, offset+226, "/");
                p(fd, offset+227, " ");
                p(fd, offset+228, "/");

                p(fd, offset+280, "/");
                p(fd, offset+282, "/");

                p(fd, offset+335, " ");
                p(fd, offset+337, "|");

                p(fd, offset+389, "|");
                p(fd, offset+390, "_");
                p(fd, offset+391, "_");
                p(fd, offset+392, "|");
                p(fd, offset+393, "|");

                p(fd, offset+445, "_");
                p(fd, offset+446, "_");
                p(fd, offset+448, "|");
	}

}
/*
 * Function: checkElectricalPanel
 * -------------
 * Displays and changes the electrical panel depending on user input.
 * Note 1: this function assumes that the player is in the electrical panel room.
 * Note 2: the state of the electrical is binary
 * 	0 => 00 => mbh and office2 off
 * 	1 => 01 => mbh off office2 on
 * 	2 => 10 => mbh mbh on and office 2 on
 *	3 => 11 => mbh and office 2 on
 */
int checkElectricalPanel() {
	char opt;
	int oldState;

	// Open the electrical panel picture
	if (fork() == 0) {
		execlp("/bin/cat", "cat", "../../../../../assets/electrical-panel-ascii.txt", NULL);
		if (errno != 0) printf("Error on talk function: %s\n", strerror(errno));
	} else wait(NULL);

	printf("CAUTION! Do not touch the switches unless strictly necessary.\n\n");
	printf("A: Switch main banking hall.\n");
	printf("B: Switch Corridor.\n");
	printf("C: Switch Office 1.\n");
	printf("D: Switch Office 2.\n");
	printf("E: Switch W.C.\n");
	printf("F: Switch Security.\n");
	printf("G: Switch Boss office.\n");
	printf("H: Janitors room.\n");
	printf("I: Exit.\n");

	while(1) {
		scanf(" %[^\n]%*c", &opt);
		if (opt == 'A' || opt == 'D' || opt == 'I') break;
		if (opt == 'B' || opt == 'C' || opt == 'E' || opt == 'F' || opt == 'G' || opt == 'H')
			printf("The switch is locked.\n");
	}

	// Switch main banking hall
	oldState = getObjStateInRoom("electrical-panel");
	if (opt == 'A') switcher('A', oldState);
	if (opt == 'D') switcher('D', oldState);

	// Display panel until quit
	if (opt != 'I') checkElectricalPanel();

	return 0;
}
