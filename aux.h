#include <stdio.h>
#include <string.h>
#include "createMessage.h"

#ifndef __AUX__
#define __AUX__

int errorMessage(Package inMessage);
int sendError(int Soquete, int numError, char origem);
int sendACK(int Soquete, char origem);
int sendNACK(int Soquete, char origem);
int sendMessage(int Soquete, Package * outMessage);
int waitForMessage(int Soquete, Package * inMessage, int origemExpect);
int waitForAnswer(int Soquete, Package * inMessage, int origemExpect);
int cutString(unsigned char * input, unsigned char *output, int num);


#endif