#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "rawSocket.h"
#include "aux.h"
#include "createMessage.h"
#include "fileFunc.h"

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
  FILE *file;
  Package inMessage, outMessage;
  copyMessage(&inMessage1, &inMessage);
  int reply = 0;
  int lineNumber=0;
  unsigned char lineReading[15];

  puts(inMessage.Dados);//Name of file
  file  = fopen(inMessage.Dados, "r");
  
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

  puts(inMessage.Dados);//Number of line
  lineNumber = atoi(inMessage.Dados);
  goLine(file, lineNumber);
  readLine(file, lineReading);


  assignMessage(&outMessage, 'S', lineReading, 0, 12, 0);
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
  
  assignMessage(&outMessage, 'S', "Second message", 0, 12, 0);  
  while(1){
      /*MESSAGE 7: 1100
      Keep sending message ultil ACK isn't received*/
      sendMessage(Soquete, &outMessage);
      printf("Data sent1: '%s'\n", outMessage.Dados);
      reply = waitForAnswer(Soquete, &inMessage, 1);
      if(reply == 0 && inMessage.Tipo == 8){//ACK
          printf("Success\n");
          break;
      }
      if(reply == 0 && inMessage.Tipo == 9)//NACK
          printf("Nack received");
      
  }
  

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
