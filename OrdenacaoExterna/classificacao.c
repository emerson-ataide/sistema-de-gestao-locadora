#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classificacao.h"

void salvarDadosClientes(int comparacoes, double tempoExecucao)
{
    FILE *arquivo = fopen("log_classificacao.txt", "a");    
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return;
    
    }

    fprintf(arquivo, "Comparacoes: %d\n", comparacoes);
    fprintf(arquivo, "Tempo de execucao: %.2f segundos\n", tempoExecucao);
    fclose(arquivo);
}


void salvarDadosClassificacao(double tempoExecucao, int numParticoes) {
    FILE *arquivo = fopen("log_classificacao.txt", "a"); // Nome do arquivo atualizado
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


void trocarCliente(TCliente *a, TCliente *b)
{
    TCliente temp = *a;
    *a = *b;
    *b = temp;
}

void memMinimaCliente(TCliente heap[], int n, int i)
{
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && heap[esq].id < heap[menor].id)
    {
        menor = esq;
    }

    if (dir < n && heap[dir].id < heap[menor].id)
    {
        menor = dir;
    }

    if (menor != i)
    {
        trocarCliente(&heap[i], &heap[menor]);
        memMinimaCliente(heap, n, menor);
    }
}

TCliente extrairMinCliente(TCliente memoria[], int *n)
{
    if (*n <= 0)
    {
        return (TCliente){0}; // Retorna um cliente vazio se não houver elementos
    }

    TCliente min = memoria[0];
    memoria[0] = memoria[*n - 1];
    (*n)--;

    memMinimaCliente(memoria, *n, 0);
    return min;
}

void inserirMemoriaCliente(TCliente memoria[], int *n, TCliente novo)
{
    int i = (*n);
    memoria[i] = novo;
    (*n)++;

    // Corrigir a propriedade da memoria subindo o elemento
    while (i > 0 && memoria[(i - 1) / 2].id > memoria[i].id)
    {
        trocarCliente(&memoria[i], &memoria[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int selecaoNaturalCliente(FILE *entrada, int M)
{

    entrada = fopen("ArquivosDat/clientes.dat", "r+b");
    if (entrada == NULL)
    {
        printf("Erro ao abrir arquivo de entrada.\n");
        return -1;
    }

     int comparacoes = 0;
    clock_t inicio = clock();
    TCliente memoria[M];
    TCliente reservatorio[M];
    int tamanhoMemoria = 0, tamanhoReservatorio = 0;
    int fimDeArquivo = 0;

    char nomeArquivo[50];
    int numParticao = 1;
    FILE *saida;

   sprintf(nomeArquivo, "particoes/cliente_particao_%d.dat", numParticao);

    saida = fopen(nomeArquivo, "wb");
    if (saida == NULL)
    {
        printf("Erro ao criar arquivo de saida.\n");
        fclose(entrada);
        return -1;
    }

   // Passo 1: Carregar M registros do arquivo para a memória (heap)
    for (int i = 0; i < M; i++)
    {
        TCliente *cliente = leCliente(entrada);
        if (cliente)
        {
            memoria[tamanhoMemoria++] = *cliente;
            printf("Carregado na memoria: ID = %d\n", cliente->id);
            free(cliente);
        }
        else
        {
            fimDeArquivo = 1;
            printf("Fim do arquivo encontrado ao carregar memoria.\n");
            break;
        }
    }

    // Construir memória inicial antes de iniciar extrações
    for (int i = tamanhoMemoria / 2 - 1; i >= 0; i--)
    {
        memMinimaCliente(memoria, tamanhoMemoria, i);
    }

    printf("Memoria inicializada com %d registros.\n", tamanhoMemoria);

        // Extrair o menor elemento ANTES de ler um novo registro do arquivo
        while (!fimDeArquivo || tamanhoMemoria > 0)
        {
            // Passo 2: Extrai o menor registro da memória e salva na partição
            if (tamanhoMemoria > 0)
            {
                TCliente menor = extrairMinCliente(memoria, &tamanhoMemoria);
                salvaCliente(&menor, saida);
                printf("Menor registro extraido e salvo na particao: ID = %d\n", menor.id);
    
                // Passo 3: Ler um novo registro do arquivo
                TCliente *novoRegistro = leCliente(entrada);
                if (novoRegistro)
                {
                    // Se for maior ou igual ao último salvo, insere no memória
                    if (novoRegistro->id >= menor.id)
                    {
                        comparacoes++;
                        inserirMemoriaCliente(memoria, &tamanhoMemoria, *novoRegistro);
                        printf("Novo registro inserido na memoria: ID = %d\n", novoRegistro->id);
                    }
                    else
                    {
                        comparacoes++;
                        // O registro é "congelado" e movido para o reservatório sem restrições de tamanho aqui.
                        reservatorio[tamanhoReservatorio++] = *novoRegistro;
                        printf("Registro movido para reservatorio: ID = %d\n", novoRegistro->id);
                    }
                    free(novoRegistro);
                }
                else
                {
                    fimDeArquivo = 1;
                }
            }
    
            // Se a memória estiver vazia, criar uma nova partição e recarregar a memória
            if (tamanhoMemoria == 0 && tamanhoReservatorio > 0)
            {
                fclose(saida);
                printf("Particao %d finalizada.\n", numParticao);
    
                numParticao++;
                sprintf(nomeArquivo, "particoes/cliente_particao_%d.dat", numParticao);

                saida = fopen(nomeArquivo, "wb");
    
                if (!saida)
                {
                    printf("Erro ao criar novo arquivo de particao!\n");
                    fclose(entrada);
                    return -1;
                }
    
                // Reinserir registros do reservatório na memória para a próxima partição
                for (int i = 0; i < tamanhoReservatorio; i++)
                {
                    inserirMemoriaCliente(memoria, &tamanhoMemoria, reservatorio[i]);
                }
                tamanhoReservatorio = 0;
            }
        }

    fclose(saida);
    printf("Foram geradas %d particoes ordenadas.\n", numParticao);

    fclose(entrada);

    clock_t fim = clock();
    double tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    salvarDadosClassificacao(tempoExecucao, numParticao);    return numParticao;
}

void verificaParticoesCliente(int numParticoes)
{

    char nomeArquivo[50];

    for (int i = 1; i <= numParticoes; i++)
    {
        sprintf(nomeArquivo, "particoes/cliente_particao_%d.dat", i);

        FILE *arquivo = fopen(nomeArquivo, "rb");

        if (!arquivo)
        {
            printf("Erro ao abrir a particao %d\n", i);
            continue;
        }

        printf("\nConteúdo da Particao %d:\n", i);
        TCliente *cliente;
        int anterior = -1;
        int ordenado = 1;

        while ((cliente = leCliente(arquivo)))
        {
            printf("ID = %d\n", cliente->id);
            if (anterior > cliente->id)
            {
                ordenado = 0; // Se encontrar um valor fora de ordem, marca como erro
            }
            anterior = cliente->id;
            free(cliente);
        }

        fclose(arquivo);

        if (ordenado)
        {
            printf("✅ Particao %d está ordenada corretamente.\n", i);
        }
        else
        {
            printf("❌ ERRO: Particao %d não está ordenada corretamente!\n", i);
        }
    }
}