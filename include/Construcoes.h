#ifndef CONSTRUCOES_H
#define CONSTRUCOES_H

#include "Cidade.h"

// Recruta soldados e constroi edificios (um unico recurso generico, "construcoes").
// Mesmo padrao dos outros servicos: NAO guarda a Cidade, que chega como parametro.
class Construcoes {
private:
    static constexpr int PRECO_SOLDADO     = 5;   // florins por soldado
    static constexpr int SERVOS_POR_SOLDADO = 1;  // servos convertidos por soldado
    static constexpr int PRECO_CONSTRUCAO  = 20;  // florins por construcao

public:
    int getPrecoSoldado() const;
    int getPrecoConstrucao() const;

    // Cada soldado custa 5 florins E 1 servo. Os DOIS recursos sao validados
    // ANTES de alterar qualquer um; se faltar um deles, nada muda e devolve false.
    bool recrutarSoldados(Cidade& cidade, int quantidade) const;

    // Cada construcao custa 20 florins. Devolve false (sem alterar nada) se a
    // quantidade for <= 0 ou se faltarem florins.
    bool construir(Cidade& cidade, int quantidade) const;
};

#endif
