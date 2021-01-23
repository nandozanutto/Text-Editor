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
  Package inMessage, outMessage;
  copyMessage(&inMessage1, &inMessage);
  int reply = 0;

  puts(inMessage.Dados);
  
  while(1){  
    //2 MESSAGE: reply
    if(errorMessage(inMessage) < 0){//NACK
      sendNACK(Soquete, 'S');
    } else {
      sendACK(Soquete, 'S');//ACK
    }
    reply = waitForMessage(Soquete, &inMessage, 1);
    if(reply!=0){ 
      sendError(Soquete, reply, 'S');
      return -1;
    }
    if(inMessage.Tipo == 10)
      break;
  }

  puts(inMessage.Dados);


  assignMessage(&outMessage, 'S', "First text message", 0, 12, 0);
  while(1){  
    //4 MESSAGE: reply with text or NACK
    if(errorMessage(inMessage) < 0){//NACK
      sendNACK(Soquete, 'S');
    } else {
        sendMessage(Soquete, &outMessage);//FIRST TEXT AS AN ACK
    }
    waitForMessage(Soquete, &inMessage, 1);
    if(inMessage.Tipo == 8)//ACK RECEIVED
      break;
  }
  printf("ACK\n");

  // assignMessage(&outMessage, 'S', "Second text message", 0, 12, 1);
  // sendMessage(Soquete, &outMessage);

}



void serverBehavior(int Soquete){
  Package inMessage, outMessage;
  int reply = waitForMessage(Soquete, &inMessage, 1);
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
    
    setvbuf (stdout, 0, _IONBF, 0);

    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }
    
    // while(1){
    //   serverBehavior(Soquete);
    // }

    serverBehavior(Soquete);


    // while(1){
    //   waitForMessage(Soquete, &inMessage, 1);
    //   puts(inMessage.Dados);
    // }
  return 0;

}
