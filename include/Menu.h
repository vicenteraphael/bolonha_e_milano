#ifndef MENU_H
#define MENU_H

#include <memory>
#include "Jogador.h"
#include "Cidade.h"

// Controla o laco principal, a leitura e a validacao das opcoes.
class Menu {
private:
    bool emExecucao;
    std::unique_ptr<Jogador> jogador;
    std::unique_ptr<Cidade> cidade;

    void exibirOpcoes() const;
    int lerOpcao();
    void processarOpcao(int opcao);
    void iniciarNovoJogo();
    void exibirStatus() const;
    void exibirSobre() const;

public:
    Menu();
    void executar();
};

#endif
