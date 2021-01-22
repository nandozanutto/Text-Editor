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

  if(errorMessage(inMessage) < 0){//NACK
    sendNACK(Soquete, 'S');
    return -1;
  }
  
  if(chdir(inMessage.Dados) < 0){ //ERROR
      perror("Erro no comando CD!\n");
      sendError(Soquete, errno, 'S');
      return -1;
    }
  
  printf("%s\n", getcwd(s, 100)); 
  sendACK(Soquete, 'S');//ACK
  return 0;

}

int linhaServer(int Soquete, Package inMessage1){
  Package inMessage;

  if(errorMessage(inMessage1) < 0){//NACK
    sendNACK(Soquete, 'S');
    return -1;
  } else {
    sendACK(Soquete, 'S');//ACK
  }
  recv(Soquete, &inMessage, sizeof(inMessage), 0);
  waitForMessage(Soquete, &inMessage);
  printf("%s", inMessage.Dados);
  printf("%s", (char *) 15);

}



void serverBehavior(int Soquete){
  Package inMessage, outMessage;
  int reply = waitForMessage(Soquete, &inMessage);
  if(!reply){
    
    switch (inMessage.Tipo)
    {
      
      case 0:
        cdServer(Soquete, inMessage);
        break;
      
      case 3:
        linhaServer(Soquete, inMessage);
        break;
      
      default:
        break;
    }
    
  }
  else
  {
      sendError(Soquete, reply, 'S'); //ERROR ON SERVER RCV
  }
  

}


int main(){
    int Soquete;
    Package outMessage, inMessage;
    Soquete = ConexaoRawSocket("lo");
    
    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }
    
    // while(1){
    //   serverBehavior(Soquete);
    //   recv(Soquete, &inMessage, sizeof(inMessage), 0); 
    // }

    serverBehavior(Soquete);



  return 0;

}
