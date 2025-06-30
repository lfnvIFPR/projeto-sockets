#include "stringDinamica.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg); 
    exit(1);    
}

int main(int argc, char *argv[]) {
 
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <endereco_servidor> <porta> [mensagem]\n", argv[0]);
        exit(1);
    }

    // socket 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERRO ao abrir socket");
    }

    // Cria um servidor em um endereço e porta especificados na linha de comando
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;          
    serv_addr.sin_port = htons(atoi(argv[2])); 
    
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        error("ERRO no endereço do servidor");
    }

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERRO ao conectar");
    }

    printf("Conectado ao servidor %s:%s\n", argv[1], argv[2]);

    // Lê uma string, sendo ou pela linha de comando ou por entrada durante execução
    char buffer[BUFFER_SIZE];
    if (argc > 3) {
       
        strncpy(buffer, argv[3], BUFFER_SIZE);
    } else {
       
        printf("Digite a mensagem: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; 
    }

    // Enviar a mensagem por meio do socket
    int n = send(sockfd, buffer, strlen(buffer), 0);
    if (n < 0) {
        error("ERRO ao enviar mensagem");
    }


    // A resposta pode ter um número desconhecido de bytes.
    // Para assegurar que ela seja lida integralmente, é necessário criar estruturas dinâmicas.
    // Execute em um loop a leitura de BUFFER_SIZE bytes de uma vez, repetindo a leitura até
    // que toda a resposta seja lida em um objeto vector_byte.

    // Assim que pronto, o objeto é convertido para uma string para poder ser impressa.
    // Não é requerido que isso ocorra, é por legibidade de código.

    vector_byte resposta = criarBytes(BUFFER_SIZE);
    do {
        memset(buffer, 0, BUFFER_SIZE);
        n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (n < 0) {
            error("ERRO ao receber resposta");
        } else if (n > 0) {
            concatenarBytes(&resposta, (byte*) buffer, n);
        }
    } while (n == BUFFER_SIZE);
    
    if (n < 0) {
        error("ERRO ao receber resposta");
    }

    concatenarByte(&resposta, u'\0');
    string resposta_str = stringLiteral((char*) resposta.data);
    apagarBytes(&resposta);

    printf("Resposta do servidor:\n\"%s\"\n", obterString(resposta_str));

    apagarString(&resposta_str);
    close(sockfd);
    return 0;
}