#ifndef PARTIDA_H
#define PARTIDA_H

#include <memory>
#include <vector>
#include "Jogador.h"
#include "Cidade.h"
#include "Nobreza.h"

// A partida de verdade: guarda de 1 a 4 jogadores/cidades (associacao), controla de
// quem e a vez, o ano e o fim de jogo. As cidades jogam em turnos; quando o ultimo
// encerra, o ano fecha.
class Partida {
private:
    static constexpr int MAXIMO_JOGADORES = 4;
    static constexpr int ANO_FINAL = 1410;

    std::vector<std::unique_ptr<Jogador>> jogadores;   // a Partida e quem possui
    std::vector<std::unique_ptr<Cidade>> cidades;
    std::size_t indiceAtual;

public:
    Partida();

    // false (e descarta os argumentos) se ja ha 4 cidades.
    bool adicionarCidade(std::unique_ptr<Jogador> jogador, std::unique_ptr<Cidade> cidade);
    const std::vector<std::unique_ptr<Cidade>>& getCidades() const;
    Cidade* getCidadeAtual() const;          // nullptr se nao ha cidades
    int getAnoAtual() const;
    int getAnoFinal() const;
    int getMaximoJogadores() const;
    int getIndiceAtual() const;               // posicao da vez (para salvar)
    bool restaurarVez(int indice);            // false se fora da lista (para carregar)

    bool passarTurno();                      // true se o ano acabou
    std::vector<Cidade*> encerrarAno(const Nobreza& nobreza);   // cidades promovidas
    bool terminou() const;
    Cidade* determinarVencedor(const Nobreza& nobreza) const;
};

#endif
