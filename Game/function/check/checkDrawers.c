#include "../../defines.h"

// Check desk in office 1
int checkDrawers(char *root, int checkedTimes) {

	if (checkedTimes > 1) {
		printf("Office supplies, not much else.\n");
		return 0;
	}

	char commandPath[PATH_MAX];
	char laxativesPath[PATH_MAX];

	sprintf(laxativesPath, "%s/../assets/tool/laxatives.tool", root);
	sprintf(commandPath, "%s/../pickup", root);
	// Create laxatives symlink
        if (fork() == 0) {
                // create the laxatives symlink and pickup
                symlink(laxativesPath, "laxatives.tool");
		execlp(commandPath, "pickup", "laxatives", root, NULL);
		if (errno != 0) {
			printf("Error picking up laxatives: %s\n", strerror(errno));
			return -1;
		}
        } else wait(NULL);

	return 0;
}
