#ifndef BUSCA_BINARIA_H
#define BUSCA_BINARIA_H

#include <stdio.h>
#include "../Entidades/entidades.h"

// Funções para busca binária em arquivo, recebendo o arquivo, o id e o tamanho da base (número de registros)
TCliente* buscaBinariaCliente(FILE *arq, int id, int tamanho);
TFilme* buscaBinariaFilme(FILE *arq, int id, int tamanho);
TLocacao* buscaBinariaLocacao(FILE *arq, int id, int tamanho);

#endif
