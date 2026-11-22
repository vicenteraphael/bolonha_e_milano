#ifndef AGRICULTURA_H
#define AGRICULTURA_H

#include "Cidade.h"

// Processa a colheita e distribui alimentos a populacao.
// Mesmo padrao do Mercado: NAO guarda a Cidade, que chega como parametro.
class Agricultura {
private:
    static constexpr int HECTARES_POR_SERVO     = 5;   // hectares que 1 servo cultiva
    static constexpr int RENDIMENTO_POR_HECTARE = 2;   // graos produzidos por hectare
    static constexpr int SERVOS_POR_GRAO        = 5;   // necessidade minima = servos / 5
    static constexpr int CRESCIMENTO_PERCENTUAL = 5;   // % de crescimento dos servos
    static constexpr int PERCENTUAL_MINIMO      = 20;  // menor fatia distribuivel
    static constexpr int PERCENTUAL_MAXIMO      = 80;  // maior fatia distribuivel

public:
    // Colher sempre funciona (nao ha "recusa"): o que se planta e limitado, nesta
    // ordem, por terras, mao de obra e sementes. Devolve os hectares cultivados.
    int processarColheita(Cidade& cidade) const;

    // Devolve false (sem alterar nada) se o percentual estiver fora de 20 a 80.
    bool distribuirAlimentos(Cidade& cidade, int percentual) const;

    int getPercentualMinimo() const;
    int getPercentualMaximo() const;
};

#endif
