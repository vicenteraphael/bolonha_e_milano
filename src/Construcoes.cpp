#include "Construcoes.h"
#include <limits>

namespace {
constexpr long long LIMITE = std::numeric_limits<int>::max();

// Confere se somar 'quantidade' a 'atual' ainda cabe em um int.
bool cabe(int atual, long long quantidade) {
    return static_cast<long long>(atual) + quantidade <= LIMITE;
}
}  // namespace

int Construcoes::getPrecoSoldado() const { return PRECO_SOLDADO; }
int Construcoes::getPrecoConstrucao() const { return PRECO_CONSTRUCAO; }

// custo = qtd x 5 florins; servosNecessarios = qtd x 1.
// Valida florins E servos antes de remover qualquer um (evita "efeito colateral parcial").
bool Construcoes::recrutarSoldados(Cidade& cidade, int quantidade) const {
    if (quantidade <= 0) return false;

    long long custo = static_cast<long long>(quantidade) * PRECO_SOLDADO;
    long long servosNecessarios = static_cast<long long>(quantidade) * SERVOS_POR_SOLDADO;

    if (custo > cidade.getFlorins() || servosNecessarios > cidade.getServos()) return false;
    if (!cabe(cidade.getSoldados(), quantidade)) return false;

    cidade.removerFlorins(static_cast<int>(custo));
    cidade.removerServos(static_cast<int>(servosNecessarios));
    cidade.adicionarSoldados(quantidade);
    return true;
}

// custo = qtd x 20 florins; tenta remover florins e, se der certo, adiciona as construcoes.
bool Construcoes::construir(Cidade& cidade, int quantidade) const {
    if (quantidade <= 0) return false;

    long long custo = static_cast<long long>(quantidade) * PRECO_CONSTRUCAO;
    if (custo > LIMITE || !cabe(cidade.getConstrucoes(), quantidade)) return false;

    if (!cidade.removerFlorins(static_cast<int>(custo))) return false;

    cidade.adicionarConstrucoes(quantidade);
    return true;
}
