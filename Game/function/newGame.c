#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include "newGame.h"

/**
 * Function: newGame
 * -----------------
 * Initializes a new game by setting the correct permissions to each directory
 * and placing each object in its corresponding starting point
 */
void newGame()
{
	// Set default to 755
	if (fork() == 0)
        {
                execlp("find", "find", "./Directories", "-type", "d", "-exec", "chmod", "755", "{}", ";", NULL);
                printf("Error applying default permissions to some directory under ./Directories %s.\n", strerror(errno));
                exit(0);
        }
	else wait(NULL);


        // Clear map
        if (fork() == 0)
        {
                execlp("find", "find", "./Directories", "-type", "l", "!", "-iname", "*description.txt", "-delete", NULL);
                printf("Error emptying directories: %s\n", strerror(errno));
                exit(0);
        }
        else wait(NULL);

	// Add room descriptions (they are not deleted from scenario, but just in case)
	symlink("../../assets/roomDescription/VanDescription.txt", "Directories/Van/.description.txt");
	symlink("../../../assets/roomDescription/MainEntranceDescription.txt", "Directories/Van/MainEntrance/.description.txt");
	symlink("../../../../assets/roomDescription/ParkingDescription.txt", "Directories/Van/MainEntrance/Parking/.description.txt");
	symlink("../../../../../assets/roomDescription/BasementDescription.txt", "Directories/Van/MainEntrance/Parking/Basement/.description.txt");
	symlink("../../../../../../assets/roomDescription/VaultCorridorDescription.txt", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/.description.txt");
	symlink("../../../../../../../assets/roomDescription/VaultRoomDescription.txt", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom/.description.txt");
	symlink("../../../../../../../../assets/roomDescription/VaultDescription.txt", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom/Vault/.description.txt");
	symlink("../../../../assets/roomDescription/MainBankingHallDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/.description.txt");
	symlink("../../../../../assets/roomDescription/ElectricalPanelRoomDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/ElectricalPanelRoom/.description.txt");
	symlink("../../../../../assets/roomDescription/LostAndFoundDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/LostAndFound/.description.txt");
	symlink("../../../../../assets/roomDescription/CorridorDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/.description.txt");
	symlink("../../../../../../assets/roomDescription/Office1Description.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1/.description.txt");
	symlink("../../../../../../assets/roomDescription/Office2Description.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2/.description.txt");
	symlink("../../../../../../assets/roomDescription/WCDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/.description.txt");
	symlink("../../../../../../../assets/roomDescription/VentilationDuctsDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/VentilationDucts/.description.txt");
	symlink("../../../../../../assets/roomDescription/SecurityRoomDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom/.description.txt");
	symlink("../../../../../../assets/roomDescription/BossOfficeDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice/.description.txt");
	symlink("../../../../../../assets/roomDescription/JanitorRoomDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/JanitorRoom/.description.txt");
	symlink("../../../../../../assets/roomDescription/RooftopDescription.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Rooftop/.description.txt");
	// Prevent GitHub from removing Inv directory by adding a dummy .description file
	symlink("../../assets/roomDescription/InvDescription.txt", "Directories/Inv/.description.txt");


	// Add symlinks to previous room and shortcuts from ventilation room and from corridor to basement
	symlink("..", "Directories/Van/MainEntrance/Van");
	symlink("..", "Directories/Van/MainEntrance/Parking/MainEntrance");
	symlink("..", "Directories/Van/MainEntrance/Parking/Basement/Parking");
	symlink("..", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/Basement");
	symlink("..", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom/VaultCorridor");
	symlink("..", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom/Vault/VaultRoom");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/MainEntrance");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/ElectricalPanelRoom/MainBankingHall");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/LostAndFound/MainBankingHall");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/Corridor/MainBankingHall");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1/Corridor");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2/Corridor");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/Corridor");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom/Corridor");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice/Corridor");
	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Rooftop/Corridor");

	symlink("..", "Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/VentilationDucts/WC");
	symlink("../../../ElectricalPanelRoom", "Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/VentilationDucts/ElectricalPanelRoom");
	symlink("../../SecurityRoom", "Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/VentilationDucts/SecurityRoom");
	symlink("../../Parking/Basement", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Basement");


	// Add all the .obj to the map
        symlink("../../../../../assets/obj/electrical-panel.obj", "Directories/Van/MainEntrance/MainBankingHall/ElectricalPanelRoom/electrical-panel.obj");
        symlink("../../../../../assets/obj/electrical-panel.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/cofee-machine.obj");
        symlink("../../../../../../assets/obj/office1-computer.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1/computer.obj");
        symlink("../../../../../../assets/obj/office1-desk.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1/desk.obj");
        symlink("../../../../../../assets/obj/office1-drawers.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1/drawers.obj");
        symlink("../../../../../../assets/obj/office1-cabinet.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1/cabinet.obj");
        symlink("../../../../../../assets/obj/office2-computer.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2/computer.obj");
	symlink("../../../../../../assets/obj/monitors.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom/monitors.obj");
	symlink("../../../../../../assets/obj/janitorial-supplies.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/JanitorRoom/janitorial-supplies.obj");
        symlink("../../../../../../assets/obj/boss-computer.obj", "Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice/computer.obj");


	// Add all the .tools to the map (except the ones provided by npcs or other interactions)
	symlink("../../assets/tool/decoder.tool", "Directories/Inv/decoder.tool");
	symlink("../../assets/tool/lockpick.tool", "Directories/Inv/lockpick.tool");
	symlink("../../assets/tool/hacking-tool.tool", "Directories/Inv/hacking-tool.tool");
	symlink("../../assets/tool/radio.tool", "Directories/Inv/radio.tool");
	symlink("../../../../../../assets/tool/telephone.tool", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2/telephone.tool");
	symlink("../../../../../../assets/tool/boss-picture.tool", "Directories/Van/MainEntrance/MainBankingHall/Corridor/JanitorRoom/boss-picture.tool");
	symlink("../../../../../../assets/tool/night-vision-googles.tool", "Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom/night-vision-googles.tool");


	// Add all the .npc to the map
	symlink("../../assets/npc/Robert.npc", "Directories/Van/Robert.npc");
	symlink("../../../assets/npc/Edurne.npc", "Directories/Van/MainEntrance/Edurne.npc");
	symlink("../../../../assets/npc/Ramon.npc", "Directories/Van/MainEntrance/MainBankingHall/Ramon.npc");
	symlink("../../../../../assets/npc/Matt.npc", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Matt.npc");
	symlink("../../../../../../assets/npc/Jade.npc", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2/Jade.npc");
	symlink("../../../../../../assets/npc/Javier.npc", "Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom/Javier.npc");
	symlink("../../../../../assets/npc/Veronica.npc", "Directories/Van/MainEntrance/MainBankingHall/LostAndFound/Veronica.npc");
	symlink("../../../../../../assets/npc/Julian.npc", "Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice/Ignacio.npc");


	// If ventilation ducts have been discovered in previous game, remove symlink
        unlink("./Directories/Van/MainEntrance/MainBankingHall/ElectricalPanelRoom/VentilationDucts");


	// Set npc's default status to 0
	DIR *d = opendir("assets/npc/");
	struct dirent *dir;
	int fd;
	char filePath[30]; // asets/npc/xxxxxxx.npc

	if (!d) {
		write(1, "Directory assets/npc/ not found.\n", strlen("Directory assets/npc/ not found.\n"));
		exit(1);
	}
	while ((dir = readdir(d)) != NULL) {
		if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
			strncpy(filePath, "assets/npc/", 30);
			strncat(filePath, dir->d_name, 30);
			fd = open(filePath, O_WRONLY);
			write(fd, "0", 1);
			printf("%s\n", filePath);
			filePath[0] = '\0';
		}
	}

	// Set readonly permissions for player to those directories that need a key or tool to be opened
	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom","0066", NULL);
		printf("Error changing VaultRoom permissions: %s.\n", strerror(errno));
		exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/Parking/Basement","0066", NULL);
		printf("Error changing Basement permissions: %s.\n", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice","0066", NULL);
		printf("Error changing BossOffice permissions: %s.\n", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/JanitorRoom","0066", NULL);
		printf("Error changing JanitorRoom permissions: %s.\n", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2","0066", NULL);
		printf("Error changing Office2 permissions: %s.\n", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1","0066", NULL);
		printf("Error changing Office1 permissions: %s.\n", strerror(errno));
                exit(0);

	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom","0066", NULL);
		printf("Error changing MainBankingHall permissions: %s.\n", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0) {
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/VentilationDucts","0066", NULL);
                printf("Error changing VentilationDucts permissions: %s.\n", strerror(errno));
                exit(0);
	}
	else wait(NULL);
}
