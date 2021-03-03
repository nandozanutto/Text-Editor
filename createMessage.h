#include <stdio.h>
#include <string.h>

#ifndef __CREATEMESSAGE__
#define __CREATEMESSAGE__

typedef struct Package
{
    unsigned char MarcadorInicio;           
    unsigned char Destino : 2;
    unsigned char Origem : 2;              
    unsigned char Tamanho : 4;                
    unsigned char Sequencia : 4;               
    unsigned char Tipo : 4;
    unsigned char Dados[15];                
    unsigned char Paridade;                 
} Package;

void assignMessage(Package * outMessage, char origem, unsigned char *data, int tipo, int sequencia);
void resetMessage(Package * inMessage);
void copyMessage(Package * oriMessage, Package * destMessage);

#endif