#include "menuConsole.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Entidades/entidades.h"
#include "../buscas/busca_binaria.h"
#include "../buscas/busca_sequencial.h"
#include "../OrdenacaoExterna/intercalacao.h"
#include "../OrdenacaoExterna/classificacao.h"
#include "../Ordenacao/selection_sort.h"
#include "menuHash.c"
#include "menuHash.h"

#define M_SELECAO_NATURAL 7

// Função para calcular quantos registros tem no arquivo
int tamanhoArquivo(FILE *arq, size_t tamanhoRegistro) {
    fseek(arq, 0, SEEK_END);
    long tamanhoArquivo = ftell(arq);
    rewind(arq);
    return (int)(tamanhoArquivo / tamanhoRegistro);
}

void exibirMenuPrincipal(FILE *arqClientes, FILE *arqFilmes, FILE *arqLocacoes, FILE *arqHash) {
    int opcao;
    do {
        printf("\n=========== MENU PRINCIPAL ===========\n");
        printf("[1] Realizar uma locacao\n");
        printf("[2] Listar locacoes\n");
        printf("[3] Realizar busca\n");
        printf("[4] Ordenar bases com Selection Sort\n");
        printf("[5] Imprimir bases\n");
        printf("[6] Ordenacao Externa\n");
        printf("[7] Operacoes Hash\n");
        printf("[0] Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int idLocacao = tamanho_arquivoLocacao(arqLocacoes) + 1;
                int idFilme, idCliente;
                char dataLocacao[11], dataDevolucao[11];
                int devolvido;
                float multa = 0.0f;

                printf("\n-- Nova Locacao --\n");
                printf("ID do filme: ");
                scanf("%d", &idFilme);
                printf("ID do cliente: ");
                scanf("%d", &idCliente);
                printf("Data da locacao (YYYY-MM-DD): ");
                scanf("%s", dataLocacao);
                printf("Data prevista para devolucao (YYYY-MM-DD): ");
                scanf("%s", dataDevolucao);

                printf("O filme foi devolvido no prazo? (1-Sim / 0-Nao): ");
                scanf("%d", &devolvido);

                if (devolvido == 0) {
                    multa = 10.0f;
                    printf("Multa aplicada: R$ %.2f\n", multa);
                }

                TLocacao *l = locacao(idLocacao, idFilme, idCliente, dataLocacao, dataDevolucao, devolvido);
                l->multa = multa;

                salvaLocacao(l, arqLocacoes);
                printf("Locacao registrada com sucesso.\n");
                free(l);
                break;
            }

            case 2:
                printf("\n-- Locacoes Registradas --\n");
                imprimirBaseLocacao(arqLocacoes, arqClientes, arqFilmes);
                break;

            case 3: {
                int tipoBusca, entidade, idBusca;
                printf("\nEscolha o tipo de busca:\n");
                printf("[1] Busca Sequencial\n");
                printf("[2] Busca Binaria\n");
                printf("Opcao: ");
                scanf("%d", &tipoBusca);

                printf("\nEscolha o que buscar:\n");
                printf("[1] Cliente\n");
                printf("[2] Filme\n");
                printf("[3] Locacao\n");
                printf("Opcao: ");
                scanf("%d", &entidade);

                int maxID = 0;
                switch (entidade) {
                    case 1: maxID = tamanhoArquivo(arqClientes, sizeof(TCliente)); break;
                    case 2: maxID = tamanhoArquivo(arqFilmes, sizeof(TFilme)); break;
                    case 3: maxID = tamanhoArquivo(arqLocacoes, sizeof(TLocacao)); break;
                    default: break;
                }

                switch (entidade) {
                    case 1: printf("Digite o ID de cliente para buscar (entre 1 e %d): ", maxID); break;
                    case 2: printf("Digite o ID de filme para buscar (entre 1 e %d): ", maxID); break;
                    case 3: printf("Digite o ID de locacao para buscar (entre 1 e %d): ", maxID); break;
                    default: printf("Digite o ID para buscar: "); break;
                }
                scanf("%d", &idBusca);

                if (tipoBusca == 1) {
                    switch(entidade) {
                        case 1: {
                            TCliente *c = buscaSequencialCliente(arqClientes, idBusca);
                            if (c) {
                                printf("Cliente encontrado: ID: %d | Nome: %s\n", c->id, c->nome);
                                free(c);
                            } else {
                                printf("Cliente com ID %d nao encontrado.\n", idBusca);
                            }
                            break;
                        }
                        case 2: {
                            TFilme *f = buscaSequencialFilme(arqFilmes, idBusca);
                            if (f) {
                                printf("Filme encontrado: ID: %d | Titulo: %s\n", f->id, f->titulo);
                                free(f);
                            } else {
                                printf("Filme com ID %d nao encontrado.\n", idBusca);
                            }
                            break;
                        }

                        case 3: {
                            TLocacao *l = buscaSequencialLocacao(arqLocacoes, idBusca);
                            if (l) {
                                printf("Locacao encontrada: ID: %d\n", l->id);
                                free(l);
                            } else {
                                printf("Locacao com ID %d nao encontrada.\n", idBusca);
                            }
                            break;
                        }
                        default: printf("Opcao invalida para entidade.\n");
                    }
                } else if (tipoBusca == 2) {
                    int n = 0;
                    switch(entidade) {
                        case 1: {
                            n = tamanhoArquivo(arqClientes, sizeof(TCliente));
                            TCliente *c = buscaBinariaCliente(arqClientes, idBusca, n);
                            if (c) {
                                printf("Cliente encontrado: ID: %d | Nome: %s\n", c->id, c->nome);
                                free(c);
                            } else {
                                printf("Cliente com ID %d nao encontrado.\n", idBusca);
                            }
                            break;
                        }
                        case 2: {
                            n = tamanhoArquivo(arqFilmes, sizeof(TFilme));
                            TFilme *f = buscaBinariaFilme(arqFilmes, idBusca, n);
                            if (f) {
                                printf("Filme encontrado: ID: %d | Titulo: %s\n", f->id, f->titulo);
                                free(f);
                            } else {
                                printf("Filme com ID %d nao encontrado.\n", idBusca);
                            }
                            break;
                        }

                        case 3: {
                            n = tamanhoArquivo(arqLocacoes, sizeof(TLocacao));
                            TLocacao *l = buscaBinariaLocacao(arqLocacoes, idBusca, n);
                            if (l) {
                                printf("Locacao encontrada: ID: %d\n", l->id);
                                free(l);
                            } else {
                                printf("Locacao com ID %d nao encontrada.\n", idBusca);
                            }
                            break;
                        }
                        default: printf("Opcao invalida para entidade.\n");
                    }
                } else {
                    printf("Tipo de busca invalido.\n");
                }
                break;
            }

            case 4: {
                int tamClientes = tamanhoArquivo(arqClientes, sizeof(TCliente));
                int tamFilmes = tamanhoArquivo(arqFilmes, sizeof(TFilme));

                ordenarClientes(arqClientes, tamClientes);
                ordenarFilmes(arqFilmes, tamFilmes);
                printf("Bases ordenadas com sucesso!\n");
                break;
            }

            case 5:
                printf("\n========== BASE DE CLIENTES ==========\n");
                imprimirBaseCliente(arqClientes);
                printf("\n========== BASE DE FILMES ==========\n");
                imprimirBaseFilme(arqFilmes);
                break;

            case 6: { //  Ordenação Externa
                int subOpcao;
                int numParticoes;

                printf("\n--- Ordenacao Externa ---\n");
                printf("[1] Gerar particoes e classificar por Selecao Natural\n");
                printf("[2] Intercalar por Intercalaco Otima\n");
                printf("[3] Voltar\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &subOpcao);

                if (subOpcao == 1) {
                    printf("\n[1] Base de dados de clientes\n");
                    printf("[2] Voltar\n");
                    printf("Escolha uma opcao: ");
                    scanf("%d", &opcao);

                    if (opcao == 1) {
                        printf("Valor de M fixado em %d.\n", M_SELECAO_NATURAL);

                        #ifdef _WIN32
                            system("mkdir particoes > nul 2>nul");
                        #else
                            system("mkdir -p particoes");
                        #endif
                        
                        #ifdef _WIN32
                            system("del /Q particoes\\*.dat");
                        #else
                            system("rm -f particoes/*.dat");
                        #endif
                    
                        numParticoes = selecaoNaturalCliente(arqClientes, M_SELECAO_NATURAL);

                        FILE *temp = fopen("num_particoes.tmp", "w");
                        if (temp != NULL) {
                            fprintf(temp, "%d", numParticoes);
                            fclose(temp);
                        }

                        if (numParticoes > 0) {
                            printf("Particoes geradas com sucesso: %d particoes.\n", numParticoes);
                        } else {
                            printf("Erro ao gerar particoes.\n");
                        }
                    } else if (opcao == 2) {
                        break; 
                    } else {
                        printf("Opcao invalida.\n");
                    }
                }
                else if (subOpcao == 2) {
                    int fatorDeIntercalacao;

                    FILE *temp = fopen("num_particoes.tmp", "r");
                    if (temp != NULL) {
                        fscanf(temp, "%d", &numParticoes);
                        fclose(temp);
                    } else {
                        printf("Erro: numero de particoes nao encontrado. Gere as particoes primeiro.\n");
                        break;
                    }

                    printf("Digite o fator de intercalacao: ");
                    scanf("%d", &fatorDeIntercalacao);

                    intercalacaoOtimaCliente(numParticoes, fatorDeIntercalacao);
                    printf("Intercalacao realizada com sucesso.\n");
                }
                else if (subOpcao == 3) {
                    break; 
                }
                else {
                    printf("Opção invalida.\n");
                }

                break;
            }

            case 7:
                menuHash(arqClientes, arqHash);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);
}