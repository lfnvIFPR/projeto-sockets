#include "stringDinamica.h"
#include "server.h"

int main() {
    server sv = criarServer(AF_INET, 8080);;
    sv.ler_index(&sv, "index.html");
    sv.iniciar(&sv);

    apagarString(&sv.index_html);
    return 0;
}