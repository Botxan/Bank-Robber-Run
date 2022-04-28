/**********************************************************
 * defines.h
 **********************************************************/

/* ------------ Rooms ------------ */
#define VAN 0
#define ENTRANCE 1 	// Main entrance
#define HALL 2 		// Main banking hall
#define LANDF 3 	// Lost and Found department
#define ELECPANEL 4 	// Electrical panel room
#define CORRIDOR 5
#define OFF1 6 		// Office 1
#define OFF2 7 		// Office 2
#define SEC 8 		// Security room
#define WC 9
#define JANITOR 10 	// Janitor's room
#define BOSS 11 	// Boss office
#define ROOF 12
#define PARKING 13
#define BASEMENT 14
#define VAULTR 15	// Vault room
#define VAULT 16


/* ------------ Skins ------------ */
#define SUIT 0
#define ELEC 1


/* ------------ Others ----------- */
#define errors(a) { perror(a); exit(1); }
#define MAXLINE 200
#define MAXARGS 20
#define NKEYS 4


/* ------ Types and structs ------ */
typedef struct {
	char *name;
	int id;
} idStruct;



