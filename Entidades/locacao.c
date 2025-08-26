#include "entidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int tamanho_registroLocacao() {
    return sizeof(TLocacao);
}

TLocacao* locacao(int id, int idFilme, int idCliente, char *dataLocacao, char *dataDevolucao, int devolvido) {
    TLocacao *l = (TLocacao*) malloc(sizeof(TLocacao));
    l->id = id;
    l->idFilme = idFilme;
    l->idCliente = idCliente;
    strcpy(l->dataLocacao, dataLocacao);
    strcpy(l->dataDevolucao, dataDevolucao);
    strcpy(l->dataDevolvida, devolvido ? dataDevolucao : "\0");
    strcpy(l->status, devolvido ? "Devolvido" : "Em aberto");
    l->multa = 0.0;
    return l;
}

void salvaLocacao(TLocacao *loc, FILE *out) {
    fseek(out, 0, SEEK_END);
    fwrite(loc, sizeof(TLocacao), 1, out);
}

TLocacao* leLocacao(FILE *in) {
    TLocacao *l = (TLocacao*) malloc(sizeof(TLocacao));
    if (fread(l, sizeof(TLocacao), 1, in)) return l;
    free(l);
    return NULL;
}

// Função auxiliar: calcula o total aproximado de dias desde o "início"
int diasDesdeInicioSimples(int dia, int mes, int ano) {
    return dia + mes * 30 + ano * 365;
}

// Função principal: calcula diferença aproximada entre duas datas no formato "YYYY-MM-DD"
int diferencaDiasSimples(char *data1, char *data2) {
    int d1, m1, a1;
    int d2, m2, a2;

    sscanf(data1, "%4d-%2d-%2d", &a1, &m1, &d1);
    sscanf(data2, "%4d-%2d-%2d", &a2, &m2, &d2);

    int totalDias1 = diasDesdeInicioSimples(d1, m1, a1);
    int totalDias2 = diasDesdeInicioSimples(d2, m2, a2);

    return abs(totalDias2 - totalDias1);
}


// NOVA IMPLEMENTAÇÃO substituindo imprimirBaseLocacao
void imprimirBaseLocacao(FILE *arqLocacoes, FILE *arqClientes, FILE *arqFilmes) {
    rewind(arqLocacoes);
    TLocacao *l;
    while ((l = leLocacao(arqLocacoes)) != NULL) {
        // Busca cliente
        rewind(arqClientes);
        TCliente *c = NULL, *cTemp;
        while ((cTemp = leCliente(arqClientes)) != NULL) {
            if (cTemp->id == l->idCliente) {
                c = cTemp;
                break;
            }
            free(cTemp);
        }

        // Busca filme
        rewind(arqFilmes);
        TFilme *f = NULL, *fTemp;
        while ((fTemp = leFilme(arqFilmes)) != NULL) {
            if (fTemp->id == l->idFilme) {
                f = fTemp;
                break;
            }
            free(fTemp);
        }

        // Impressão
        printf("ID Locacao: %d\n", l->id);
        printf("Cliente: %s\n", c ? c->nome : "Nao encontrado");
        printf("Filme: %s\n", f ? f->titulo : "Nao encontrado");
        printf("Data Locacao: %s\n", l->dataLocacao);
        printf("Data Devolucao: %s\n", l->dataDevolucao);

        int dias = diferencaDiasSimples(l->dataLocacao, l->dataDevolucao);
        double valor = dias * 2.0;
        printf("Valor da locacao: R$%.2f (%d dias)\n", valor, dias);

        printf("-------------------------\n");

        free(l);
        if (c) free(c);
        if (f) free(f);
    }
}

void criarBaseLocacao(FILE *out, int tam) {
    if (out == NULL) {
        printf("Arquivo invalido para criar base de locacoes.\n");
        return;
    }

    for (int i = 0; i < tam; i++) {
        int id = i + 1;
        int idFilme, idCliente;
        char dataLocacao[11], dataDevolucao[11];
        int devolvido;

        printf("\nCriando locacao %d de %d\n", id, tam);

        printf("Digite o ID do Filme: ");
        scanf("%d", &idFilme);

        printf("Digite o ID do Cliente: ");
        scanf("%d", &idCliente);

        printf("Digite a data da locação (AAAA-MM-DD): ");
        scanf("%10s", dataLocacao);

        printf("Digite a data da devolução prevista (AAAA-MM-DD): ");
        scanf("%10s", dataDevolucao);

        printf("Devolvido? (0 - Não, 1 - Sim): ");
        scanf("%d", &devolvido);

        TLocacao *l = locacao(id, idFilme, idCliente, dataLocacao, dataDevolucao, devolvido);
        salvaLocacao(l, out);
        free(l);
    }
}

void criarBaseLocacaoOrdenada(FILE *out, int tam) {
    criarBaseLocacao(out, tam);
}

int tamanho_arquivoLocacao(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = ftell(arq) / sizeof(TLocacao);
    rewind(arq);
    return tam;
}

void shuffleLocacao(int *vet, int TAM) {
    srand(time(NULL));
    for (int i = 0; i < TAM; i++) {
        int r = rand() % TAM;
        int temp = vet[i];
        vet[i] = vet[r];
        vet[r] = temp;
    }
}
