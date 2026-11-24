#include "Nobreza.h"
#include <limits>

// Definicao fora da classe (exigida por compiladores que nao tratam constexpr static como inline).
constexpr int Nobreza::PRESTIGIO_MINIMO[6];

int Nobreza::calcularPrestigio(const Cidade& cidade) const {
    long long prestigio =
        static_cast<long long>(cidade.getConstrucoes()) * PESO_CONSTRUCAO +
        static_cast<long long>(cidade.getSoldados()) * PESO_SOLDADO +
        static_cast<long long>(cidade.getServos()) * PESO_SERVO +
        cidade.getTerras() / HECTARES_POR_PONTO;
    constexpr long long LIMITE = std::numeric_limits<int>::max();
    return static_cast<int>(prestigio > LIMITE ? LIMITE : prestigio);
}

int Nobreza::prestigioParaProximoTitulo(const Jogador& jogador) const {
    if (jogador.ehMonarca()) return -1;
    return PRESTIGIO_MINIMO[jogador.getNivelTitulo() + 1];
}

bool Nobreza::verificarPromocao(Cidade& cidade) const {
    Jogador* dono = cidade.getDono();
    int necessario = prestigioParaProximoTitulo(*dono);
    if (necessario < 0) return false;
    if (calcularPrestigio(cidade) < necessario) return false;
    return dono->promover();
}
