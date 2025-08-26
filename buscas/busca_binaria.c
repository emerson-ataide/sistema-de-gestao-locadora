#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "busca_binaria.h"

void salvarDadosBinaria(int comparacoes, double tempoExecucao) {
    FILE *log = fopen("log_binaria.txt", "a");
    if (log == NULL) return;
    fprintf(log, "Comparacoes: %d | Tempo: %.6fs\n", comparacoes, tempoExecucao);
    fclose(log);
}

TCliente* buscaBinariaCliente(FILE *arq, int id, int n) {
    int inicio = 0, fim = n - 1;
    int comparacoes = 0;
    clock_t inicioTempo = clock();

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        fseek(arq, meio * sizeof(TCliente), SEEK_SET);

        TCliente *c = (TCliente*) malloc(sizeof(TCliente));
        if (fread(c, sizeof(TCliente), 1, arq) != 1) {
            free(c);
            break;
        }

        comparacoes++;
        if (c->id == id) {
            clock_t fimTempo = clock();
            double tempo = ((double)(fimTempo - inicioTempo)) / CLOCKS_PER_SEC;
            salvarDadosBinaria(comparacoes, tempo);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return c;
        } else if (c->id < id) {
            free(c);
            inicio = meio + 1;
        } else {
            free(c);
            fim = meio - 1;
        }
    }

    clock_t fimTempo = clock();
    double tempo = ((double)(fimTempo - inicioTempo)) / CLOCKS_PER_SEC;
    salvarDadosBinaria(comparacoes, tempo);
    printf("Comparacoes realizadas: %d\n", comparacoes);
    return NULL;
}

TFilme* buscaBinariaFilme(FILE *arq, int id, int n) {
    int inicio = 0, fim = n - 1;
    int comparacoes = 0;
    clock_t inicioTempo = clock();

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        fseek(arq, meio * sizeof(TFilme), SEEK_SET);

        TFilme *f = (TFilme*) malloc(sizeof(TFilme));
        if (fread(f, sizeof(TFilme), 1, arq) != 1) {
            free(f);
            break;
        }

        comparacoes++;
        if (f->id == id) {
            clock_t fimTempo = clock();
            double tempo = ((double)(fimTempo - inicioTempo)) / CLOCKS_PER_SEC;
            salvarDadosBinaria(comparacoes, tempo);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return f;
        } else if (f->id < id) {
            free(f);
            inicio = meio + 1;
        } else {
            free(f);
            fim = meio - 1;
        }
    }

    clock_t fimTempo = clock();
    double tempo = ((double)(fimTempo - inicioTempo)) / CLOCKS_PER_SEC;
    salvarDadosBinaria(comparacoes, tempo);
    printf("Comparacoes realizadas: %d\n", comparacoes);
    return NULL;
}

TLocacao* buscaBinariaLocacao(FILE *arq, int id, int n) {
    int inicio = 0, fim = n - 1;
    int comparacoes = 0;
    clock_t inicioTempo = clock();

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        fseek(arq, meio * sizeof(TLocacao), SEEK_SET);

        TLocacao *l = (TLocacao*) malloc(sizeof(TLocacao));
        if (fread(l, sizeof(TLocacao), 1, arq) != 1) {
            free(l);
            break;
        }

        comparacoes++;
        if (l->id == id) {
            clock_t fimTempo = clock();
            double tempo = ((double)(fimTempo - inicioTempo)) / CLOCKS_PER_SEC;
            salvarDadosBinaria(comparacoes, tempo);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return l;
        } else if (l->id < id) {
            free(l);
            inicio = meio + 1;
        } else {
            free(l);
            fim = meio - 1;
        }
    }

    clock_t fimTempo = clock();
    double tempo = ((double)(fimTempo - inicioTempo)) / CLOCKS_PER_SEC;
    salvarDadosBinaria(comparacoes, tempo);
    printf("Comparacoes realizadas: %d\n", comparacoes);
    return NULL;
}
