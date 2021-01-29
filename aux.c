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
#include "createMessage.h"
#include "aux.h"
#include <time.h>
#include <errno.h>

extern int errno ;

int errorMessage(Package inMessage){
    return 0;
}

int sendError(int Soquete, int numError, char origem){
    Package outMessage;
    unsigned char dados[15];
    memset(dados, 0, 15);
    dados[0] = numError;
    
    if(numError > 255) return -1;
    assignMessage(&outMessage, origem, dados, 0, 15, 0);
    sendMessage(Soquete, &outMessage);
}

int sendACK(int Soquete, char origem){
    Package outMessage;
    unsigned char dados[15];
    memset(dados, 0, 15);

    assignMessage(&outMessage, origem, dados, 0, 8, 0);
    sendMessage(Soquete, &outMessage);
}

int sendNACK(int Soquete, char origem){
    Package outMessage;
    unsigned char dados[15];
    memset(dados, 0, 15);

    assignMessage(&outMessage, origem, dados, 0, 9, 0);
    sendMessage(Soquete, &outMessage);
}

int sendMessage(int Soquete, Package * outMessage)
{
    
    if(send(Soquete, outMessage, sizeof(Package),  0) < 0)
    {
          perror("erro no envio da mensagem !\n");
          return -1;
    }
    return 0;
}

int waitForMessage(int Soquete, Package * inMessage, int origemExpect)
{
    Package duplicate;
    resetMessage(inMessage);//Reset to receive a new one
    int errnum;//added
    while(1){
        if (recv(Soquete, inMessage, sizeof(Package), 0) < 0)
        {
            errnum = errno; //added
            if(errnum != 11){
                perror("Erro no recebimento!\n");
                return errnum;
            }
        }
        if(((*inMessage).MarcadorInicio == 126) && ((*inMessage).Origem == origemExpect) ) {
            recv(Soquete, &duplicate, sizeof(Package), 0); //Ignoring duplicate message
            return 0;
        }
    }
}

int waitForAnswer(int Soquete, Package * inMessage, int origemExpect)
{
    Package duplicate;
    resetMessage(inMessage);//Reset to receive a new one
    int errnum;//added
    time_t i, f;
    i = time(NULL);
    f = 0;
    while( (time(NULL) - i) < 2 ){
        if (recv(Soquete, inMessage, sizeof(Package), 0) < 0)
        {
            errnum = errno; //added
            if(errnum != 11){
                perror("Erro no recebimento!\n");
                return -1;
            }
        }
        if(((*inMessage).MarcadorInicio == 126) && ((*inMessage).Origem == origemExpect)){ 
            recv(Soquete, &duplicate, sizeof(Package), 0); //Ignoring duplicate message            
            return 0;
        }
    }

    return -2;//timeout. NO message or Wrong message received

}

int cutString(unsigned char * input, unsigned char *output, int num){
    unsigned char * pt;
    int size = strlen(input);
    float aux = (float)size/14;
    int result=0;

    result = (aux > size/14) ? size/14+1 : size/14;
    if(num>result) return -1;

    pt = input + (num-1)*14;
    strncpy(output, pt, 14);
    printf("%s\n",output);
    return 0;
}