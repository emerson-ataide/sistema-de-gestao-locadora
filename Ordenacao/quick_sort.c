#include "selection_sort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include "../Entidades/entidades.h"

// Variáveis globais para contar comparações em cada tipo
static int comparacoesClientes = 0;
static int comparacoesFilmes = 0;
static int comparacoesLocacoes = 0;

// Função para salvar log
double tempo_execucao(clock_t inicio, clock_t fim) {
    return ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

void salvarDadosSelectionSort(const char *nomeArquivo, int comparacoes, double tempoExecucao) {
    FILE *log = fopen(nomeArquivo, "a");
    if (!log) return;
    fprintf(log, "Comparacoes: %d | Tempo: %.6fs\n", comparacoes, tempoExecucao);
    fclose(log);
}

// Trocas
static void trocaCliente(FILE *arq, int i, int j) {
    TCliente c1, c2;
    fseek(arq, i * sizeof(TCliente), SEEK_SET);
    fread(&c1, sizeof(TCliente), 1, arq);
    fseek(arq, j * sizeof(TCliente), SEEK_SET);
    fread(&c2, sizeof(TCliente), 1, arq);
    fseek(arq, i * sizeof(TCliente), SEEK_SET);
    fwrite(&c2, sizeof(TCliente), 1, arq);
    fseek(arq, j * sizeof(TCliente), SEEK_SET);
    fwrite(&c1, sizeof(TCliente), 1, arq);
}

static void trocaFilme(FILE *arq, int i, int j) {
    TFilme f1, f2;
    fseek(arq, i * sizeof(TFilme), SEEK_SET);
    fread(&f1, sizeof(TFilme), 1, arq);
    fseek(arq, j * sizeof(TFilme), SEEK_SET);
    fread(&f2, sizeof(TFilme), 1, arq);
    fseek(arq, i * sizeof(TFilme), SEEK_SET);
    fwrite(&f2, sizeof(TFilme), 1, arq);
    fseek(arq, j * sizeof(TFilme), SEEK_SET);
    fwrite(&f1, sizeof(TFilme), 1, arq);
}

static void trocaLocacao(FILE *arq, int i, int j) {
    TLocacao l1, l2;
    fseek(arq, i * sizeof(TLocacao), SEEK_SET);
    fread(&l1, sizeof(TLocacao), 1, arq);
    fseek(arq, j * sizeof(TLocacao), SEEK_SET);
    fread(&l2, sizeof(TLocacao), 1, arq);
    fseek(arq, i * sizeof(TLocacao), SEEK_SET);
    fwrite(&l2, sizeof(TLocacao), 1, arq);
    fseek(arq, j * sizeof(TLocacao), SEEK_SET);
    fwrite(&l1, sizeof(TLocacao), 1, arq);
}

// Ordenações com Selection Sort
void ordenarClientes(FILE *arq, int n) {
    comparacoesClientes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        TCliente min, atual;
        fseek(arq, i * sizeof(TCliente), SEEK_SET);
        fread(&min, sizeof(TCliente), 1, arq);

        for (int j = i + 1; j < n; j++) {
            fseek(arq, j * sizeof(TCliente), SEEK_SET);
            fread(&atual, sizeof(TCliente), 1, arq);
            comparacoesClientes++;
            if (atual.id < min.id) {
                min = atual;
                minIdx = j;
            }
        }
        if (minIdx != i) trocaCliente(arq, i, minIdx);
    }

    clock_t fim = clock();
    salvarDadosSelectionSort("log_selectionsort_clientes.txt", comparacoesClientes, tempo_execucao(inicio, fim));
    printf("Selection Sort Clientes - Comparacoes: %d\n", comparacoesClientes);
}

void ordenarFilmes(FILE *arq, int n) {
    comparacoesFilmes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        TFilme min, atual;
        fseek(arq, i * sizeof(TFilme), SEEK_SET);
        fread(&min, sizeof(TFilme), 1, arq);

        for (int j = i + 1; j < n; j++) {
            fseek(arq, j * sizeof(TFilme), SEEK_SET);
            fread(&atual, sizeof(TFilme), 1, arq);
            comparacoesFilmes++;
            if (atual.id < min.id) {
                min = atual;
                minIdx = j;
            }
        }
        if (minIdx != i) trocaFilme(arq, i, minIdx);
    }

    clock_t fim = clock();
    salvarDadosSelectionSort("log_selectionsort_filmes.txt", comparacoesFilmes, tempo_execucao(inicio, fim));
    printf("Selection Sort Filmes - Comparacoes: %d\n", comparacoesFilmes);
}

void ordenarLocacoes(FILE *arq, int n) {
    comparacoesLocacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        TLocacao min, atual;
        fseek(arq, i * sizeof(TLocacao), SEEK_SET);
        fread(&min, sizeof(TLocacao), 1, arq);

        for (int j = i + 1; j < n; j++) {
            fseek(arq, j * sizeof(TLocacao), SEEK_SET);
            fread(&atual, sizeof(TLocacao), 1, arq);
            comparacoesLocacoes++;
            if (atual.id < min.id) {
                min = atual;
                minIdx = j;
            }
        }
        if (minIdx != i) trocaLocacao(arq, i, minIdx);
    }

    clock_t fim = clock();
    salvarDadosSelectionSort("log_selectionsort_locacoes.txt", comparacoesLocacoes, tempo_execucao(inicio, fim));
    printf("Selection Sort Locacoes - Comparacoes: %d\n", comparacoesLocacoes);
}
