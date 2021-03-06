#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 2000

int goLine(FILE *file, int num){
    if(file == NULL) return -1;
    char line[BUFFER_SIZE]; //attention to maximum line size
    int count=2;
    
    if(num == 1 || num == 0){
        rewind(file);
        return 0;
    }

    while (fgets(line, sizeof line, file) != NULL) //stops in error or EOF
    {
        if (count == num)
            return 0;
        else
            count++;
    }

}

int countLines(FILE *file){
    int count = 0; 
    char c;  
    if(file == NULL) return -1;
    
    for (c = getc(file); c != EOF; c = getc(file)) 
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1;
    return count+1;
}

int readLine(FILE *file, unsigned char * str){
    if(file == NULL) return -1;
    memset(str, 0, 15);//putting zeros
    const char ch = '\n';
    if(fgets (str, 15, file) == NULL)
        return -1;//didn't found any caracter

    if(strchr(str, ch)!=NULL)
        return -2; //line found here 
    
    return 0;//succesful read and no linebreak found

    /* Use this for reading and send messages.
    while(1){
        resul = readFile(file, str);
        if(resul == -1) break;
        printf("%s", str);
        if(resul == -2) break;//line breaker
    }
    */

}

int editLine(char * path, char * newline, int line){
    FILE * fPtr;
    FILE * fTemp;
    int count = 0;
    char buffer[BUFFER_SIZE];
    int numFinalLine=0;

    fPtr  = fopen(path, "r");
    fTemp = fopen("replace.tmp", "w");


    if (fPtr == NULL || fTemp == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open file.\n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        return -1;
    }
    
    numFinalLine = countLines(fPtr);
    goLine(fPtr, 1);
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL)
    {
        count++;

        /* If current line is line to replace */
        if((count == line) && (line == numFinalLine)){
            fputs(newline, fTemp);
        }
        else if (count == line){
            fputs(newline, fTemp);
            fputc('\n', fTemp);
        }
        else
            fputs(buffer, fTemp);
    }


    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fTemp);


    /* Delete original source file */
    remove(path);

    /* Rename temporary file as original file */
    rename("replace.tmp", path);

    printf("\nSuccessfully replaced '%d' line with '%s'.", line, newline);
    return 0;

}
