#ifndef CIDADE_H
#define CIDADE_H

#include <string>
#include "Jogador.h"

// Representa a cidade: nome, ano atual (comeca em 1400) e o jogador dono.
// A Cidade apenas "conhece" o dono (associacao); quem possui o Jogador e o Menu.
class Cidade {
private:
    std::string nome;
    int anoAtual;
    const Jogador* dono;

public:
    Cidade(const std::string& nome, const Jogador* dono);

    void avancarAno();      // so sera usado a partir da Etapa 7
    void exibirStatus() const;
};

#endif
