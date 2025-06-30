#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <sys/socket.h>
#include <unistd.h>

#include "server.h"
#include "vector.h"
#include "stringDinamica.h"
#include "utilidade.h"

#define MAX_TAMANHO 4096



/**
 * @brief Gera e retorna a resposta HTTP apropriada para uma requisição recebida.
 *
 * Esta função recebe uma requisição HTTP, a processa e constrói a resposta HTTP correspondente.
 * Caso a requisão seja inválida, retorna uma resposta HTTP 400
 *
 * @param server Ponteiro para o struct do servidor que está processando a requisição.
 * @param requisicao String contendo a requisição HTTP recebida.
 * @return Um vetor de bytes contendo a resposta HTTP gerada, normalmente em texto.
 */
vector_byte responder(server* server, const char* requisicao) {
    char copia[MAX_TAMANHO];
    char intermediario[MAX_TAMANHO];
    
    // Devido ao fato que strtok modifica a string e a requisição é uma
    // const char*, é necessário copiar a requisição

    strcpy(copia, requisicao);
    char* linha = strtok(copia, "\r\n");
    if (linha == NULL) {
        puts("Nenhuma linha recebida na requisição.");
        string resp = stringLiteral("HTTP/1.1 400 Bad Request"); 
        return moverString(&resp);
    }
    
    string primeiraLinha = stringLiteral(linha);
    char* metodo = strtok(obterString(primeiraLinha), " ");
    char* caminho = strtok(NULL, " ");
    
    puts(obterString((primeiraLinha)));
    
    if (metodo == NULL || caminho == NULL) {
        apagarString(&primeiraLinha);
        puts("Método ou caminho não encontrados na requisição.");
        string resp = stringLiteral("HTTP/1.1 400 Bad Request"); 
        return moverString(&resp);
    }
    
    printf("Metodo: %s\t|\tCaminho: %s\n", metodo, caminho);
    
    // Em geral, respostas são geradas da mesma forma.
    // Abrir o arquivo, seja por uma string ou por um vetor
    // Formatar o cabeçalho com as informações necessárias
    // E concatenar o arquivo ao cabeçalho

    if (strcmp("GET", metodo) == 0) {
        puts("Respondendo método GET...");
        char* cabecalhoHTTP = 
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";
        
        if (strcmp("/", caminho) == 0) {
            snprintf(intermediario, sizeof(intermediario), cabecalhoHTTP, "200 OK", "text/html", server->index_html.tamanho, server->index_html.data);
            string resp = stringLiteral(intermediario);
            return moverString(&resp);
        } else if (strcmp("/doido", caminho) == 0) {
            string arquivo = lerArquivo("doido.html");
            snprintf(intermediario, sizeof(intermediario), cabecalhoHTTP, "200 OK", "text/html", arquivo.tamanho, "");
            
            string cabecalho = stringLiteral(intermediario);
            concatenarString(&cabecalho, obterString(arquivo));
            apagarString(&arquivo);
            apagarString(&primeiraLinha);
            return moverString(&cabecalho);
        } else if (strcmp("/favicon.ico", caminho) == 0) {
            vector_byte arquivo = lerArquivoBinario("icon.png");
            snprintf(intermediario, sizeof(intermediario), cabecalhoHTTP, "200 OK", "image/png", arquivo.tamanho, "");
            
            string cabecalho = stringLiteral(intermediario);
            size_t cabecalho_tam = cabecalho.tamanho;
            vector_byte vec = moverString(&cabecalho);
            
            pop(&vec);
            concatenarVetorBytes(&vec, &arquivo);
            concatenarByte(&vec, U'\0');

            // debugBIN(vec, cabecalho_tam);
            apagarString(&primeiraLinha);
            return vec;
        } else {
            string _404 = lerArquivo("not_found.html");
            puts("arquivo não encontrado");
            snprintf(intermediario, sizeof(intermediario), cabecalhoHTTP, "404 NOT FOUND", "text/html", _404.tamanho, _404.data);
            apagarString(&primeiraLinha);
            apagarString(&_404);
            string resp = stringLiteral(intermediario); 
            return moverString(&resp);
        }
    }
    
    string resp = stringLiteral("HTTP/1.1 400 Bad Request"); 
    return moverString(&resp);
}


/**
 * @brief Prepara o servidor descrito a receber requisições.
 *
 * Esta função incializa a capacidade de receber requisições do servidor.
 * A resposta é emitida aqui, mas gerada em responder().
 *
 * @param server Ponteiro para a estrutura do servidor que será inicializada.
 */
void iniciar(server *server) {
    char buffer[MAX_TAMANHO];

    while (true) {
        int addrlen = sizeof(server->endereco);
        int new_socket = accept(server->socket_fd, (struct sockaddr*)&server->endereco, (socklen_t*)&addrlen);
        ssize_t requisicao = read(new_socket, buffer, MAX_TAMANHO - 1);
        if (requisicao >= 0) {
            buffer[requisicao] = '\0';
            puts(buffer);
        } else {
            perror("Erro ao ler buffer...\n");
        }

        vector_byte resposta = responder(server, buffer);
        
        printf("Tamanho da resposta: %zu\n", resposta.tamanho);
        write(new_socket, resposta.data, resposta.tamanho);
        apagarBytes(&resposta);
        close(new_socket);
    }
}


// Essa função lê a pagina padrão do servidor.
void ler_index(struct server *server, const char *nomeArquivo) {
    if (server->index_html.data != NULL) {
        apagarString(&server->index_html);
    }
    server->index_html = lerArquivo(nomeArquivo);
}

/**
 * @brief Cria e inicializa uma estrutura de servidor com os parâmetros especificados.
 *
 * Esta função configura um servidor TCP, inicializando seus campos, criando o socket,
 * configurando opções, vinculando à porta e colocando-o em modo de escuta.
 *
 * @param ip_ver Versão do protocolo IP (AF_INET para IPv4, AF_INET6 para IPv6).
 * @param porta Porta na qual o servidor irá escutar.
 * @return Estrutura server inicializada com valores.
 */
server criarServer(int ip_ver, int porta) {
    server sv = {
        .ip_ver = ip_ver,
        .porta = porta,
        .servico = SOCK_STREAM,
        .tipo = IPPROTO_TCP,
        .backlog = 5,
        .endereco = {0},
        .socket_fd = -1,
        .index_html = stringDinamica(0),
        .ler_index = ler_index,
        .iniciar = iniciar
    };

    sv.endereco.sin_family = ip_ver;
    sv.endereco.sin_port = htons(porta);
    sv.endereco.sin_addr.s_addr = INADDR_ANY;

    sv.socket_fd = socket(ip_ver, sv.servico, sv.tipo);
    if (sv.socket_fd < 0) {
        sair("Erro ao criar socket");
    }

    int reuse = 1;
    if (setsockopt(sv.socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse)) < 0) {
        sair("Erro ao configurar socket");
    }

    if (bind(sv.socket_fd, (struct sockaddr *)&sv.endereco, sizeof(sv.endereco)) < 0) {
        sair("Erro ao vincular socket");
    }

    if (listen(sv.socket_fd, sv.backlog) < 0) {
        sair("Erro ao escutar no socket");
    }

    return sv;
}