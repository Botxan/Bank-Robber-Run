#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[]){
  
  if (argc!=2){
    printf("Pick a different item to less");
    printf("\n");
    return 0;
    
  }else{

      char buffer[50];
      char c;
      int file;
      file = open(argv[1], O_RDWR);

        if (file != -1){
          read(file, buffer, 50);
          printf("\n %s \n", buffer);
        }else{
            printf("There is no %s here.\n", argv[1]);
        }
    close(file);
    return 0;
  }

    return 0;
  
}
