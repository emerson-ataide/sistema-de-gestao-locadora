#ifndef INTERCALACAO_H_INCLUDED
#define INTERCALACAO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Entidades/entidades.h"


// *********************************************** ENTIDADE CLIENTE ***********************************************
// Estrutura para armazenar os primeiros registros de cada arquivo
typedef struct
{
    TCliente cliente;
    int origem; // Índice do arquivo de origem
} ElementoMemoriaCliente;

void intercalacaoOtimaCliente(int numArquivos, int F);

void intercalarArquivosCliente(char arquivos[][50], int numArquivos, char *arquivoSaida, int *comparacoes);

void memMinimaElementosCliente(ElementoMemoriaCliente heap[], int n, int i, int *comparacoes);

void salvarDadosIntercalacao(int comparacoes, double tempoExecucao);

#endif // INTERCALACAO_H_INCLUDED