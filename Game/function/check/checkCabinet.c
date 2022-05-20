
// Check cabinet in office 1 => find employee suit => switch current skin
int checkCabinet() {
	char opt;
	int electrician = -1; // 0 => electrician, -1 => executive

	while (1) {
		// Get player's current skin
	        electrician = access("../../../../../Inv/electrician.skin", R_OK);


		if (electrician == 0) printf("There are a few executive suits.\nChange current skin?\n");
		else printf("There is the electrician suit you left before.\nChange current skin?\n");

		printf("Y: Yes.\n");
		printf("N: No.\n");
		scanf(" %[^\n]%*c", &opt);

		switch(opt) {
			case 'Y':
				// Unlink the current skin
				if (electrician == 0) {
					unlink("../../../../../Inv/electrician.skin");
					symlink("../../assets/skin/executive.skin", "../../../../../Inv/executive.skin");
				} else {
					unlink("../../../../../Inv/executive.skin");
					symlink("../../assets/skin/electrician.skin", "../../../../../Inv/electrician.skin");
				}
				printf("[*] Skin changed [*]\n");
				break;
			case 'N':
				return 0;
		}
	}

	return 0;
}
