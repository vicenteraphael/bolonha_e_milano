#ifndef MENU_H
#define MENU_H

#include <memory>
#include "Jogador.h"
#include "Cidade.h"
#include "Partida.h"
#include "Nobreza.h"
#include "Mercado.h"
#include "Agricultura.h"
#include "Governo.h"
#include "Construcoes.h"

// Orquestra o jogo: mostra opcoes, le e valida a entrada e delega para a classe
// certa. Guarda a partida atual (jogador + cidade) e uma instancia de cada servico.
class Menu {
private:
    // std::cin faz o papel do "scanner" do Java.
    bool emExecucao;
    std::unique_ptr<Partida> partida;   // nenhuma ate "Novo jogo" (e de novo nula ao fim)
    Cidade* cidade;                     // a cidade da vez (pertence a Partida); nullptr sem partida
    const Mercado mercado;              // servico de compra e venda (final)
    const Agricultura agricultura;      // servico de colheita e alimentos (final)
    const Governo governo;              // servico de impostos e justica (final)
    const Construcoes construcoes;      // servico de soldados e construcoes (final)
    const Nobreza nobreza;              // servico de prestigio e titulos (final)

    void exibirOpcoes() const;
    int lerOpcao();
    void processarOpcao(int opcao);
    void iniciarNovoJogo();
    void exibirStatus() const;
    bool exigirPartida() const;          // avisa e devolve false se nao ha partida
    void exibirSobre() const;

    // Etapa 3: submenu do Mercado
    void abrirMercado();
    void exibirOpcoesMercado() const;
    int lerQuantidade();
    void comprarGraos();
    void venderGraos();
    void comprarTerras();
    void venderTerras();
    void informarResultado(bool sucesso, const char* sucessoMsg) const;

    // Etapa 4: submenu da Agricultura (o Menu so delega e imprime; a regra mora na classe)
    void abrirAgricultura();
    void exibirOpcoesAgricultura() const;
    int lerInteiro(const char* prompt);
    void colher();
    void distribuirAlimentos();

    // Etapa 5: submenu do Governo
    void abrirGoverno();
    void exibirOpcoesGoverno() const;
    void arrecadarImpostos();
    void definirTaxaImposto();
    void definirJustica();

    // Etapa 6: submenu de Construcoes
    void abrirConstrucoes();
    void exibirOpcoesConstrucoes() const;
    void recrutarSoldados();
    void construir();

    // Etapa 7: rodadas, classificacao e fim de jogo
    void encerrarTurno();
    void exibirClassificacao() const;
    void encerrarPartida();
    void anunciarVez() const;

public:
    Menu();
    void executar();
};

#endif
