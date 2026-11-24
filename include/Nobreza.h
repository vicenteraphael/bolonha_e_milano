#ifndef NOBREZA_H
#define NOBREZA_H

#include "Cidade.h"
#include "Jogador.h"

// Calcula o prestigio de uma cidade e decide quando o governante sobe de titulo.
// Servico: nao guarda a Cidade, que chega como parametro (dependencia).
class Nobreza {
private:
    static constexpr int PESO_CONSTRUCAO   = 10;
    static constexpr int PESO_SOLDADO      = 2;
    static constexpr int PESO_SERVO        = 1;
    static constexpr int HECTARES_POR_PONTO = 100;
    // Prestigio minimo de cada titulo (mesma ordem de Jogador::TITULOS).
    static constexpr int PRESTIGIO_MINIMO[6] = {0, 400, 550, 700, 850, 1000};

public:
    // construcoes x 10 + soldados x 2 + servos x 1 + terras / 100 (florins nao contam).
    int calcularPrestigio(const Cidade& cidade) const;
    // Prestigio exigido para o proximo titulo do jogador; -1 se ja e Rei/Rainha.
    int prestigioParaProximoTitulo(const Jogador& jogador) const;
    // Promove o dono da cidade, no maximo UM titulo por chamada. true se promoveu.
    bool verificarPromocao(Cidade& cidade) const;
};

#endif
