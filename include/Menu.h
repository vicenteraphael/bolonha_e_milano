#ifndef MENU_H
#define MENU_H

#include <memory>
#include "Jogador.h"
#include "Cidade.h"
#include "Mercado.h"

// Orquestra o jogo: mostra opcoes, le e valida a entrada e delega para a classe
// certa. Guarda a partida atual (jogador + cidade) e uma instancia de cada servico.
class Menu {
private:
    // std::cin faz o papel do "scanner" do Java.
    bool emExecucao;
    std::unique_ptr<Jogador> jogador;   // nenhum ate "Novo jogo"
    std::unique_ptr<Cidade> cidade;     // nenhuma ate "Novo jogo"
    const Mercado mercado;              // servico de compra e venda (final)

    void exibirOpcoes() const;
    int lerOpcao();
    void processarOpcao(int opcao);
    void iniciarNovoJogo();
    void exibirStatus() const;
    bool exigirPartida() const;          // avisa e devolve false se nao ha partida
    void exibirNaoImplementado(const char* nome, int etapa) const;
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

public:
    Menu();
    void executar();
};

#endif
