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

    assignMessage(&outMessage, 'C', dados, 10, 0, 1);
    sendMessage(Soquete, &outMessage);
    printf("Data send: %d\n", outMessage.Dados[0]);
    
    return 0;
}
