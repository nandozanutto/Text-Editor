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
#include <sys/stat.h>
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
  int reply=0, lineNumber=0, result=0;
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
  int aux = readLine(file, lineReading);


  assignMessage(&outMessage, 'S', lineReading, 12, 0);
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
  
  while(1){   
    if(aux==-1 || aux==-2) break; //already finished reading line
    result = readLine(file, lineReading);
    if(result == -1) break;
    assignMessage(&outMessage, 'S', lineReading, 12, 0);  
    while(1){
        /*MESSAGE 7: 1100
        Keep sending message ultil ACK isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 1);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
    if(result == -2) break;//line breaker
  }
  assignMessage(&outMessage, 'S', "", 13, 0);
    
  while(1){
    /*MESSAGE 1: 1101
    Keep sending message ultil ACK isn't received*/
    sendMessage(Soquete, &outMessage);
    reply = waitForAnswer(Soquete, &inMessage, 1);
    if(reply == 0 && inMessage.Tipo == 8){//ACK
        fclose(file);
        return 0;
    }
    if(reply == 0 && inMessage.Tipo == 9)//NACK
        printf("Nack received");
      
  }
  

}

int linhasServer(int Soquete, Package inMessage1){
  FILE *file;
  Package inMessage, outMessage;
  copyMessage(&inMessage1, &inMessage);
  int reply=0, lineNumber1=0, result=0, lineNumber2=0, count=0;
  unsigned char lineReading[15];
  unsigned char aux[15]; 
  char *token;
  const char s[2] = " ";

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

  strcpy(aux, inMessage.Dados);
  token= strtok(aux, s);
  lineNumber1 = atoi(token);
  token = strtok(NULL, s);
  lineNumber2 = atoi(token);
  
  // lineNumber = atoi(inMessage.Dados);
  goLine(file, lineNumber1);
  result = readLine(file, lineReading);
  if(result == -2) count++;//line breaker  
  assignMessage(&outMessage, 'S', lineReading, 12, 0);
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

  while(1){   
    result = readLine(file, lineReading);
    if(result == -1) break;
    assignMessage(&outMessage, 'S', lineReading, 12, 0);  
    if(count == (lineNumber2-lineNumber1 + 1)) break;
    if(result == -2) count++;//line breaker
    while(1){
        /*MESSAGE 7: 1100
        Keep sending message ultil ACK isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 1);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
    
  }

  assignMessage(&outMessage, 'S', "", 13, 0);
    
    while(1){
      /*MESSAGE 1: 1101
      Keep sending message ultil ACK isn't received*/
      sendMessage(Soquete, &outMessage);
      reply = waitForAnswer(Soquete, &inMessage, 1);
      if(reply == 0 && inMessage.Tipo == 8){//ACK
          fclose(file);
          return 0;
      }
      if(reply == 0 && inMessage.Tipo == 9)//NACK
          printf("Nack received");
      
  }



}

int verServer(int Soquete, Package inMessage1){
  FILE *file;
  Package inMessage, outMessage;
  copyMessage(&inMessage1, &inMessage);
  int reply=0, result=0, lineNumber2=0, count=0;
  unsigned char lineReading[15];


  puts(inMessage.Dados);//Name of file
  file  = fopen(inMessage.Dados, "r");
  
  goLine(file, 1);
  result = readLine(file, lineReading);
  assignMessage(&outMessage, 'S', lineReading, 12, 0);
  
  while(1){  
    //2 MESSAGE: reply
    if(errorMessage(inMessage) < 0){//NACK
      sendNACK(Soquete, 'S');
    } else {
      sendMessage(Soquete, &outMessage);//first message as ACK
    }
    reply = waitForMessage(Soquete, &inMessage, 1);
    if(reply!=0){ 
      sendError(Soquete, reply, 'S');
      return -1;
    }
    if(inMessage.Tipo == 8)//ACK RECEIVED
      break;
  }

  while(1){   
    result = readLine(file, lineReading);
    if(result == -1) break;
    assignMessage(&outMessage, 'S', lineReading, 12, 0);  
    while(1){
        /*MESSAGE 7: 1100
        Keep sending message ultil ACK isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 1);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
  }

  assignMessage(&outMessage, 'S', "", 13, 0);
    
  while(1){
    /*MESSAGE 1: 1101
    Keep sending message ultil ACK isn't received*/
    sendMessage(Soquete, &outMessage);
    reply = waitForAnswer(Soquete, &inMessage, 1);
    if(reply == 0 && inMessage.Tipo == 8){//ACK
        fclose(file);
        return 0;
    }
    if(reply == 0 && inMessage.Tipo == 9)//NACK
        printf("Nack received");
      
  }


}

int editServer(int Soquete, Package inMessage1){
  Package inMessage, outMessage;
  copyMessage(&inMessage1, &inMessage);
  int reply=0, result=0, count=0;
  unsigned char string[100];//ATTENTION TO SIZE OF THIS STRING
  unsigned char nameFIle[100];//ATTENTION TO SIZE
  int lineNumber;
  memset(string,0,sizeof(string));
  resetMessage(&outMessage);

  puts(inMessage.Dados);//Name of file
  strcpy(nameFIle, inMessage.Dados);
  
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
    if(inMessage.Tipo == 12)
      break;
  } 

  strcat(string, inMessage.Dados);
  // printf("%s", inMessage.Dados);

  while(1){
    while(1){  
        //MESSAGE 5: reply
        if(errorMessage(inMessage) < 0){//NACK
            sendNACK(Soquete, 'S');
        } else {
            sendACK(Soquete, 'S');//ACK
        }
        waitForMessage(Soquete, &inMessage, 1);
        if(inMessage.Tipo == 12 || inMessage.Tipo == 13)
            break;
    }
    if(inMessage.Tipo == 13){
        sendACK(Soquete, 'S');
        break;
    }
    strcat(string, inMessage.Dados);
    // printf("%s", inMessage.Dados);
  }
  printf("%s %d\n", string, lineNumber);
  if(editLine(nameFIle, string, lineNumber) == -1)
    return -1;
  chmod(nameFIle, 0777);
  return 0;

}

int lsServer(int Soquete, Package inMessage1){
  FILE *file;
  Package inMessage, outMessage;
  copyMessage(&inMessage1, &inMessage);
  int reply=0, result=0, lineNumber2=0, count=0;
  unsigned char lineReading[15];

  file  = popen("ls", "r");
  
  result = readLine(file, lineReading);
  assignMessage(&outMessage, 'S', lineReading, 11, 0);
  
  while(1){  
    //2 MESSAGE: reply
    if(errorMessage(inMessage) < 0){//NACK
      sendNACK(Soquete, 'S');
    } else {
      sendMessage(Soquete, &outMessage);//first message as ACK
    }
    reply = waitForMessage(Soquete, &inMessage, 1);
    if(reply!=0){ 
      sendError(Soquete, reply, 'S');
      return -1;
    }
    if(inMessage.Tipo == 8)//ACK RECEIVED
      break;
  }

  while(1){   
    result = readLine(file, lineReading);
    if(result == -1) break;
    assignMessage(&outMessage, 'S', lineReading, 11, 0);  
    while(1){
        /*MESSAGE 7: 1100
        Keep sending message ultil ACK isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 1);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
  }

  assignMessage(&outMessage, 'S', "", 13, 0);
    
  while(1){
    /*MESSAGE 1: 1101
    Keep sending message ultil ACK isn't received*/
    sendMessage(Soquete, &outMessage);
    reply = waitForAnswer(Soquete, &inMessage, 1);
    if(reply == 0 && inMessage.Tipo == 8){//ACK
        pclose(file);
        return 0;
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
      
      case 1:
        lsServer(Soquete, inMessage);
        break;

      case 2:
        verServer(Soquete, inMessage);
        break;

      case 3:
        linhaServer(Soquete, inMessage);
        break;
      
      case 4:
        linhasServer(Soquete, inMessage);
        break;

      case 5:
        editServer(Soquete, inMessage);
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
    Soquete = ConexaoRawSocket("lo");

    
    setvbuf (stdout, 0, _IONBF, 0);

    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }
    
    while(1)
      serverBehavior(Soquete);



  return 0;

}
