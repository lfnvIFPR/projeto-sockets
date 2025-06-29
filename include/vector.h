#pragma once
#include <stddef.h>

#include "stringDinamica.h"
typedef unsigned char byte;

typedef struct vector_byte {
    byte *data;
    size_t tamanho;
    size_t capacidade;
} vector_byte;

vector_byte criarBytes(size_t capacidade);
vector_byte moverString(string* str);
vector_byte copiarString(const string *str);

vector_byte lerArquivoBinario(const char *nomeArquivo);

byte pop(vector_byte* vec);

void concatenarByte(vector_byte *vec, byte byte);
void concatenarBytes(vector_byte *vec, const byte *bytes, size_t tamanho);
void concatenarVetorBytes(vector_byte *vec, vector_byte *outro_vec);
void apagarBytes(vector_byte *vec);

