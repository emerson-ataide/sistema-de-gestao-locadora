#ifndef BUSCA_SEQUENCIAL_H
#define BUSCA_SEQUENCIAL_H

#include <stdio.h>
#include "../Entidades/entidades.h"

// Declarações das funções de busca sequencial
TCliente* buscaSequencialCliente(FILE *arq, int id);
TFilme* buscaSequencialFilme(FILE *arq, int id);
TLocacao* buscaSequencialLocacao(FILE *arq, int id);
void salvarDadosSequencial(int comparacoes, double tempoExecucao);

#endif