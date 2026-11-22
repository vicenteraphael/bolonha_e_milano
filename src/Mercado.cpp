#include "Mercado.h"
#include <limits>

namespace {
constexpr long long LIMITE = std::numeric_limits<int>::max();

// Confere se somar 'quantidade' a 'atual' ainda cabe em um int.
bool cabe(int atual, long long quantidade) {
    return static_cast<long long>(atual) + quantidade <= LIMITE;
}
}  // namespace

int Mercado::getPrecoGrao() const { return PRECO_GRAO; }
int Mercado::getPrecoTerra() const { return PRECO_TERRA; }

// custo = quantidade x PRECO_GRAO; debita florins e, se der certo, credita graos.
bool Mercado::comprarGraos(Cidade& cidade, int quantidade) const {
    if (quantidade <= 0) return false;

    long long custo = static_cast<long long>(quantidade) * PRECO_GRAO;
    if (custo > LIMITE || !cabe(cidade.getGraos(), quantidade)) return false;

    if (!cidade.removerFlorins(static_cast<int>(custo))) return false;

    cidade.adicionarGraos(quantidade);
    return true;
}

// Logica invertida: remove os graos e, se der certo, credita florins.
bool Mercado::venderGraos(Cidade& cidade, int quantidade) const {
    if (quantidade <= 0) return false;

    long long receita = static_cast<long long>(quantidade) * PRECO_GRAO;
    // Valida tudo ANTES de alterar qualquer recurso.
    if (receita > LIMITE || !cabe(cidade.getFlorins(), receita)) return false;

    if (!cidade.removerGraos(quantidade)) return false;

    cidade.adicionarFlorins(static_cast<int>(receita));
    return true;
}

bool Mercado::comprarTerras(Cidade& cidade, int quantidade) const {
    if (quantidade <= 0) return false;

    long long custo = static_cast<long long>(quantidade) * PRECO_TERRA;
    if (custo > LIMITE || !cabe(cidade.getTerras(), quantidade)) return false;

    if (!cidade.removerFlorins(static_cast<int>(custo))) return false;

    cidade.adicionarTerras(quantidade);
    return true;
}

bool Mercado::venderTerras(Cidade& cidade, int quantidade) const {
    if (quantidade <= 0) return false;

    long long receita = static_cast<long long>(quantidade) * PRECO_TERRA;
    if (receita > LIMITE || !cabe(cidade.getFlorins(), receita)) return false;

    if (!cidade.removerTerras(quantidade)) return false;

    cidade.adicionarFlorins(static_cast<int>(receita));
    return true;
}
