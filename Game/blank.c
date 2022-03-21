char* blank(char* input) //font:https://stackoverflow.com/questions/13084236/function-to-remove-spaces-$
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)
    {
     	if (input[i]!=' ')
            output[j]=input[i];
        else
            j--;
    }
    output[j]=0;
    return output;
}
