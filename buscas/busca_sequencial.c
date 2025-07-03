#include "busca_sequencial.h"
#include <time.h>
#include <stdio.h>

void salvarDadosSequencial(int comparacoes, double tempoExecucao) {
    FILE *log = fopen("log_sequencial.txt", "a");
    if (log == NULL) return;
    fprintf(log, "Comparacoes: %d | Tempo: %.6fs\n", comparacoes, tempoExecucao);
    fclose(log);
}

TCliente* buscaSequencialCliente(FILE *arq, int id) {
    rewind(arq);
    TCliente *c;
    int comparacoes = 0;
    clock_t inicio = clock();

    while ((c = leCliente(arq)) != NULL) {
        comparacoes++;
        if (c->id == id) {
            clock_t fim = clock();
            double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            salvarDadosSequencial(comparacoes, tempo);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return c;
        }
        free(c);
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    salvarDadosSequencial(comparacoes, tempo);
    printf("Comparacoes realizadas: %d\n", comparacoes);
    return NULL;
}

TFilme* buscaSequencialFilme(FILE *arq, int id) {
    rewind(arq);
    TFilme *f;
    int comparacoes = 0;
    clock_t inicio = clock();

    while ((f = leFilme(arq)) != NULL) {
        comparacoes++;
        if (f->id == id) {
            clock_t fim = clock();
            double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            salvarDadosSequencial(comparacoes, tempo);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return f;
        }
        free(f);
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    salvarDadosSequencial(comparacoes, tempo);
    printf("Comparacoes realizadas: %d\n", comparacoes);
    return NULL;
}

TLocacao* buscaSequencialLocacao(FILE *arq, int id) {
    rewind(arq);
    TLocacao *l;
    int comparacoes = 0;
    clock_t inicio = clock();

    while ((l = leLocacao(arq)) != NULL) {
        comparacoes++;
        if (l->id == id) {
            clock_t fim = clock();
            double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            salvarDadosSequencial(comparacoes, tempo);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return l;
        }
        free(l);
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    salvarDadosSequencial(comparacoes, tempo);
    printf("Comparacoes realizadas: %d\n", comparacoes);
    return NULL;
}
