#include "Agricultura.h"
#include <algorithm>
#include <limits>

namespace {
constexpr long long LIMITE = std::numeric_limits<int>::max();

// Sementes necessarias = hectares / 2 (regra do enunciado; nao consta na tabela
// de constantes do manual, por isso fica local a este arquivo).
constexpr int HECTARES_POR_SEMENTE = 2;

// Quanto ainda cabe em um int antes de estourar.
int folga(int atual) {
    return static_cast<int>(LIMITE - atual);
}
}  // namespace

// hectares  = min(terras, servos x 5)
// sementes  = hectares / 2   (reduzidas ao estoque de graos se faltar)
// producao  = hectares x 2   (somada ao estoque, depois de gastar as sementes)
int Agricultura::processarColheita(Cidade& cidade) const {
    long long maoDeObra = static_cast<long long>(cidade.getServos()) * HECTARES_POR_SERVO;
    long long hectares = std::min<long long>(cidade.getTerras(), maoDeObra);

    long long sementes = hectares / HECTARES_POR_SEMENTE;
    if (sementes > cidade.getGraos()) {
        // Faltou semente: a area cultivada encolhe para caber no estoque.
        sementes = cidade.getGraos();
        hectares = sementes * HECTARES_POR_SEMENTE;
    }

    cidade.removerGraos(static_cast<int>(sementes));  // 0 sementes: nada a retirar

    long long producao = hectares * RENDIMENTO_POR_HECTARE;
    producao = std::min<long long>(producao, folga(cidade.getGraos()));
    cidade.adicionarGraos(static_cast<int>(producao));

    return static_cast<int>(hectares);
}

// alimento   = graos x percentual / 100   (retirado do estoque)
// necessidade = servos / 5
// alimento >= necessidade: servos crescem 5%; senao perdem o deficit (nunca < 0).
bool Agricultura::distribuirAlimentos(Cidade& cidade, int percentual) const {
    if (percentual < PERCENTUAL_MINIMO || percentual > PERCENTUAL_MAXIMO) return false;

    int alimento = static_cast<int>(
        static_cast<long long>(cidade.getGraos()) * percentual / 100);
    cidade.removerGraos(alimento);  // 0 de alimento: nada a retirar

    int necessidade = cidade.getServos() / SERVOS_POR_GRAO;
    if (alimento >= necessidade) {
        long long crescimento =
            static_cast<long long>(cidade.getServos()) * CRESCIMENTO_PERCENTUAL / 100;
        crescimento = std::min<long long>(crescimento, folga(cidade.getServos()));
        cidade.adicionarServos(static_cast<int>(crescimento));
    } else {
        int deficit = necessidade - alimento;
        cidade.removerServos(std::min(deficit, cidade.getServos()));
    }
    return true;
}

int Agricultura::getPercentualMinimo() const { return PERCENTUAL_MINIMO; }
int Agricultura::getPercentualMaximo() const { return PERCENTUAL_MAXIMO; }
