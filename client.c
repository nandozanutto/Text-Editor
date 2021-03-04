#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rawSocket.h"
#include <unistd.h>
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
#include <math.h>
extern int errno ;

int cdClient(int Soquete, unsigned char * inputString){
    int reply=0;
    Package outMessage, inMessage;
    assignMessage(&outMessage, 'C', inputString, 0, 0);
    
    while(1){
        sendMessage(Soquete, &outMessage);
        // printf("Data sent: '%s'\n", outMessage.Dados);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            // printf("Success\n");
            return 0;
        }
        if(reply == 0 && inMessage.Tipo == 15){//ERROR
            printf("Error %d on server, %s\n", inMessage.Dados[0], strerror(inMessage.Dados[0]));
            return -1;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
    
}

int lcdClient(unsigned char *inputString){
    char s[100]; 
    printf("%s\n", getcwd(s, 100)); 
    
    if(chdir(inputString) < 0){ //ERROR
        perror("Erro no comando CD!\n");
        return -1;
    }
    
    printf("%s\n", getcwd(s, 100)); 
}

int llsClient(){
    char c;
    FILE *file;
    file  = popen("ls", "r");
    if(file == NULL){
        printf("Error on ls command");
        return -1;
    }
    while(1){
        c = fgetc(file);
        if(c == EOF)
            return 0;
        printf("%c", c);
    }

}

int linhaClient(int Soquete, unsigned char *inputString, unsigned char *lineNumber, int tipo){
    int reply=0;
    Package outMessage, inMessage;
    assignMessage(&outMessage, 'C', inputString, tipo, 0);

    while(1){
        /*MESSAGE 1: 0011
        Keep sending message ultil ACK or error isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 15){//ERROR
            printf("Error on server, %s\n", strerror(inMessage.Dados[0]));
            return -1;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }

    assignMessage(&outMessage, 'C', lineNumber, 10, 1);
    while (1)
    {
        /*MESSAGE 3: 1010
        Keep sending message while no reply or nack is recieved*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 12){//FIRST TEXT MESSAGE AS ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
    }

    printf("%s", inMessage.Dados);
    
    while(1){
        while(1){  
            //MESSAGE 5: reply
            if(errorMessage(inMessage) < 0){//NACK
                sendNACK(Soquete, 'C');
            } else {
                sendACK(Soquete, 'C');//ACK
            }
            waitForMessage(Soquete, &inMessage, 2);
            if(inMessage.Tipo == 12 || inMessage.Tipo == 13)
                break;
        }
        if(inMessage.Tipo == 13){
            sendACK(Soquete, 'C');
            return 0;
        }
        printf("%s", inMessage.Dados);
    }
}

int linhasClient(int Soquete, unsigned char *inputString, unsigned char *lines){
    linhaClient(Soquete, inputString, lines, 4);
}

int verClient(int Soquete, unsigned char *inputString){
    int reply=0, count=2;
    Package outMessage, inMessage;
    assignMessage(&outMessage, 'C', inputString, 2, 0);

    while(1){
        /*MESSAGE 1: 0011
        Keep sending message ultil ACK or error isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 12){//FIRST TEXT MESSAGE AS ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 15){//ERROR
            printf("Error on server, %s\n", strerror(inMessage.Dados[0]));
            return -1;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
    printf("1 %s", inMessage.Dados);
    while(1){
        while(1){  
            //MESSAGE 5: reply
            if(errorMessage(inMessage) < 0){//NACK
                sendNACK(Soquete, 'C');
            } else {
                sendACK(Soquete, 'C');//ACK
            }
            waitForMessage(Soquete, &inMessage, 2);
            if(inMessage.Tipo == 12 || inMessage.Tipo == 13)
                break;
        }
        if(inMessage.Tipo == 13){
            sendACK(Soquete, 'C');
            return 0;
        }
        if(strchr(inMessage.Dados, '\n')!=NULL){
            printf("%s%d ", inMessage.Dados, count);
            count++;
        }else{
            printf("%s", inMessage.Dados);
        }
    }


}

int editClient(int Soquete, unsigned char *inputString, unsigned char *inputString2, unsigned char *lineNumber){
    int reply=0, result=0, count=1;
    Package outMessage, inMessage;
    assignMessage(&outMessage, 'C', inputString, 5, 0);
    unsigned char string[15];

    while(1){
        /*MESSAGE 1: 0011
        Keep sending message ultil ACK or error isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 15){//ERROR
            printf("Error on server, %s\n", strerror(inMessage.Dados[0]));
            return -1;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }

    assignMessage(&outMessage, 'C', lineNumber, 10, 0);
    while(1){
        /*MESSAGE 1: 0011
        Keep sending message ultil ACK or error isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 8){//ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 15){//ERROR
            printf("Error on server, %s\n", strerror(inMessage.Dados[0]));
            return -1;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }

    
    while(1){    
        result = cutString(inputString2, string, count);
        if(result < 0) break;
        assignMessage(&outMessage, 'C', string, 12, 0);
        while(1){
            /*MESSAGE 1: 0011
            Keep sending message ultil ACK or error isn't received*/
            sendMessage(Soquete, &outMessage);
            reply = waitForAnswer(Soquete, &inMessage, 2);
            if(reply == 0 && inMessage.Tipo == 8){//ACK
                break;
            }
            if(reply == 0 && inMessage.Tipo == 9)//NACK
                printf("Nack received");
        }
        count++;
    }
    
    assignMessage(&outMessage, 'C', "", 13, 0);
    while(1){
      /*MESSAGE 1: 1101
      Keep sending message ultil ACK isn't received*/
      sendMessage(Soquete, &outMessage);
      reply = waitForAnswer(Soquete, &inMessage, 2);
      if(reply == 0 && inMessage.Tipo == 8){//ACK
          return 0;
      }
      if(reply == 0 && inMessage.Tipo == 9)//NACK
          printf("Nack received");
      
  }

}

