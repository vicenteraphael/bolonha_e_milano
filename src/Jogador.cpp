#include "Jogador.h"

// Definicao fora da classe: compiladores mais antigos (MinGW) exigem para membros static constexpr.
constexpr const char* Jogador::TITULOS[Jogador::NUM_TITULOS];

Jogador::Jogador(const std::string& nome)
    : nome(nome), nivelTitulo(0) {}

std::string Jogador::getNome() const { return nome; }
std::string Jogador::getTitulo() const { return TITULOS[nivelTitulo]; }
int Jogador::getNivelTitulo() const { return nivelTitulo; }

bool Jogador::ehMonarca() const { return nivelTitulo == NUM_TITULOS - 1; }

bool Jogador::promover() {
    if (ehMonarca()) return false;
    nivelTitulo++;
    return true;
}
