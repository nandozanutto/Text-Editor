#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rawSocket.h"
#include "aux.h"
#include "createMessage.h"
#include <errno.h>
#include "fileFunc.h"

#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/if.h>
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

int linhaClient(int Soquete, unsigned char *inputString, unsigned char *lineNumber){
    int reply=0;
    Package outMessage, inMessage;
    assignMessage(&outMessage, 'C', inputString, 10, 3, 0);

    while(1){
        /*MESSAGE 1: 0011
        Keep sending message ultil ACK or error isn't received*/
        sendMessage(Soquete, &outMessage);
        printf("Data sent1: '%s'\n", outMessage.Dados);
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

    assignMessage(&outMessage, 'C', lineNumber, 10, 10, 1);
    while (1)
    {
        /*MESSAGE 3: 1010
        Keep sending message while no reply or nack is recieved*/
        sendMessage(Soquete, &outMessage);
        printf("Data sent2: '%s'\n", outMessage.Dados);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 12){//FIRST TEXT MESSAGE AS ACK
            printf("Success\n");
            break;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
    }

    printf("%s", inMessage.Dados);
    
    while(1){  
        //MESSAGE 5: reply
        if(errorMessage(inMessage) < 0){//NACK
            sendNACK(Soquete, 'C');
        } else {
            sendACK(Soquete, 'C');//ACK
        }
        waitForMessage(Soquete, &inMessage, 2);//Second text message
        if(inMessage.Tipo == 12)
            break;
    }
    printf("%s", inMessage.Dados);
    sendACK(Soquete, 'C');

}

int main(){

    int Soquete;
    Soquete = ConexaoRawSocket("lo");
    Package outMessage, inMessage;

    setvbuf (stdout, 0, _IONBF, 0);
    
    unsigned char inputString[] = "alice.txt";
    
    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }

    linhaClient(Soquete, inputString, "9");
    // assignMessage(&outMessage, 'C', "nando", 0, 0, 0);
    // send(Soquete, &outMessage, sizeof(Package), 0);
    // assignMessage(&outMessage, 'C', "zanu", 0, 0, 0);
    // send(Soquete, &outMessage, sizeof(Package), 0);

    return 0;
}
