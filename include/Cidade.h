#ifndef CIDADE_H
#define CIDADE_H

#include <string>
#include "Jogador.h"

// O que e governado: ano e recursos (florins, terras, graos).
// Cidade CONHECE o dono (associacao permanente), mas quem possui o Jogador e o Menu.
//
// Padrao seguro dos recursos: adicionarX() sempre funciona; removerX() devolve
// false (e nao altera nada) se nao houver quantidade suficiente. Nenhum recurso
// jamais fica negativo.
class Cidade {
private:
    // Constantes de valor inicial: so preenchem o construtor.
    static constexpr int ANO_INICIAL      = 1400;
    static constexpr int FLORINS_INICIAIS = 1000;
    static constexpr int TERRAS_INICIAIS  = 10000;
    static constexpr int GRAOS_INICIAIS   = 500;
    static constexpr int SERVOS_INICIAIS  = 200;

    const std::string nome;   // final
    int anoAtual;
    Jogador* const dono;      // final (a associacao nao muda; o Jogador continua editavel)
    int florins;
    int terras;
    int graos;
    int servos;               // populacao

public:
    Cidade(const std::string& nome, Jogador* dono);

    std::string getNome() const;
    int getAnoAtual() const;
    Jogador* getDono() const;
    int getFlorins() const;
    int getTerras() const;
    int getGraos() const;
    int getServos() const;

    void avancarAno();   // ainda nao e chamado por nenhum menu (Etapa 7)

    void adicionarFlorins(int quantidade);
    bool removerFlorins(int quantidade);
    void adicionarTerras(int quantidade);
    bool removerTerras(int quantidade);
    void adicionarGraos(int quantidade);
    bool removerGraos(int quantidade);
    void adicionarServos(int quantidade);
    bool removerServos(int quantidade);

    void exibirStatus() const;
};

#endif
