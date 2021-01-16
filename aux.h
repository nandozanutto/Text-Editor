#include <stdio.h>
#include <string.h>
#include "createMessage.h"

#ifndef __AUX__
#define __AUX__

int sendMessage(int Soquete, Package * outMessage);
int waitForMessage(int Soquete, Package * inMessage);
int waitForAnswer(int Soquete, Package * inMessage, int origemExpect);
// int waitForAnswer();
// int waitForAnswer(int Soquete, Package * inMessage);

#endif