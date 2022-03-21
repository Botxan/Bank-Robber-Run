int search(char *word , int file){
   char line[4] ;
   int i = 0;

   while(read(file,line[i],3) == 1){
    if(strcmp(line, word)){
     return i;
    }
   memset(line,0,strlen(line));
   i = i+3;
   }
}
