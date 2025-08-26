#ifndef ORDENACAOEXTERNA_H_INCLUDED
#define ORDENACAOEXTERNA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Entidades/entidades.h"


// *********************************************** ENTIDADE CLIENTE ***********************************************

int selecaoNaturalCliente(FILE *entrada, int M);

void inserirMemoriaCliente(TCliente heap[], int *n, TCliente novo);

TCliente extrairMinCliente(TCliente heap[], int *n);

void memMinimaCliente(TCliente heap[], int n, int i);

void trocarCliente(TCliente *a, TCliente *b);

void salvarDadosClassificacao(double tempoExecucao, int numParticoes);

void verificaParticoesCliente(int numParticoes);



#endif // ORDENACAOEXTERNA_H_INCLUDED