#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rawSocket.h"
#include "aux.h"
#include "createMessage.h"

int main(){

    int Soquete;
    Soquete = ConexaoRawSocket("lo");
    Package outMessage, inMessage;
    
    unsigned char dados[15];
    memset(dados, 0, 15);
    dados[0] = 0xfa;
    
    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }

    assignMessage(&outMessage, 'C', dados, 10, 5, 1);
    sendMessage(Soquete, &outMessage);
    printf("Data sent: %d\n", outMessage.Dados[0]);

    if(!(waitForAnswer(Soquete, &inMessage, 2)))
        printf("Data received: %d\n", inMessage.Dados[0]);
    else 
        printf("No data received\n");
    
    return 0;
}
