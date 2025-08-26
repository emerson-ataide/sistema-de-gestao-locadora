#ifndef MENU_CONSOLE_H
#define MENU_CONSOLE_H

#include <stdio.h>

void exibirMenuPrincipal(FILE *arqClientes, FILE *arqFilmes, FILE *arqLocacoes, FILE *arqHash);
int tamanhoArquivo(FILE *arq, size_t tamanhoRegistro);

#endif