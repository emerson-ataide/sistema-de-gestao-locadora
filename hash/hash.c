#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

void inicializarTabelaHash(FILE *arqHash) {
    RegistroHash reg;
    reg.codigo = -1;
    reg.posicao = -1;
    reg.proximo = -1;
    
    for(int i = 0; i < TAMANHO_TABELA; i++) {
        fseek(arqHash, i * sizeof(RegistroHash), SEEK_SET);
        fwrite(&reg, sizeof(RegistroHash), 1, arqHash);
    }
}

int funcaoHash(int chave) {
    return chave % TAMANHO_TABELA;
}

// VERSÃO CORRIGIDA
void inserirHash(FILE *arqHash, FILE *arqCliente, TCliente cliente) {
    int pos = funcaoHash(cliente.id);
    RegistroHash reg;
    long posRegistroAtual = pos * sizeof(RegistroHash);

    fseek(arqHash, posRegistroAtual, SEEK_SET);
    fread(&reg, sizeof(RegistroHash), 1, arqHash);

    if(reg.codigo != -1) { // Colisão
        // Encontra o final da lista encadeada
        while(reg.proximo != -1) {
            posRegistroAtual = reg.proximo * sizeof(RegistroHash);
            fseek(arqHash, posRegistroAtual, SEEK_SET);
            fread(&reg, sizeof(RegistroHash), 1, arqHash);
        }

        // Encontra o próximo espaço livre na área de overflow
        int novaPosicao = TAMANHO_TABELA;
        RegistroHash regTemp;
        while(1) {
            fseek(arqHash, novaPosicao * sizeof(RegistroHash), SEEK_SET);
            if (fread(&regTemp, sizeof(RegistroHash), 1, arqHash) != 1 || regTemp.codigo == -1) {
                break;
            }
            novaPosicao++;
        }

        // Linka o último nó da lista ao novo espaço livre
        reg.proximo = novaPosicao;
        fseek(arqHash, posRegistroAtual, SEEK_SET); // Volta para a posição do último nó
        fwrite(&reg, sizeof(RegistroHash), 1, arqHash);

        pos = novaPosicao; // A nova inserção ocorrerá na área de overflow
    }

    // Insere o cliente no final do arquivo de dados
    fseek(arqCliente, 0, SEEK_END);
    long posicaoCliente = ftell(arqCliente);
    salvaCliente(&cliente, arqCliente);

    // Prepara e escreve o novo registro na tabela hash
    reg.codigo = cliente.id;
    reg.posicao = posicaoCliente;
    reg.proximo = -1;

    fseek(arqHash, pos * sizeof(RegistroHash), SEEK_SET);
    fwrite(&reg, sizeof(RegistroHash), 1, arqHash);
}

TCliente* buscarHash(FILE *arqHash, FILE *arqCliente, int codigo) {
    int pos = funcaoHash(codigo);
    RegistroHash reg;
    
    fseek(arqHash, pos * sizeof(RegistroHash), SEEK_SET);
    fread(&reg, sizeof(RegistroHash), 1, arqHash);
    
    while(reg.codigo != -1) {
        if(reg.codigo == codigo) {
            fseek(arqCliente, reg.posicao, SEEK_SET);
            return leCliente(arqCliente);
        }
        if(reg.proximo == -1) break;
        
        fseek(arqHash, reg.proximo * sizeof(RegistroHash), SEEK_SET);
        fread(&reg, sizeof(RegistroHash), 1, arqHash);
    }
    
    return NULL;
}

