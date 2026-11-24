#ifndef TESTES_H
#define TESTES_H

#include <memory>
#include <string>
#include "Cidade.h"

// Testes automatizados (sem biblioteca externa). Padrao de cada teste:
// preparar uma situacao, executar o metodo e verificar o resultado.
// Tudo static: ninguem cria um objeto Testes. O jogo nao conhece esta classe.
class Testes {
private:
    static int aprovados;
    static int reprovados;

    static void verificar(const std::string& descricao, bool condicao);

    // Atalho para "preparar": cidade com os valores iniciais (dono fica vivo ate o fim).
    struct CidadeDeTeste {
        std::unique_ptr<Jogador> dono;
        std::unique_ptr<Cidade> cidade;
    };
    static CidadeDeTeste novaCidade(const std::string& nome = "Bolonha");

    static void testarCidade();
    static void testarMercado();
    static void testarAgricultura();
    static void testarGoverno();
    static void testarConstrucoes();
    static void testarJogadorENobreza();
    static void testarPartida();
    static void testarPersistencia();   // extra: salvar/carregar
    static void testarTerminal();       // extra: limpar tela e pausar

public:
    // Roda tudo e devolve 0 (todos passaram) ou 1 (alguma regra quebrou).
    static int executar();
};

#endif
