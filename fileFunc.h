#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef FILEFUNC_H_
#define FILEFUNC_H_


int goLine(FILE *file, int num);
int readLine(FILE *file, unsigned char * str);
int editLine(char * path, char * newline, int line);


#endif