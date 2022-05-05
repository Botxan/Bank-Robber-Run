/**********************************************************
 * defines.h
 **********************************************************/

/* ------------ Files ------------ */
#define MAINMENU "main-menu.txt"


/* ---------- Main menu ---------- */
#define NEW_GAME 1
#define LOAD_GAME 2
#define OPTIONS 3
#define QUIT 4

/* ------------ Rooms ------------ */
#define VAN 0
#define ENTRANCE 1 	// Main entrance
#define HALL 2 		// Main banking hall
#define LANDF 3 	// Lost and Found department
#define ELECPANEL 4 	// Electrical panel room
#define VENT 5		// Ventilation ducts
#define CORRIDOR 6
#define OFF1 7 		// Office 1
#define OFF2 8 		// Office 2
#define SEC 9 		// Security room
#define WC 10
#define JANITOR 11 	// Janitor's room
#define BOSS 12 	// Boss office
#define ROOF 13
#define PARKING 14
#define BASEMENT 15
#define VAULTR 16	// Vault room
#define VAULTC 17	// Vault corridor
#define VAULT 18


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



