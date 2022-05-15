#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#define PRINT(x) printf("%s\n", x)
#define PRINT_INT(x) printf("%i\n", x)

int main(int argc, char** argv){
    if(argc > 1){
        printf("No parameters for ls command needed!\n");
        exit(-1);
    }
  
    char* current_directory = getcwd(NULL, 0);
    DIR* dir = opendir(current_directory);
    struct dirent* dent = NULL;
    printf("./ ../ ");
    
    while((dent = readdir(dir)) != NULL){
        if(dent->d_name[0] != '.'){
            struct stat data;
            stat(dent->d_name, &data);
          
  // Check if its a directory
    if(data.st_mode == 16877) //modificable
      printf("%s/ ", dent->d_name);
    else
      printf("%s ", dent->d_name);
      //printf("%i ", data.st_mode);
    }
  }
    printf("\n");
    return 0;
}
