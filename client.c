#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rawSocket.h"
#include "aux.h"

int main(){

    int Soquete;
    Soquete = ConexaoRawSocket("lo");
    Package outMessage, inMessage;
    outMessage.MarcadorInicio = (char) 0x7e;
    outMessage.Origem = (char) 01;
    outMessage.Dados[0] = 0x6f;
    printf("Data to send: %d\n", outMessage.Dados[0]);


    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }

    sendMessage(Soquete, &outMessage);
    
    return 0;
}
