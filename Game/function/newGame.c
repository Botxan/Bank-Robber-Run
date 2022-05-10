#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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
                execlp("find", "find", "./Directories", "-type", "l", "-delete", NULL);
                printf("Error emptying directories: %s\n", strerror(errno));
                exit(0);
        }
        else wait(NULL);


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
	symlink("../../../../../../assets/npc/Julian.npc", "Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice/Julian.npc");


	// If ventilation ducts have been discovered in previous game, remove symlink
        unlink("./Directories/Van/MainEntrance/MainBankingHall/ElectricalPanelRoom/VentilationDucts");


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
