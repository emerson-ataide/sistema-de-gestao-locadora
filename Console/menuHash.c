#include <stdio.h>
#include <stdlib.h>
#include "../hash/hash.h"

void menuHash(FILE *arqClientes, FILE *arqHash) {
    int opcao, codigo;
    TCliente cliente;

    do {
        printf("\n====== MENU HASH ======\n");
        printf("[1] Buscar cliente\n");
        printf("[2] Inserir cliente\n");
        printf("[3] Remover cliente\n");
        printf("[4] Imprimir Tabela Hash\n");
        printf("[0] Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite o codigo do cliente: ");
                scanf("%d", &codigo);
                TCliente* c = buscarHash(arqHash, arqClientes, codigo);
                if(c != NULL) {
                    printf("\nCliente encontrado:\n");
                    imprimirCliente(c);
                    free(c);
                } else {
                    printf("Cliente nao encontrado!\n");
                }
                break;

            case 2:
                printf("Digite o codigo (ID) do cliente: ");
                scanf("%d", &cliente.id);

                // VERIFICA SE O CLIENTE JÁ EXISTE
                TCliente *existente = buscarHash(arqHash, arqClientes, cliente.id);
                if (existente != NULL) {
                    printf("\nERRO: Um cliente com o ID %d ja existe!\n", cliente.id);
                    free(existente); // Libera a memória do cliente encontrado na busca
                    break;           // Volta para o menu
                }

                // Se não existe, continua o cadastro
                printf("Digite o nome: ");
                scanf(" %[^\n]", cliente.nome);
                printf("Digite o CPF: ");
                scanf(" %[^\n]", cliente.cpf);
                printf("Digite o telefone: ");
                scanf(" %[^\n]", cliente.telefone);
                inserirHash(arqHash, arqClientes, cliente);
                printf("Cliente inserido com sucesso!\n");
                break;

            case 3:
                printf("Digite o codigo do cliente: ");
                scanf("%d", &codigo);
                removerHash(arqHash, codigo);
                printf("Cliente removido com sucesso!\n");
                break;
            
            case 4:
                imprimirHash(arqHash); // Chamada atualizada
                break;
        }
    } while(opcao != 0);
}