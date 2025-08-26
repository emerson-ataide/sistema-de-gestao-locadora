#ifndef BUSCA_BINARIA_H
#define BUSCA_BINARIA_H

#include <stdio.h>
#include "../Entidades/entidades.h"

// Declarações das funções de busca binária
TCliente* buscaBinariaCliente(FILE *arq, int id, int n);
TFilme* buscaBinariaFilme(FILE *arq, int id, int n);
TLocacao* buscaBinariaLocacao(FILE *arq, int id, int n);
void salvarDadosBinaria(int comparacoes, double tempoExecucao);

#endif