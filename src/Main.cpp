#include <cstring>
#include "Menu.h"
#include "Terminal.h"

// Uso: bolonhaemilano [--simples]
// --simples: nao limpa a tela nem pausa (util para rodar com entrada redirecionada).
int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--simples") == 0) Terminal::setAtivo(false);
    }
    Menu menu;
    menu.executar();
    return 0;
}
