#include "Jogador.h"

Jogador::Jogador(const std::string& nome)
    : nome(nome), titulo("Cavaleiro") {}

std::string Jogador::getNome() const {
    return nome;
}

std::string Jogador::getTitulo() const {
    return titulo;
}
