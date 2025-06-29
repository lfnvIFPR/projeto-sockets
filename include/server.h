#pragma once
#include <netinet/in.h>
#include <sys/socket.h>

#include "stringDinamica.h"

typedef struct server {
    int ip_ver;
    int porta;
    int servico;
    int tipo;
    int backlog;
    struct sockaddr_in endereco;

    string index_html;

    int socket_fd;

    void (*ler_index)(struct server *sv, const char *nomeArquivo);
    void (*iniciar)(struct server *sv);

} server;

server criarServer(int ip_ver, int porta);