int lsClient(int Soquete){
    int reply=0, count=2;
    Package outMessage, inMessage;
    assignMessage(&outMessage, 'C', "", 1, 0);

    while(1){
        /*MESSAGE 1: 0001
        Keep sending message ultil ACK or error isn't received*/
        sendMessage(Soquete, &outMessage);
        reply = waitForAnswer(Soquete, &inMessage, 2);
        if(reply == 0 && inMessage.Tipo == 11){//FIRST TEXT MESSAGE AS ACK
            break;
        }
        if(reply == 0 && inMessage.Tipo == 15){//ERROR
            printf("Error on server, %s\n", strerror(inMessage.Dados[0]));
            return -1;
        }
        if(reply == 0 && inMessage.Tipo == 9)//NACK
            printf("Nack received");
        
    }
    printf("%s", inMessage.Dados);
    while(1){
        while(1){  
            //MESSAGE 5: reply
            if(errorMessage(inMessage) < 0){//NACK
                sendNACK(Soquete, 'C');
            } else {
                sendACK(Soquete, 'C');//ACK
            }
            waitForMessage(Soquete, &inMessage, 2);
            if(inMessage.Tipo == 11 || inMessage.Tipo == 13)
                break;
        }
        if(inMessage.Tipo == 13){
            sendACK(Soquete, 'C');
            return 0;
        }
        printf("%s", inMessage.Dados);
        
    }

}


int main(){

    int Soquete;
    char inputString[1045] = "cd nando ahola";
    char command[15];
    char param1[15];
    char param2[15];
    char param3[1000];
    int cont=0;
    const char s[2] = " ";
    Soquete = ConexaoRawSocket("lo");
    Package outMessage, inMessage;

    setvbuf (stdout, 0, _IONBF, 0);
    
    if (Soquete == -1)
    {
        perror("Erro no soquete\n");
        exit(1);
    }
    
    
    fgets(inputString, 1045, stdin);      
    inputString[strcspn(inputString, "\n")] = 0;//Removing the '\n' caracter  
    char *token = strtok(inputString, " ");
    while (token != NULL) 
    { 
            switch (cont)
            {
            case 0:
                    strcpy(command, token);
                    break;
            
            case 1:
                    strcpy(param1, token);
                    break;
            case 2:
                    strcpy(param2, token);
                    break;
            case 3:
                    strcpy(param3, token);
                    break;
            default:
                    printf("Too many arguments\n");
                    break;
            }
            token = strtok(NULL, " "); 
            cont++;
    } 

    if(!strcmp(command, "CD"))
        cdClient(Soquete, param1);




    // if (param1[0] == '\0')
    //     printf("yes");

    return 0;

}
