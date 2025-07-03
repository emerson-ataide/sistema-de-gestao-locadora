
#include "entidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Retorna o tamanho em bytes de um registro Filme
int tamanho_registroFilme() {
    return sizeof(TFilme);
}

// Cria um novo filme em memória e inicializa seus dados
TFilme* filme(int id, char *titulo, char *genero) {
    TFilme *f = (TFilme*) malloc(sizeof(TFilme));
    if (f == NULL) {
        printf("Erro ao alocar memória para filme.\n");
        exit(1);
    }
    f->id = id;
    strncpy(f->titulo, titulo, sizeof(f->titulo));
    f->titulo[sizeof(f->titulo)-1] = '\0'; // segurança na string
    strncpy(f->genero, genero, sizeof(f->genero));
    f->genero[sizeof(f->genero)-1] = '\0';
    return f;
}

// Salva um filme em arquivo binário (append)
void salvaFilme(TFilme *filme, FILE *out) {
    fwrite(filme, tamanho_registroFilme(), 1, out);
}

// Lê um filme de arquivo binário (na posição atual do cursor)
TFilme* leFilme(FILE *in) {
    TFilme *f = (TFilme*) malloc(sizeof(TFilme));
    if (f == NULL) {
        printf("Erro ao alocar memória para leitura de filme.\n");
        exit(1);
    }
    if (fread(f, tamanho_registroFilme(), 1, in) != 1) {
        free(f);
        return NULL; // fim do arquivo ou erro
    }
    return f;
}

// Imprime as informações de um filme na tela
void imprimirFilme(TFilme *filme) {
    printf("ID: %d\n", filme->id);
    printf("Titulo: %s\n", filme->titulo);
    printf("Genero: %s\n", filme->genero);
    printf("---------------------------\n");
}

// Cria base de dados com filmes aleatórios desordenados
void criarBaseFilme(FILE *out, int tam) {
    if (out == NULL) {
        printf("Arquivo invalido para criar base de filmes.\n");
        return;
    }

    char *titulos[] = {"Panico", "Harry Potter", "Titanic", "Missao Impossivel", "Barbie"};
    char *generos[] = {"Fantasia", "Romance", "Drama", "Aventura", "Terror"};

    int ids[tam];
    for (int i = 0; i < tam; i++) {
        ids[i] = i + 1;
    }

    shuffleFilme(ids, tam);

    srand(time(NULL));

    for (int i = 0; i < tam; i++) {
        int id = ids[i];
        char *titulo = titulos[rand() % 5];
        char *genero = generos[rand() % 5];

        TFilme *f = filme(id, titulo, genero);
        salvaFilme(f, out);
        free(f);
    }
}

// Imprime todos os filmes de um arquivo
void imprimirBaseFilme(FILE *out) {
    if (out == NULL) {
        printf("Arquivo inválido para imprimir base de filmes.\n");
        return;
    }
    rewind(out);
    TFilme *f;
    while ((f = leFilme(out)) != NULL) {
        imprimirFilme(f);
        free(f);
    }
    rewind(out);
}

// Retorna o tamanho do arquivo (em bytes)
int tamanho_arquivoFilme(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tamanho = ftell(arq);
    rewind(arq);
    return tamanho;
}

// Embaralha vetor de inteiros - para criar base desordenada
void shuffleFilme(int *vet, int TAM) {
    srand(time(NULL));
    for (int i = TAM - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vet[i];
        vet[i] = vet[j];
        vet[j] = temp;
    }
}
