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

void cdServer(int Soquete, Package inMessage){
  Package outMessage;
  //Doing some server stuff here
  unsigned char dados[15];
  memset(dados, 0, 15);
  dados[0] = 0xff;

  
  assignMessage(&outMessage, 'S', dados, 10, 0, 1);
  sendMessage(Soquete, &outMessage); 
  printf("Data received from client: %d\n", inMessage.Dados[0]);
  printf("Data send to client: %d\n", outMessage.Dados[0]); 

}

void serverBehavior(int Soquete){
  Package inMessage, outMessage;
  int reply = waitForMessage(Soquete, &inMessage);
  if(!reply){
    
    switch (inMessage.Tipo)
    {
      
      case 5:
        cdServer(Soquete, inMessage);
        break;
      
      default:
        break;
    }
    
  }
  else
  {
    printf("error...");
  }
   
}


int main(){
    int Soquete;
    Package outMessage, inMessage;
    Soquete = ConexaoRawSocket("lo");
    

    unsigned char dados[15];
    memset(dados, 0, 15);
    dados[0] = 0xff;


    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }
    
    serverBehavior(Soquete);

  return 0;

}
