// Display the monitors
int checkMonitors() {
	// Non-hacked monitors
	if (getObjStateInRoom("monitors") == 0)
		system("cat ../../../../../../assets/monitors-ascii.txt");
	else // hacked monitors
		system("cat ../../../../../../assets/hacked-monitors-ascii.txt");

	return 0;
}
