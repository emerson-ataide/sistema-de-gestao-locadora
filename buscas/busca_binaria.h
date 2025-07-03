#ifndef BUSCA_BINARIA_H
#define BUSCA_BINARIA_H

#include <stdio.h>
#include "../Entidades/entidades.h"

void salvarDadosBinaria(int comparacoes, double tempoExecucao);

TCliente* buscaBinariaCliente(FILE *arq, int id, int n);
TFilme* buscaBinariaFilme(FILE *arq, int id, int n);
TLocacao* buscaBinariaLocacao(FILE *arq, int id, int n);

#endif
