#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rawSocket.h"
#include "aux.h"
#include "createMessage.h"
#include <errno.h>

extern int errno ;
int main(){

    int Soquete;
    Soquete = ConexaoRawSocket("lo");
    Package outMessage, inMessage;
    
    unsigned char inputString[] = "NANDO";
    unsigned char dados[15];
    memset(dados, 0, 15);
    strcpy(dados, inputString);
    
    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }

    assignMessage(&outMessage, 'C', dados, 10, 5, 1);
    sendMessage(Soquete, &outMessage);
    printf("Data sent: %s\n", outMessage.Dados);

    if(!(waitForAnswer(Soquete, &inMessage, 2))){
        printf("Data type received: %d %d\n", inMessage.Tipo, inMessage.Dados[0]);
        printf("Error opening file: %s\n", strerror( inMessage.Dados[0] ));
    }
    else 
        printf("No data received\n");
    
    return 0;
}
