int search(char *word , int file){
   char line[4] ;
   int i = 4;

   while(read(file,line[i],4) == 1){
    if(strcmp(line, word)){
     return i;
    }
   memset(line,0,strlen(line));
   i = i+4;
   }
}
