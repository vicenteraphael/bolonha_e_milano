#include "Partida.h"

Partida::Partida() : indiceAtual(0) {}

bool Partida::adicionarCidade(std::unique_ptr<Jogador> jogador, std::unique_ptr<Cidade> cidade) {
    if (static_cast<int>(cidades.size()) >= MAXIMO_JOGADORES) return false;
    jogadores.push_back(std::move(jogador));
    cidades.push_back(std::move(cidade));
    return true;
}

const std::vector<std::unique_ptr<Cidade>>& Partida::getCidades() const { return cidades; }

Cidade* Partida::getCidadeAtual() const {
    return cidades.empty() ? nullptr : cidades[indiceAtual].get();
}

// Todas as cidades fecham o ano juntas, entao a primeira representa a partida.
int Partida::getAnoAtual() const {
    return cidades.empty() ? 0 : cidades.front()->getAnoAtual();
}

int Partida::getAnoFinal() const { return ANO_FINAL; }
int Partida::getMaximoJogadores() const { return MAXIMO_JOGADORES; }

int Partida::getIndiceAtual() const { return static_cast<int>(indiceAtual); }

bool Partida::restaurarVez(int indice) {
    if (indice < 0 || indice >= static_cast<int>(cidades.size())) return false;
    indiceAtual = static_cast<std::size_t>(indice);
    return true;
}

bool Partida::passarTurno() {
    indiceAtual++;
    if (indiceAtual >= cidades.size()) {
        indiceAtual = 0;
        return true;   // o ano acabou
    }
    return false;
}

std::vector<Cidade*> Partida::encerrarAno(const Nobreza& nobreza) {
    std::vector<Cidade*> promovidas;
    for (auto& cidade : cidades) {
        cidade->avancarAno();
        if (nobreza.verificarPromocao(*cidade)) promovidas.push_back(cidade.get());
    }
    return promovidas;
}

// Acaba quando alguem vira Rei/Rainha ou o ano passa de 1410.
bool Partida::terminou() const {
    for (const auto& cidade : cidades) {
        if (cidade->getDono()->ehMonarca()) return true;
    }
    return getAnoAtual() > ANO_FINAL;
}

// Titulo mais alto; empate: maior prestigio; novo empate: quem entrou primeiro.
Cidade* Partida::determinarVencedor(const Nobreza& nobreza) const {
    Cidade* vencedor = nullptr;
    for (const auto& cidade : cidades) {
        if (!vencedor) { vencedor = cidade.get(); continue; }
        int nivelA = cidade->getDono()->getNivelTitulo();
        int nivelB = vencedor->getDono()->getNivelTitulo();
        if (nivelA > nivelB ||
            (nivelA == nivelB &&
             nobreza.calcularPrestigio(*cidade) > nobreza.calcularPrestigio(*vencedor))) {
            vencedor = cidade.get();
        }
    }
    return vencedor;
}
