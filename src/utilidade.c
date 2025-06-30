#include "utilidade.h"
#include <stdio.h>
#include <stdlib.h>
#include "stringDinamica.h"
#include "vector.h"


void sair(const char *msg) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Erro na aplicacao!\n%s", msg);
    perror(buffer);
    exit(EXIT_FAILURE);
}

void debugBIN(vector_byte vec, size_t tamanho_string) {
    vector_byte str_vec = criarBytes(tamanho_string);
    concatenarBytes(&str_vec, vec.data, str_vec.capacidade);
    str_vec.data[tamanho_string] = '\0';
    string copia = stringLiteral((char*) str_vec.data);
    apagarBytes(&str_vec);

    puts(obterString(copia));
    printf("[");
    for (size_t i = tamanho_string; i < vec.tamanho; i++) {
        printf("U'\\%u'%s", vec.data[i], i == vec.tamanho ? "" : ", ");
    }
    printf("]\n");
    
}


