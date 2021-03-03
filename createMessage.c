#include "createMessage.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void assignMessage(Package * outMessage, char origem, unsigned char *data, int tipo, int sequencia){
    unsigned char seq1;
	unsigned char seq2;
	unsigned char paridade;
    (*outMessage).MarcadorInicio = 126;
    (*outMessage).Origem = (origem == 'C') ? 1:2;
    (*outMessage).Destino = (origem == 'C') ? 2:1;
    (*outMessage).Tamanho = sizeof(data);
    (*outMessage).Sequencia = sequencia;
    (*outMessage).Tipo = tipo;
    memset((*outMessage).Dados, 0, 15);//putting zeros
    strcpy((*outMessage).Dados, data);
	//Creating parity bellow 
    seq1 =  ((*outMessage).Origem<<6) | ((*outMessage).Destino<<4) | (*outMessage).Tipo;
	seq2 = ((*outMessage).Sequencia<<4) | (*outMessage).Tamanho;
	paridade = seq1 ^ seq2;
	for(int i=0; i<strlen((*outMessage).Dados); i++)
		paridade ^= (*outMessage).Dados[i];
    (*outMessage).Paridade = paridade;
}

void resetMessage(Package * inMessage){
    (*inMessage).MarcadorInicio = 0;
    (*inMessage).Origem = 0;
    (*inMessage).Destino = 0;
    (*inMessage).Tamanho = 0;
    (*inMessage).Sequencia = 0;
    (*inMessage).Tipo = 0;
    memset((*inMessage).Dados, 0, 15);//putting zeros
    (*inMessage).Paridade = 0;
}

void copyMessage(Package * oriMessage, Package * destMessage){
    (*destMessage).MarcadorInicio = (*oriMessage).MarcadorInicio;
    (*destMessage).Origem = (*oriMessage).Origem;
    (*destMessage).Destino = (*oriMessage).Destino;
    (*destMessage).Tamanho = (*oriMessage).Tamanho;
    (*destMessage).Sequencia = (*oriMessage).Sequencia;
    (*destMessage).Tipo = (*oriMessage).Tipo;
    memset((*destMessage).Dados, 0, 15);//putting zeros
    strcpy((*destMessage).Dados, (*oriMessage).Dados);
    (*destMessage).Paridade = (*oriMessage).Paridade;
}