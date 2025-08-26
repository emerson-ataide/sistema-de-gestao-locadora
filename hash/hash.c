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

void inserirHash(FILE *arqHash, FILE *arqCliente, TCliente cliente) {
    int pos = funcaoHash(cliente.id);
    RegistroHash reg;
    
    fseek(arqHash, pos * sizeof(RegistroHash), SEEK_SET);
    fread(&reg, sizeof(RegistroHash), 1, arqHash);
    
    if(reg.codigo != -1) {
        while(reg.proximo != -1) {
            fseek(arqHash, reg.proximo * sizeof(RegistroHash), SEEK_SET);
            fread(&reg, sizeof(RegistroHash), 1, arqHash);
        }
        
        int novaPosicao = TAMANHO_TABELA;
        RegistroHash regTemp;
        while(1) {
            fseek(arqHash, novaPosicao * sizeof(RegistroHash), SEEK_SET);
            if (fread(&regTemp, sizeof(RegistroHash), 1, arqHash) != 1 || regTemp.codigo == -1) {
                break;
            }
            novaPosicao++;
        }
        
        reg.proximo = novaPosicao;
        fseek(arqHash, -sizeof(RegistroHash), SEEK_CUR);
        fwrite(&reg, sizeof(RegistroHash), 1, arqHash);
        
        pos = novaPosicao;
    }
    
    fseek(arqCliente, 0, SEEK_END);
    long posicaoCliente = ftell(arqCliente);
    salvaCliente(&cliente, arqCliente);
    
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