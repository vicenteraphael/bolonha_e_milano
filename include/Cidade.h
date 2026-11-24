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
// Foto dos valores que variam durante a partida (usada para salvar/carregar).
// Taxa de imposto e nivel de justica ficam de fora: a validacao delas e do Governo.
struct EstadoCidade {
    int ano;
    int florins, terras, graos, servos, soldados, construcoes;
    bool colheitaFeita, impostosArrecadados;
};

class Cidade {
private:
    // Constantes de valor inicial: so preenchem o construtor.
    static constexpr int ANO_INICIAL      = 1400;
    static constexpr int FLORINS_INICIAIS = 1000;
    static constexpr int TERRAS_INICIAIS  = 10000;
    static constexpr int GRAOS_INICIAIS   = 500;
    static constexpr int SERVOS_INICIAIS  = 200;
    static constexpr int TAXA_IMPOSTO_INICIAL = 10;
    static constexpr const char* NIVEL_JUSTICA_INICIAL = "Moderada";
    static constexpr int SOLDADOS_INICIAIS    = 0;
    static constexpr int CONSTRUCOES_INICIAIS = 0;

    const std::string nome;   // final
    int anoAtual;
    Jogador* const dono;      // final (a associacao nao muda; o Jogador continua editavel)
    int florins;
    int terras;
    int graos;
    int servos;               // populacao
    int taxaImposto;          // configuracao (%), nao e um recurso acumulavel
    std::string nivelJustica; // configuracao: "Justa", "Moderada" ou "Severa"
    int soldados;             // recurso acumulavel (forca de defesa)
    int construcoes;          // recurso acumulavel (infraestrutura)
    bool colheitaFeita;       // ja colheu neste ano?
    bool impostosArrecadados; // ja arrecadou neste ano?

public:
    Cidade(const std::string& nome, Jogador* dono);

    std::string getNome() const;
    int getAnoAtual() const;
    Jogador* getDono() const;
    int getFlorins() const;
    int getTerras() const;
    int getGraos() const;
    int getServos() const;
    int getSoldados() const;
    int getConstrucoes() const;
    int getTaxaImposto() const;
    std::string getNivelJustica() const;

    // Configuracoes: so getter/setter simples (a validacao e do Governo).
    void setTaxaImposto(int taxa);
    void setNivelJustica(const std::string& nivel);

    // Indicadores "ja fez neste ano?" (zerados por avancarAno).
    bool isColheitaFeita() const;
    void marcarColheitaFeita();
    bool isImpostosArrecadados() const;
    void marcarImpostosArrecadados();

    void avancarAno();   // soma 1 ao ano e zera os dois indicadores (chamado pela Partida)

    void adicionarFlorins(int quantidade);
    bool removerFlorins(int quantidade);
    void adicionarTerras(int quantidade);
    bool removerTerras(int quantidade);
    void adicionarGraos(int quantidade);
    bool removerGraos(int quantidade);
    void adicionarServos(int quantidade);
    bool removerServos(int quantidade);
    void adicionarSoldados(int quantidade);
    bool removerSoldados(int quantidade);
    void adicionarConstrucoes(int quantidade);
    bool removerConstrucoes(int quantidade);

    EstadoCidade getEstado() const;
    // Restaura uma foto salva. Devolve false (sem alterar nada) se algum valor for
    // invalido: ano antes de 1400 ou qualquer recurso negativo.
    bool restaurarEstado(const EstadoCidade& estado);

    void exibirStatus() const;
};

#endif
