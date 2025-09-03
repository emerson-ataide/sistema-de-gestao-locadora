#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "classificacao.h"

// ... (As funções auxiliares como salvarDados, trocarCliente, memMinima, etc., não mudam) ...
void salvarDadosClassificacao(double tempoExecucao, int numParticoes) {
    FILE *arquivo = fopen("log_classificacao.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de log.\n");
        return;
    }
    fprintf(arquivo, "****************************\n");
    fprintf(arquivo, "Selecao Natural - TCliente:\n");
    fprintf(arquivo, "Numero de particoes criadas: %d\n", numParticoes);
    fprintf(arquivo, "Tempo de execucao: %.2lf segundos\n", tempoExecucao);
    fprintf(arquivo, "****************************\n\n");
    fclose(arquivo);
}

void trocarCliente(TCliente *a, TCliente *b) {
    TCliente temp = *a;
    *a = *b;
    *b = temp;
}

void memMinimaCliente(TCliente heap[], int n, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
    if (esq < n && heap[esq].id < heap[menor].id) menor = esq;
    if (dir < n && heap[dir].id < heap[menor].id) menor = dir;
    if (menor != i) {
        trocarCliente(&heap[i], &heap[menor]);
        memMinimaCliente(heap, n, menor);
    }
}

TCliente extrairMinCliente(TCliente memoria[], int *n) {
    TCliente min = memoria[0];
    memoria[0] = memoria[*n - 1];
    (*n)--;
    memMinimaCliente(memoria, *n, 0);
    return min;
}

void inserirMemoriaCliente(TCliente memoria[], int *n, TCliente novo) {
    int i = (*n);
    memoria[i] = novo;
    (*n)++;
    while (i > 0 && memoria[(i - 1) / 2].id > memoria[i].id) {
        trocarCliente(&memoria[i], &memoria[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int selecaoNaturalCliente(FILE *entrada, int M) {
    entrada = fopen("ArquivosDat/clientes.dat", "r+b");
    if (entrada == NULL) {
        printf("Erro ao abrir arquivo de entrada.\n");
        return -1;
    }

    clock_t inicio = clock();
    
    
    TCliente memoria[M]; // O HEAP
    TCliente reservatorio[M]; // O RESERVATÓRIO
    int tamanhoMemoria = 0;
    int tamanhoReservatorio = 0;
   

    int fimDeArquivo = 0;
    char nomeArquivo[50];
    int numParticao = 1;
    FILE *saida;

    sprintf(nomeArquivo, "particoes/cliente_particao_%d.dat", numParticao);
    saida = fopen(nomeArquivo, "wb");

    // Passo 1: Ler M registros do arquivo para a memória (heap)
    for (int i = 0; i < M; i++) {
        TCliente *cliente = leCliente(entrada);
        if (cliente) {
            memoria[tamanhoMemoria++] = *cliente;
            free(cliente);
        } else {
            fimDeArquivo = 1;
            break;
        }
    }
    // Constrói a propriedade de heap
    for (int i = tamanhoMemoria / 2 - 1; i >= 0; i--) {
        memMinimaCliente(memoria, tamanhoMemoria, i);
    }

    while (tamanhoMemoria > 0) {
        // Passo 2 e 3: Seleciona o menor do heap e grava na partição
        TCliente menor = extrairMinCliente(memoria, &tamanhoMemoria);
        salvaCliente(&menor, saida);
        printf("Salvo na particao %d: ID = %d\n", numParticao, menor.id);

        TCliente *novoRegistro = NULL;
        if (!fimDeArquivo) {
            novoRegistro = leCliente(entrada);
            if (novoRegistro == NULL) {
                fimDeArquivo = 1;
            }
        }

        if (novoRegistro) {
            if (novoRegistro->id >= menor.id) {
                inserirMemoriaCliente(memoria, &tamanhoMemoria, *novoRegistro);
            } else {
                reservatorio[tamanhoReservatorio++] = *novoRegistro;
                printf("Registro movido para RESERVATORIO: ID = %d (Ocupacao: %d/%d)\n", novoRegistro->id, tamanhoReservatorio, M);
            }
            free(novoRegistro);
        }


        if (tamanhoReservatorio >= M || (fimDeArquivo && tamanhoMemoria == 0)) {
            
            // A. Finaliza a partição atual esvaziando o que resta no heap
            printf("(Reservatorio cheio ou Fim de arquivo) ---\n");
            printf("Esvaziando o resto do HEAP na particao %d...\n", numParticao);
            while (tamanhoMemoria > 0) {
                TCliente resto = extrairMinCliente(memoria, &tamanhoMemoria);
                salvaCliente(&resto, saida);
                printf("Salvo  na particao %d: ID = %d\n", numParticao, resto.id);
            }
            fclose(saida);
            printf("--- Particao %d finalizada. ---\n", numParticao);

            // Se não há mais nada no reservatório, o processo acabou.
            if (tamanhoReservatorio == 0) break;

            // B. Inicia uma nova partição
            numParticao++;
            sprintf(nomeArquivo, "particoes/cliente_particao_%d.dat", numParticao);
            saida = fopen(nomeArquivo, "wb");
            printf("\n--- Particao %d iniciada ---\n", numParticao);

            // C. Copia os registros do RESERVATÓRIO para o HEAP
            printf("Copiando %d registros do RESERVATORIO para o HEAP...\n", tamanhoReservatorio);
            for (int i = 0; i < tamanhoReservatorio; i++) {
                memoria[i] = reservatorio[i];
            }
            tamanhoMemoria = tamanhoReservatorio;
            tamanhoReservatorio = 0; // Esvazia o reservatório

            // D. Reconstrói a propriedade de heap para os novos dados
            for (int i = tamanhoMemoria / 2 - 1; i >= 0; i--) {
                memMinimaCliente(memoria, tamanhoMemoria, i);
            }
        }
        // =================================================================================
    }

    if(saida) fclose(saida);
    fclose(entrada);

    clock_t fim = clock();
    double tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    salvarDadosClassificacao(tempoExecucao, numParticao);
    return numParticao;
}