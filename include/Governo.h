#ifndef GOVERNO_H
#define GOVERNO_H

#include <string>
#include "Cidade.h"

// Arrecada impostos e valida a taxa de imposto e o nivel de justica.
// Mesmo padrao do Mercado e da Agricultura: NAO guarda a Cidade, que chega como parametro.
class Governo {
private:
    static constexpr int    TAXA_MAXIMA            = 50;   // maior taxa permitida (%)
    static constexpr double MULTIPLICADOR_JUSTA    = 0.8;
    static constexpr double MULTIPLICADOR_MODERADA = 1.0;
    static constexpr double MULTIPLICADOR_SEVERA   = 1.3;

    // Fator de arrecadacao do nivel de justica ("Justa", "Moderada" ou "Severa").
    double multiplicadorPara(const std::string& nivel) const;

public:
    // base = servos x taxa / 100; arrecadado = base x multiplicador (soma aos florins).
    // So vale uma vez por ano: se ja arrecadou, devolve -1 sem alterar nada.
    // Senao devolve o valor arrecadado.
    int arrecadarImpostos(Cidade& cidade) const;

    // Devolve false (sem alterar nada) se a taxa estiver fora de 0 a 50.
    bool definirTaxaImposto(Cidade& cidade, int taxa) const;

    // Devolve false (sem alterar nada) se o nivel nao for "Justa", "Moderada" ou "Severa".
    bool definirJustica(Cidade& cidade, const std::string& nivel) const;

    int getTaxaMaxima() const;
};

#endif
