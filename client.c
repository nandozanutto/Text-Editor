#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rawSocket.h"
#include "aux.h"
#include "createMessage.h"
#include <errno.h>

extern int errno ;

int cdClient(int Soquete, unsigned char * inputString){
    int reply=0;
    Package outMessage, inMessage;
    assignMessage(&outMessage, 'C', inputString, 10, 0, 1);
    
    while(1){
        sendMessage(Soquete, &outMessage);
        printf("Data sent: '%s'\n", outMessage.Dados);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            printf("Success\n");
            return 0;
        }
        if(reply == 0 && inMessage.Tipo == 15){//ERROR
            printf("Error on server, %s\n", strerror(inMessage.Dados[0]));
            return -1;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
    
}

int linhaClient(int Soquete, unsigned char *inputString){
    int reply=0;
    Package outMessage, inMessage;
    assignMessage(&outMessage, 'C', inputString, 10, 3, 0);

    while(1){
        sendMessage(Soquete, &outMessage);
        printf("Data sent: '%s'\n", outMessage.Dados);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            printf("Success\n");
            break;
        }
        if(reply == 0 && inMessage.Tipo == 15){//ERROR
            printf("Error on server, %s\n", strerror(inMessage.Dados[0]));
            return -1;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
    
    assignMessage(&outMessage, 'C', "nando", 10, 10, 1);
    while (1)
    {
        sendMessage(Soquete, &outMessage);
        printf("Data sent: '%s'\n", outMessage.Dados);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 12){//FIRST TEXT MESSAGE
            printf("Success\n");
            break;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
    }
    
    printf("%s", inMessage.Dados);









}

int main(){

    int Soquete;
    Soquete = ConexaoRawSocket("lo");
    Package outMessage, inMessage;
    
    unsigned char inputString[] = "alice.txt";
    
    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }

    linhaClient(Soquete, inputString);
    
    return 0;
}
