#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>

// Utilitarios de terminal: limpar a tela e pausar ("Pressione ENTER para continuar").
// Tudo static. Pode ser desligado (setAtivo(false)) para rodar o jogo com entrada
// automatica (scripts, testes): desligado, nao limpa nem le nada.
class Terminal {
private:
    static bool ativo;

public:
    static void setAtivo(bool valor);
    static bool isAtivo();

    // Limpa a tela com sequencias ANSI (no Windows habilita o modo virtual do console).
    static void limparTela(std::ostream& saida = std::cout);

    // Mostra "Pressione ENTER para continuar. . ." e espera uma linha.
    // Devolve false se a entrada acabou (EOF); true caso contrario (inclusive desligado).
    static bool pausar(std::istream& entrada = std::cin, std::ostream& saida = std::cout);
};

#endif
