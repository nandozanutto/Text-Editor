#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "rawSocket.h"
#include "aux.h"
#include "createMessage.h"

#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/if.h>

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
    
    // waitForMessage(Soquete, &inMessage);
    waitForAnswer(Soquete, &inMessage, 1);
    printf("Data received: %d\n", inMessage.Dados[0]);



  return 0;

}
