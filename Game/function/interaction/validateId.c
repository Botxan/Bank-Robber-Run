/*
 * Returns 0 if the introduced id is correct (4015)
 */
int validateId() {
	char line[256];
	fgets(line, sizeof(line), stdin);
	return strcmp(line, "4015\n");
}
