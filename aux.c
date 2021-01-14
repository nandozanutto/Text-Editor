#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rawSocket.h"
#include "aux.h"
#include <time.h>

void sendMessage(int Soquete, Package outMessage)
{
    
    if(send(Soquete, &outMessage, sizeof(outMessage),  0) < 0)
    {
          perror("erro no envio da mensagem !\n");
          exit(1);
    }
}

Package waitForMessage(int Soquete, Package inMessage)
{
    while(1){
        if (recv(Soquete, &inMessage, sizeof(inMessage), 0) < 0)
        {
            perror("Erro no recebimento!\n");
            exit(1);
        }
        if(inMessage.MarcadorInicio == 126){
            return inMessage;
        }
    }
}

int waitForAnswer()
{
    clock_t i, f;
    i = clock();
    f = 0;
    while(1){
        if(( ((double)(f-i))/CLOCKS_PER_SEC) > 1.0)
            return -1;
        f = clock();
    }
}