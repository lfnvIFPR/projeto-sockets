#include "vector.h"
#include "stringDinamica.h"
#include "utilidade.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

vector_byte criarBytes(size_t capacidade) {
      if (capacidade == 0) {
        vector_byte vec = {
            .data = NULL,
            .tamanho = 0,
            .capacidade = 0
        };
        return vec;
    }

    vector_byte vec = {
        .data = (byte *) malloc(capacidade * sizeof(byte)),
        .tamanho = 0,
        .capacidade = capacidade
    };

    if (vec.data == NULL) {
        sair("Erro ao alocar memoria para vetor de bytes");
    }
    
    return vec;
}

vector_byte lerArquivoBinario(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        sair("Erro ao abrir o arquivo");
    }

    fseek(arquivo, 0, SEEK_END);
    size_t tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    vector_byte vec = criarBytes(tamanho);
    
    fread(vec.data, sizeof(char), tamanho, arquivo);
    
    vec.tamanho = tamanho;
    fclose(arquivo);
    return vec;
}

vector_byte moverString(string* str) {
    size_t tamanho = str->tamanho + 1;
    vector_byte vec = criarBytes(tamanho);
    memcpy(vec.data, str->data, tamanho);
    vec.tamanho = tamanho;
    apagarString(str);
    return vec;
}

byte pop(vector_byte* vec) {
    if (vec->tamanho == 0) return U'\0';
    vec->tamanho -= 1;
    return vec->data[vec->tamanho];
}

vector_byte copiarString(const string *str) {
    size_t tamanho = str->tamanho + 1;
    vector_byte vec = criarBytes(tamanho);
    memcpy(vec.data, str->data, tamanho);
    return vec;
}

void concatenarByte(vector_byte *vec, byte val) {
    if (vec->tamanho + 1 > vec->capacidade) {
        vec->data = (byte *) realloc(vec->data, vec->capacidade * 2 * sizeof(byte));
        vec->capacidade *= 2;
    }

    vec->data[vec->tamanho] = val;
    vec->tamanho += 1;
}

void concatenarBytes(vector_byte *vec, const byte *bytes, size_t tamanho) {
    while (vec->tamanho + tamanho > vec->capacidade) {
        vec->data = (byte *) realloc(vec->data, vec->capacidade * 2 * sizeof(byte));
        vec->capacidade *= 2;
    }

    memcpy(vec->data + vec->tamanho, bytes, tamanho);
    vec->tamanho += tamanho;
}

/// !!! Apagará o segundo parâmetro. !!!
void concatenarVetorBytes(vector_byte *vec, vector_byte *outro_vec) {
    concatenarBytes(vec, outro_vec->data, outro_vec->tamanho);
    apagarBytes(outro_vec);
}

void apagarBytes(vector_byte *vec) {
    if (vec == NULL || vec->data == NULL) {
        return;
    }
    free(vec->data);
    vec->capacidade = 0;
    vec->tamanho = 0;
    vec->data = NULL;
}