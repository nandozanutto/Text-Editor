#include <stdio.h>
#include <string.h>

#ifndef __AUX__
#define __AUX__

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

void sendMessage(int Soquete, Package outMessage);
Package waitForMessage(int Soquete, Package inMessage);
int waitForAnswer();

#endif