void removerHash(FILE *arqHash, int codigo) {
    int pos = funcaoHash(codigo);
    RegistroHash reg, anterior;
    anterior.proximo = -1;
    long posAnterior = -1;

    fseek(arqHash, pos * sizeof(RegistroHash), SEEK_SET);
    fread(&reg, sizeof(RegistroHash), 1, arqHash);
    
    while(reg.codigo != -1) {
        if(reg.codigo == codigo) {
            if (anterior.proximo == -1) { 
                if (reg.proximo != -1) { 
                    RegistroHash proximoReg;
                    fseek(arqHash, reg.proximo * sizeof(RegistroHash), SEEK_SET);
                    fread(&proximoReg, sizeof(RegistroHash), 1, arqHash);
                    fseek(arqHash, pos * sizeof(RegistroHash), SEEK_SET);
                    fwrite(&proximoReg, sizeof(RegistroHash), 1, arqHash);
                } else {
                    reg.codigo = -1;
                    fseek(arqHash, pos * sizeof(RegistroHash), SEEK_SET);
                    fwrite(&reg, sizeof(RegistroHash), 1, arqHash);
                }
            } else {
                anterior.proximo = reg.proximo;
                fseek(arqHash, posAnterior, SEEK_SET);
                fwrite(&anterior, sizeof(RegistroHash), 1, arqHash);
            }
            return;
        }

        if(reg.proximo == -1) break;
        
        posAnterior = ftell(arqHash) - sizeof(RegistroHash);
        anterior = reg;
        fseek(arqHash, reg.proximo * sizeof(RegistroHash), SEEK_SET);
        fread(&reg, sizeof(RegistroHash), 1, arqHash);
    }
}

void imprimirHash(FILE *arqHash) {
    printf("\n--- IMPRESSAO DA TABELA HASH ---\n");
    RegistroHash reg;

    for (int i = 0; i < TAMANHO_TABELA; i++) {
        printf("Compartimento [%d]: ", i);
        fseek(arqHash, i * sizeof(RegistroHash), SEEK_SET);
        fread(&reg, sizeof(RegistroHash), 1, arqHash);

        if (reg.codigo == -1) {
            printf("Vazio\n");
            continue;
        }

        int proximo = i;
        int primeiro = 1;
        while (proximo != -1) {
            if (!primeiro) {
                printf(" -> ");
            }
            fseek(arqHash, proximo * sizeof(RegistroHash), SEEK_SET);
            fread(&reg, sizeof(RegistroHash), 1, arqHash);

            if (reg.codigo != -1) {
                printf("ID %d", reg.codigo);
            }
            proximo = reg.proximo;
            primeiro = 0;
        }
        printf("\n");
    }
    printf("--- FIM DA TABELA HASH ---\n");
}

// VERSÃO CORRIGIDA
void popularHashComClientesIniciais(FILE *arqHash, FILE *arqClientes) {
    rewind(arqClientes);
    TCliente *c;
    long posCliente;

    while (1) {
        posCliente = ftell(arqClientes);
        c = leCliente(arqClientes);
        if (c == NULL) {
            break;
        }

        int posHash = funcaoHash(c->id);
        RegistroHash reg;
        long posRegistroAtual = posHash * sizeof(RegistroHash);

        fseek(arqHash, posRegistroAtual, SEEK_SET);
        fread(&reg, sizeof(RegistroHash), 1, arqHash);

        if (reg.codigo != -1) { // Colisão
            while (reg.proximo != -1) {
                posRegistroAtual = reg.proximo * sizeof(RegistroHash);
                fseek(arqHash, posRegistroAtual, SEEK_SET);
                fread(&reg, sizeof(RegistroHash), 1, arqHash);
            }

            int novaPosicaoOverflow = TAMANHO_TABELA;
            RegistroHash regTemp;
            while (1) {
                fseek(arqHash, novaPosicaoOverflow * sizeof(RegistroHash), SEEK_SET);
                if (fread(&regTemp, sizeof(RegistroHash), 1, arqHash) != 1 || regTemp.codigo == -1) {
                    break;
                }
                novaPosicaoOverflow++;
            }

            reg.proximo = novaPosicaoOverflow;
            fseek(arqHash, posRegistroAtual, SEEK_SET); // Volta para a posição do último nó
            fwrite(&reg, sizeof(RegistroHash), 1, arqHash);

            posHash = novaPosicaoOverflow;
        }

        reg.codigo = c->id;
        reg.posicao = posCliente;
        reg.proximo = -1;

        fseek(arqHash, posHash * sizeof(RegistroHash), SEEK_SET);
        fwrite(&reg, sizeof(RegistroHash), 1, arqHash);

        free(c);
    }
}