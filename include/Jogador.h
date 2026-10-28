#ifndef JOGADOR_H
#define JOGADOR_H

#include <string>

// Representa o jogador: nome e titulo de nobreza (comeca como "Cavaleiro").
class Jogador {
private:
    std::string nome;
    std::string titulo;

public:
    explicit Jogador(const std::string& nome);

    std::string getNome() const;
    std::string getTitulo() const;
};

#endif
