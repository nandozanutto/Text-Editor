#include "createMessage.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void assignMessage(Package * outMessage, char origem, unsigned char *data, int paridade, int tipo, int sequencia){
    (*outMessage).MarcadorInicio = 126;
    (*outMessage).Origem = (origem == 'C') ? 1:2;
    (*outMessage).Destino = (origem == 'C') ? 2:1;
    (*outMessage).Tamanho = sizeof(data);
    (*outMessage).Sequencia = sequencia;
    (*outMessage).Tipo = tipo;
    memset((*outMessage).Dados, 0, 15);//putting zeros
    strcpy((*outMessage).Dados, data);
    (*outMessage).Paridade = paridade;

}