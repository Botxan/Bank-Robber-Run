int checkVault() {
	system("gpg-connect-agent reloadagent /bye &>/dev/null"); // Otherwise prompt may not be display
	system("gpg ../../../../../../../assets/vault.txt.gpg &>/dev/null");
	if (access("../../../../../../../assets/vault.txt", R_OK) == 0) {
		printf("\x1b[32m*Correct password*\x1b[0m\n");
		printf("\x1b[32m*You have obtained thesecret*\x1b[0m\n");
		symlink("../../assets/vault.txt", "../../../../../../Inv/thesecret.tool");
		system("echo \"The secret of passing IOS subject will be discovered in the next episode of Bank Robber Run!\" > ../../../../../../Inv/thesecret.tool");
	} else{
		printf("\x1b[31m*Incorrect password*\x1b[0m\n");
	}
	return 0;
}
