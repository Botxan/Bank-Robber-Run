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
                printf("\033[31mError applying default permissions to some directory under ./Directories %s.\n\033[37m", strerror(errno));
                exit(0);
        }
	else wait(NULL);


        // Clear map
        if (fork() == 0)
        {
                execlp("find", "find", "./Directories", "-type", "l", "!", "-iname", "*description.txt", "-delete", NULL);
                printf("\033[31mError emptying directories: %s\n\033[37m", strerror(errno));
                exit(0);
        }
        else wait(NULL);


	// Add room visited counters
        symlink("../../assets/roomVisitedCounter/VanCounter.txt", "Directories/Van/.counter.txt");
        symlink("../../../assets/roomVisitedCounter/MainEntranceCounter.txt", "Directories/Van/MainEntrance/.counter.txt");
        symlink("../../../../assets/roomVisitedCounter/ParkingCounter.txt", "Directories/Van/MainEntrance/Parking/.counter.txt");
        symlink("../../../../../assets/roomVisitedCounter/BasementCounter.txt", "Directories/Van/MainEntrance/Parking/Basement/.counter.txt");
        symlink("../../../../../../assets/roomVisitedCounter/VaultCorridorCounter.txt", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/.counter.txt");
        symlink("../../../../../../../assets/roomVisitedCounter/VaultRoomCounter.txt", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom/.counter.txt");
        symlink("../../../../../../../../assets/roomVisitedCounter/VaultCounter.txt", "Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom/Vault/.counter.txt");
        symlink("../../../../assets/roomVisitedCounter/MainBankingHallCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/.counter.txt");
        symlink("../../../../../assets/roomVisitedCounter/ElectricalPanelRoomCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/ElectricalPanelRoom/.counter.txt");
        symlink("../../../../../assets/roomVisitedCounter/LostAndFoundCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/LostAndFound/.counter.txt");
        symlink("../../../../../assets/roomVisitedCounter/CorridorCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/.counter.txt");
        symlink("../../../../../../assets/roomVisitedCounter/Office1Counter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1/.counter.txt");
        symlink("../../../../../../assets/roomVisitedCounter/Office2Counter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2/.counter.txt");
        symlink("../../../../../../assets/roomVisitedCounter/WCCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/.counter.txt");
        symlink("../../../../../../../assets/roomVisitedCounter/VentilationDuctsCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/VentilationDucts/.counter.txt");
        symlink("../../../../../../assets/roomVisitedCounter/SecurityRoomCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom/.counter.txt");
        symlink("../../../../../../assets/roomVisitedCounter/BossOfficeCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice/.counter.txt");
        symlink("../../../../../../assets/roomVisitedCounter/JanitorRoomCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/JanitorRoom/.counter.txt");
        symlink("../../../../../../assets/roomVisitedCounter/RooftopCounter.txt", "Directories/Van/MainEntrance/MainBankingHall/Corridor/Rooftop/.counter.txt");


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


	// Copy electrical panel
	system("cp assets/readonly/electrical-panel-ascii.txt assets/electrical-panel-ascii.txt");


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
	char filePath[80]; // asets/roomVisitedCounter/room_x_counter.npc

	if (!d) {
		write(1, "\033[31mDirectory assets/npc not found.\n\033[37m", strlen("\033[31mDirectory assets/npc not found.\n\033[37m"));
		exit(1);
	}

	while ((dir = readdir(d)) != NULL) {
		if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, "who.txt")) {
			strncpy(filePath, "assets/npc/", 50);
			strncat(filePath, dir->d_name, 50);
			fd = open(filePath, O_WRONLY);
			write(fd, "0", 1);
			filePath[0] = '\0';
			close(fd);
		}
	}
	closedir(d);

	// Set each room visited counter to 0
	d = opendir("assets/roomVisitedCounter");
	if (!d) {
                write(1, "\033[31mDirectory assets/roomVisitedCounter not found.\n\033[37m", strlen("\033[31mDirectory assets/roomVisitedCounter not found.\n\033[37m"));
                exit(1);
        }

	while ((dir = readdir(d)) != NULL) {
                if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                        strncpy(filePath, "assets/roomVisitedCounter/", 80);
                        strncat(filePath, dir->d_name, 80);
                        fd = open(filePath, O_WRONLY);
			ftruncate(fd, 1);
                        write(fd, "0", 1);
                        filePath[0] = '\0';
                        close(fd);
                }
        }
	closedir(d);


	// Set .obj default status to 0 (except electrical panel room, to 3 (= all lights on)
	d = opendir("assets/obj");
        if (!d) {
                write(1, "\033[31mDirectory assets/obj not found.\n\033[37m", strlen("\033[37mDirectory assets/obj not found.\n\033[37m"));
                exit(1);
        }

        while ((dir = readdir(d)) != NULL) {
                if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                        strncpy(filePath, "assets/obj/", 80);
                        strncat(filePath, dir->d_name, 80);
                        fd = open(filePath, O_WRONLY);
                        ftruncate(fd, 1);
			if (strcmp(dir->d_name, "electrical-panel.obj") == 0) write(fd, "3", 1);
			else write(fd, "0", 1);

	                filePath[0] = '\0';
                        close(fd);
                }
        }
        closedir(d);

	// Set readonly permissions for player to those directories that need a key or tool to be opened
	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/Parking/Basement/VaultCorridor/VaultRoom","0066", NULL);
		printf("\033[31mError changing VaultRoom permissions: %s.\n\033[37m", strerror(errno));
		exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/Parking/Basement","0066", NULL);
		printf("\033[31mError changing Basement permissions: %s.\n\033[37m", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/BossOffice","0066", NULL);
		printf("\033[31mError changing BossOffice permissions: %s.\n\033[37m", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/JanitorRoom","0066", NULL);
		printf("\033[31mError changing JanitorRoom permissions: %s.\n\033[37m", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/Office2","0066", NULL);
		printf("\033[31mError changing Office2 permissions: %s.\n\033[37m", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/Office1","0066", NULL);
		printf("\033[31mError changing Office1 permissions: %s.\n\033[37m", strerror(errno));
                exit(0);

	}
	else wait(NULL);

	if (fork() == 0)
	{
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/SecurityRoom","0066", NULL);
		printf("\033[31mError changing MainBankingHall permissions: %s.\n\033[37m", strerror(errno));
                exit(0);
	}
	else wait(NULL);

	if (fork() == 0) {
		execlp("./chmod","./chmod","./Directories/Van/MainEntrance/MainBankingHall/Corridor/WC/VentilationDucts","0066", NULL);
                printf("\033[31mError changing VentilationDucts permissions: %s.\n\033[37m", strerror(errno));
                exit(0);
	}
	else wait(NULL);


	// Print bank robber run ascii and begin the game!
	if (fork () == 0)
	{
		execlp("/bin/cat", "/bin/cat", "./assets/newGameAscii.txt", NULL);
		printf("\033[31mError printing new game ascii art: %s\n\033[37m", strerror(errno));
	}
	else wait(NULL);
}
