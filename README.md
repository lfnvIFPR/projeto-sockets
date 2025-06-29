# Projeto redes-sockets

Este projeto é um exemplo de comunicação cliente-servidor utilizando sockets em C, com suporte a respostas HTTP GET, enviando recursos em texto e binários. Ele inclui um servidor HTTP simples capaz de servir arquivos estáticos e um cliente para envio e recebimento de mensagens.

## Estrutura do Projeto

- `src/` — Código-fonte dos programas cliente e servidor.
- `include/` — Headers das estruturas e funções auxiliares.
- `CMakeLists.txt` — Arquivo de build principal.
- `README.md` — Este arquivo de instruções.

## Como compilar

O projeto utiliza **CMake** para facilitar a compilação. Certifique-se de ter o CMake instalado.

```sh
mkdir build
cd build
cmake ..
make
```

Para rodar o servidor na porta 8080:
```sh
cd build/src
./server
```


Para rodar o cliente:
```sh
cd build/src
./cliente # <IP> <porta> <mensagem>
# (padrão: ./cliente 127.0.0.1 8080 "GET / HTTP/1.1")
```

As requisições com recursos disponíveis são:
- `index.html`, pelo caminho `/`
- `doido.html`, pelo caminho `/doido` (uma página com mais de 4096 bytes)
- `favicon.ico` (um arquivo não textual)

Há uma página 404 em `not_found.html`.