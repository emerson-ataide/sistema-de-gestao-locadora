#include "entidades.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Retorna o tamanho em bytes de um registro Cliente
int tamanho_registroCliente() {
    return sizeof(TCliente);
}

// Cria um novo cliente na memória e inicializa seus dados
TCliente* cliente(int id, char *nome, char *cpf, char *telefone) {
    TCliente *c = (TCliente*) malloc(sizeof(TCliente));
    if (c == NULL) {
        printf("Erro ao alocar memória para cliente.\n");
        exit(1);
    }
    c->id = id;
    strncpy(c->nome, nome, sizeof(c->nome));
    c->nome[sizeof(c->nome)-1] = '\0';
    strncpy(c->cpf, cpf, sizeof(c->cpf));
    c->cpf[sizeof(c->cpf)-1] = '\0';
    strncpy(c->telefone, telefone, sizeof(c->telefone));
    c->telefone[sizeof(c->telefone)-1] = '\0';
    return c;
}

// Salva um cliente em arquivo binário (append)
void salvaCliente(TCliente *cliente, FILE *out) {
    fwrite(cliente, tamanho_registroCliente(), 1, out);
}

// Lê um cliente de arquivo binário (na posição atual do cursor)
TCliente* leCliente(FILE *in) {
    TCliente *c = (TCliente*) malloc(sizeof(TCliente));
    if (c == NULL) {
        printf("Erro ao alocar memória para leitura de cliente.\n");
        exit(1);
    }
    if (fread(c, tamanho_registroCliente(), 1, in) != 1) {
        free(c);
        return NULL; // fim do arquivo ou erro
    }
    return c;
}

// Imprime as informações de um cliente na tela
void imprimirCliente(TCliente *cliente) {
    printf("ID: %d\n", cliente->id);
    printf("Nome: %s\n", cliente->nome);
    printf("CPF: %s\n", cliente->cpf);
    printf("Telefone: %s\n", cliente->telefone);
    printf("---------------------------\n");
}

// Cria base de dados com clientes aleatórios para teste
void criarBaseCliente(FILE *out, int tam) {
    if (out == NULL) {
        printf("Arquivo inválido para criar base de clientes.\n");
        return;
    }

    char *nomes[] = {"Ana", "Bruno", "Carlos", "Diego", "Eduardo"};
    char *cpfs[] = {"12345678900", "98765432100", "11122233344", "55566677788", "99988877766"};
    char *telefones[] = {"31999999999", "31988888888", "31977777777", "31966666666", "31955555555"};

    srand(time(NULL));

    // Criar vetor com os IDs e embaralhar
    int ids[tam];
    for (int i = 0; i < tam; i++) {
        ids[i] = i + 1;
    }

    shuffleCliente(ids, tam); // embaralha os IDs

    for (int i = 0; i < tam; i++) {
        int id = ids[i]; // usa id embaralhado
        char *nome = nomes[rand() % 5];
        char *cpf = cpfs[rand() % 5];
        char *telefone = telefones[rand() % 5];

        TCliente *c = cliente(id, nome, cpf, telefone);
        salvaCliente(c, out);
        free(c);
    }
}


// Imprime todos os clientes de um arquivo
void imprimirBaseCliente(FILE *out) {
    if (out == NULL) {
        printf("Arquivo inválido para imprimir base de clientes.\n");
        return;
    }
    rewind(out);
    TCliente *c;
    while ((c = leCliente(out)) != NULL) {
        imprimirCliente(c);
        free(c);
    }
    rewind(out);
}

// Retorna o tamanho do arquivo (em bytes)
int tamanho_arquivoCliente(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tamanho = ftell(arq);
    rewind(arq);
    return tamanho;
}

// Embaralha vetor de inteiros para criar base desordenada
void shuffleCliente(int *vet, int TAM) {
    srand(time(NULL));
    for (int i = TAM - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vet[i];
        vet[i] = vet[j];
        vet[j] = temp;
    }
}
