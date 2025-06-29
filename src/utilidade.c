#include <stdio.h>
#include "utilidade.h"
#include <stdlib.h>

void sair(const char *msg) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Erro na aplicacao!\n%s", msg);
    perror(buffer);
    exit(EXIT_FAILURE);
}

