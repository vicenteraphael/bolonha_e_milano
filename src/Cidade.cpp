#include "Cidade.h"
#include <iostream>

Cidade::Cidade(const std::string& nome, Jogador* dono)
    : nome(nome),
      anoAtual(ANO_INICIAL),
      dono(dono),
      florins(FLORINS_INICIAIS),
      terras(TERRAS_INICIAIS),
      graos(GRAOS_INICIAIS),
      servos(SERVOS_INICIAIS),
      taxaImposto(TAXA_IMPOSTO_INICIAL),
      nivelJustica(NIVEL_JUSTICA_INICIAL) {}

std::string Cidade::getNome() const { return nome; }
int Cidade::getAnoAtual() const { return anoAtual; }
Jogador* Cidade::getDono() const { return dono; }
int Cidade::getFlorins() const { return florins; }
int Cidade::getTerras() const { return terras; }
int Cidade::getGraos() const { return graos; }
int Cidade::getServos() const { return servos; }
int Cidade::getTaxaImposto() const { return taxaImposto; }
std::string Cidade::getNivelJustica() const { return nivelJustica; }

void Cidade::setTaxaImposto(int taxa) { taxaImposto = taxa; }
void Cidade::setNivelJustica(const std::string& nivel) { nivelJustica = nivel; }

void Cidade::avancarAno() {
    anoAtual++;
}

// Adicionar so aceita quantidades positivas, para nunca reduzir um recurso por
// engano (quantidade negativa) nem deixa-lo negativo.
void Cidade::adicionarFlorins(int quantidade) {
    if (quantidade > 0) florins += quantidade;
}

bool Cidade::removerFlorins(int quantidade) {
    if (quantidade <= 0 || quantidade > florins) return false;
    florins -= quantidade;
    return true;
}

void Cidade::adicionarTerras(int quantidade) {
    if (quantidade > 0) terras += quantidade;
}

bool Cidade::removerTerras(int quantidade) {
    if (quantidade <= 0 || quantidade > terras) return false;
    terras -= quantidade;
    return true;
}

void Cidade::adicionarGraos(int quantidade) {
    if (quantidade > 0) graos += quantidade;
}

bool Cidade::removerGraos(int quantidade) {
    if (quantidade <= 0 || quantidade > graos) return false;
    graos -= quantidade;
    return true;
}

void Cidade::adicionarServos(int quantidade) {
    if (quantidade > 0) servos += quantidade;
}

bool Cidade::removerServos(int quantidade) {
    if (quantidade <= 0 || quantidade > servos) return false;
    servos -= quantidade;
    return true;
}

void Cidade::exibirStatus() const {
    std::cout << "\n--- Status da cidade ---\n"
              << "Cidade:     " << nome << "\n"
              << "Governante: " << dono->getTitulo() << " " << dono->getNome() << "\n"
              << "Ano atual:  " << anoAtual << "\n"
              << "Florins:    " << florins << "\n"
              << "Terras:     " << terras << " hectares\n"
              << "Graos:      " << graos << " unidades\n"
              << "Servos:     " << servos << "\n"
              << "Imposto:    " << taxaImposto << "%\n"
              << "Justica:    " << nivelJustica << "\n"
              << "------------------------\n";
}
