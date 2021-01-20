#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int goLine(FILE *file, int num){
    char line[500]; //attention to maximum line size
    int count=1;
    
    if(num == 0){
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

int readFile(FILE *file, unsigned char * str){
    const char ch = '\n';
    if(fgets (str, 15, file) == NULL)
        return -1;//didn't found any caracter

    if(strchr(str, ch)!=NULL)
        return -2; //line found here 
    
    return 0;//succesful read and no linebreak found


}


int main(){
    
    int lineNumber = 5;
    char filename[] = "alice.txt";
    char c;
    int count=0;
    unsigned char str[15];
    const char ch = '\n';
    
    FILE *file = fopen(filename, "r+");

    if ( file == NULL )
        printf("ERROR");

    
    int resul;
    while(1){
        resul = readFile(file, str);
        printf("%s", str);
        if(resul!=0)
            break;
    }
    
    while(1){
        resul = readFile(file, str);
        if(resul == -1) break;
        printf("%s", str);
        if(resul == -2) break;

    }


    fclose(file);

}