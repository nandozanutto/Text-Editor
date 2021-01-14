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

int main(){

    int Soquete, count=0;
    char hello[] = "Hello from client\n"; 
    char inMessage[20];
    Soquete = ConexaoRawSocket("lo");
    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }

    // while(1){
    //     if(send(Soquete, hello, strlen(hello),  0) < 0)
    //     {
    //         perror("erro no envio da mensagem !\n");
    //         exit(1);
    //     }
    // }
      if(send(Soquete, hello, sizeof(hello),  0) < 0)
      {
          perror("erro no envio da mensagem !\n");
          exit(1);
      }

    
    while(1){
        if (recv(Soquete, inMessage, 20*sizeof(char), 0) < 0)
        {
            perror("Erro no recebimento!\n");
            exit(1);
          }
        if(inMessage[11] == 's'){
          printf("%s", inMessage);
          break;
        }
 
    }


    return 0;
}
