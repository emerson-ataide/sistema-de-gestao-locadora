#include <stdio.h>
#include <stdlib.h>
#include "Console/menuConsole.c"
#include "Entidades/filme.c"
#include "Entidades/locacao.c"
#include "Entidades/cliente.c"
#include "OrdenacaoExterna/classificacao.c"
#include "OrdenacaoExterna/intercalacao.c"
#include "buscas/busca_binaria.c"
#include "buscas/busca_sequencial.c"
#include "Ordenacao/selection_sort.c"
#include "hash/hash.c"


int main() {
    system("mkdir ArquivosDat >nul 2>nul");

    FILE *arqClientes = fopen("ArquivosDat/clientes.dat", "w+b");
    FILE *arqFilmes = fopen("ArquivosDat/filmes.dat", "w+b");
    FILE *arqLocacoes = fopen("ArquivosDat/locacoes.dat", "w+b");
    FILE *arqHash = fopen("ArquivosDat/hash.dat", "w+b");

    if (arqClientes == NULL || arqFilmes == NULL || arqLocacoes == NULL || arqHash == NULL)
    {
        printf("Erro ao abrir um dos arquivos\n");
        exit(1);
    }

    inicializarTabelaHash(arqHash);

    int op, tamClientes, tamFilmes;

    printf("========== SISTEMA DE LOCADORA ==========\n");

    printf("\nDigite a quantidade de clientes para gerar: ");
    scanf("%d", &tamClientes);
    printf("Digite a quantidade de filmes para gerar: ");
    scanf("%d", &tamFilmes);

    criarBaseCliente(arqClientes, tamClientes);
    criarBaseFilme(arqFilmes, tamFilmes);

    printf("\nDeseja imprimir os dados criados? [1] Sim [2] Nao: ");
    scanf("%d", &op);
    if (op == 1) {
        printf("\n========== BASE DE CLIENTES ==========\n");
        imprimirBaseCliente(arqClientes);
        printf("\n========== BASE DE FILMES ==========\n");
        imprimirBaseFilme(arqFilmes);
    }

     // *** PERGUNTA SOBRE ORDENAÇÃO COM SELECTION SORT ***
    printf("\nDeseja ordenar as bases com Selection Sort? [1] Sim [2] Nao: ");
    scanf("%d", &op);

    if (op == 1) {
        ordenarClientes(arqClientes, tamClientes);
        ordenarFilmes(arqFilmes, tamFilmes);
        printf("\n========== BASE DE CLIENTES ==========\n");
        imprimirBaseCliente(arqClientes);
        printf("\n========== BASE DE FILMES ==========\n");
        imprimirBaseFilme(arqFilmes);
        printf("Arquivos ordenados com sucesso!\n");
    } else {
        printf("Arquivos nao foram ordenados. Busca binaria pode nao funcionar corretamente.\n");
    }

    // Chama o menu principal (onde as buscas e demais funções estão)
    exibirMenuPrincipal(arqClientes, arqFilmes, arqLocacoes, arqHash);

    fclose(arqClientes);
    fclose(arqFilmes);
    fclose(arqLocacoes);
    fclose(arqHash);

    return 0;
}