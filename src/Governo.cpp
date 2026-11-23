#include "Governo.h"
#include <algorithm>
#include <limits>

namespace {
const char* const JUSTA    = "Justa";
const char* const MODERADA = "Moderada";
const char* const SEVERA   = "Severa";
}  // namespace

double Governo::multiplicadorPara(const std::string& nivel) const {
    if (nivel == JUSTA)  return MULTIPLICADOR_JUSTA;
    if (nivel == SEVERA) return MULTIPLICADOR_SEVERA;
    return MULTIPLICADOR_MODERADA;
}

int Governo::arrecadarImpostos(Cidade& cidade) const {
    int base = static_cast<int>(
        static_cast<long long>(cidade.getServos()) * cidade.getTaxaImposto() / 100);
    double multiplicador = multiplicadorPara(cidade.getNivelJustica());
    int arrecadado = static_cast<int>(base * multiplicador);

    // Nunca estoura o limite do int dos florins.
    int folga = std::numeric_limits<int>::max() - cidade.getFlorins();
    arrecadado = std::min(arrecadado, folga);

    cidade.adicionarFlorins(arrecadado);
    return arrecadado;
}

bool Governo::definirTaxaImposto(Cidade& cidade, int taxa) const {
    if (taxa < 0 || taxa > TAXA_MAXIMA) return false;
    cidade.setTaxaImposto(taxa);
    return true;
}

bool Governo::definirJustica(Cidade& cidade, const std::string& nivel) const {
    if (nivel != JUSTA && nivel != MODERADA && nivel != SEVERA) return false;
    cidade.setNivelJustica(nivel);
    return true;
}

int Governo::getTaxaMaxima() const { return TAXA_MAXIMA; }
