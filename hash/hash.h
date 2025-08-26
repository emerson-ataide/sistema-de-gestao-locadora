#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "../Entidades/entidades.h"

#define TAMANHO_TABELA 7

typedef struct RegistroHash {
    int codigo;
    long posicao;
    int proximo;
} RegistroHash;

void inicializarTabelaHash(FILE *arqHash);
int funcaoHash(int chave);
void inserirHash(FILE *arqHash, FILE *arqCliente, TCliente cliente);
TCliente* buscarHash(FILE *arqHash, FILE *arqCliente, int codigo);
void removerHash(FILE *arqHash, int codigo);
void imprimirHash(FILE *arqHash); // Parâmetro arqCliente removido
void popularHashComClientesIniciais(FILE *arqHash, FILE *arqClientes);

#endif