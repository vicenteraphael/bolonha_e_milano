#include <Cidade.h>
#include <iostream>

Cidade::Cidade(const std::string& nome, const Jogador* dono)
    : nome(nome), anoAtual(1400), dono(dono) {}

void Cidade::avancarAno() {
    anoAtual++;
}

void Cidade::exibirStatus() const {
    std::cout << "\n--- Status da cidade ---\n"
              << "Cidade:     " << nome << "\n"
              << "Governante: " << dono->getTitulo() << " " << dono->getNome() << "\n"
              << "Ano atual:  " << anoAtual << "\n"
              << "------------------------\n";
}
