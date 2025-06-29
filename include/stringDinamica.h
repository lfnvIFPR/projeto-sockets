#pragma once
#include <stdlib.h>

typedef struct {
    char *data;
    size_t tamanho;
} string;

string stringDinamica(size_t tamanho);
string stringLiteral(const char *literal);

string lerArquivo(const char *nomeArquivo);

void apagarString(string* str);
void concatenarString(string *str, const char *adicional);
void concatenarBinario(string *str, const char *binario, size_t tamanho);
char* obterString(const string str);
size_t obterTamanho(const string str);
