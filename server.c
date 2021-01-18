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
#include <errno.h>
extern int errno ;


int cdServer(int Soquete, Package inMessage){
  Package outMessage;
  char s[100]; 
  printf("%s\n", getcwd(s, 100)); 

  if(errorMessage(inMessage) < 0){
    sendNACK(Soquete, 'S');
    return -1;
  }
  
  if(chdir(inMessage.Dados) < 0){
      perror("Erro no comando CD!\n");
      sendError(Soquete, errno, 'S');
    }
  
  printf("%s\n", getcwd(s, 100)); 
  sendACK(Soquete, 'S');

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
      sendError(Soquete, reply, 'S');
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
