#include "utilidade.h"
#include "stringDinamica.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

string stringDinamica(size_t tamanho) {
    if (tamanho == 0) {
        string str = {
            .data = NULL,
            .tamanho = 0
        };
        return str;
    }

    string str = {
        .data = (char *) malloc((tamanho + 1) * sizeof(char)),
        .tamanho = tamanho
    };

    if (str.data == NULL) {
        sair("Erro ao alocar memoria para string dinamica");
    }
    
    str.data[0] = '\0'; // Inicializa a string como vazia
    return str;
}

string lerArquivo(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        sair("Erro ao abrir o arquivo");
    }

    fseek(arquivo, 0, SEEK_END);
    size_t tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    printf("t:%zu f:%s\n", tamanho, nomeArquivo);

    string str = stringDinamica(tamanho);
    
    fread(str.data, sizeof(char), tamanho, arquivo);
    str.data[tamanho] = '\0';

    fclose(arquivo);
    return str;
}

void apagarString(string* str) {
    if (str->data != NULL) {
        free(str->data);
    }
    str->tamanho = 0;
}

string stringLiteral(const char *literal) {
    if (literal == NULL) return stringDinamica(0);

    size_t tamanhoLiteral = strlen(literal);
    string str = stringDinamica(tamanhoLiteral);
    strcpy(str.data, literal);
    return str;
}

void concatenarString(string *str, const char *adicional) {
    if (str->data == NULL || adicional == NULL) return;

    size_t tamanhoAtual = strlen(str->data);
    size_t tamanhoAdicional = strlen(adicional);

    while (tamanhoAdicional + tamanhoAtual > str->tamanho) {
        str->data = (char *) realloc(str->data, str->tamanho * 2 * sizeof(char));
        str->tamanho *= 2;
    }

    if (str->data == NULL) {
        sair("Erro ao realocar memoria para string dinamica");
    }
    
    strcat(str->data, adicional);
}


void concatenarBinario(string *str, const char *binario, const size_t tamanho) {
    if (str->data == NULL || binario == NULL) return;

    size_t tamanhoAtual = strlen(str->data);

    while (tamanho + tamanhoAtual > str->tamanho) {
        str->data = (char *) realloc(str->data, tamanho + tamanhoAtual * sizeof(char) + 1);
        str->tamanho = tamanho + tamanhoAtual;
    }

    if (str->data == NULL) {
        sair("Erro ao realocar memoria para dados binarios");
    }
    
    memcpy(str->data + tamanhoAtual, binario, tamanho);
}


char* obterString(const string str) {
    return str.data;
}

size_t obterTamanho(const string str) {
    return str.tamanho;
}
