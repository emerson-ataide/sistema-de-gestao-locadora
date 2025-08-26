#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// ##################################### DECLARAÇÕES ENTIDADE FILME #####################################

typedef struct Filme {
    int id;
    char titulo[100];
    char genero[50];
} TFilme;

int tamanho_registroFilme();
TFilme* filme(int id, char *titulo, char *genero);
void salvaFilme(TFilme *filme, FILE *out);
TFilme* leFilme(FILE *in);                    
void imprimirFilme(TFilme *filme);
void criarBaseFilme(FILE *out, int tam);
void imprimirBaseFilme(FILE *out);
int tamanho_arquivoFilme(FILE *arq);
void shuffleFilme(int *vet, int TAM);
void ordenarFilmes(FILE *arq, int tam);       

// ##################################### DECLARAÇÕES ENTIDADE CLIENTE #####################################

typedef struct Cliente {
    int id;
    char nome[100];
    char cpf[15];
    char telefone[20];
} TCliente;

int tamanho_registroCliente();
TCliente* cliente(int id, char *nome, char *cpf, char *telefone);
void salvaCliente(TCliente *cliente, FILE *out);
TCliente* leCliente(FILE *in);                
void imprimirCliente(TCliente *cliente);
void criarBaseCliente(FILE *out, int tam);
void imprimirBaseCliente(FILE *out);
int tamanho_arquivoCliente(FILE *arq);
void shuffleCliente(int *vet, int TAM);
void ordenarClientes(FILE *arq, int tam);     

// ##################################### DECLARAÇÕES ENTIDADE LOCACAO #####################################

typedef struct Locacao {
    int id;
    int idCliente;
    int idFilme;
    char dataLocacao[11];   
    char dataDevolucao[11]; 
    char dataDevolvida[11]; 
    char status[30];
    double multa;           
    int devolvido;          
} TLocacao;

int tamanho_registroLocacao();
TLocacao* locacao(int id, int idFilme, int idCliente, char *dataLocacao, char *dataDevolucao, int devolvido);
void salvaLocacao(TLocacao *locacao, FILE *out);
TLocacao* leLocacao(FILE *in);                
void criarBaseLocacao(FILE *out, int tam);
void imprimirBaseLocacao(FILE *arqLocacoes, FILE *arqClientes, FILE *arqFilmes);
int tamanho_arquivoLocacao(FILE *arq);
void shuffleLocacao(int *vet, int TAM);

#endif