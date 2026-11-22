#ifndef MERCADO_H
#define MERCADO_H

#include "Cidade.h"

// Compra e vende graos e terras a preco fixo.
// NAO guarda nenhuma Cidade: ela chega como parametro em cada metodo
// (dependencia, nao associacao).
class Mercado {
private:
    static constexpr int PRECO_GRAO  = 2;  // florins por unidade de grao
    static constexpr int PRECO_TERRA = 1;  // florins por hectare

public:
    int getPrecoGrao() const;
    int getPrecoTerra() const;

    // Todas devolvem false (sem alterar a cidade) se a quantidade for <= 0,
    // se faltar saldo/estoque, ou se o resultado estouraria o limite do int.
    bool comprarGraos(Cidade& cidade, int quantidade) const;
    bool venderGraos(Cidade& cidade, int quantidade) const;
    bool comprarTerras(Cidade& cidade, int quantidade) const;
    bool venderTerras(Cidade& cidade, int quantidade) const;
};

#endif
