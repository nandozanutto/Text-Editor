#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "rawSocket.h"
#include "aux.h"

int main(){
    int Soquete;
    Package outMessage, inMessage;
    Soquete = ConexaoRawSocket("lo");
    outMessage.MarcadorInicio = (char) 0x7e;
    memset(inMessage.Dados, 0, 15);

    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }

    waitForMessage(Soquete, &inMessage);
    printf("Data received: %d\n", inMessage.Dados[0]);



  return 0;

}
