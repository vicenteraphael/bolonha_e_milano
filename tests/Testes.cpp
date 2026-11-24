#include "Testes.h"
#include <iostream>
#include "Agricultura.h"
#include "Construcoes.h"
#include "Governo.h"
#include "Jogador.h"
#include "Mercado.h"
#include "Nobreza.h"
#include "Partida.h"

int Testes::aprovados = 0;
int Testes::reprovados = 0;

void Testes::verificar(const std::string& descricao, bool condicao) {
    if (condicao) {
        aprovados++;
        std::cout << "[OK]    " << descricao << "\n";
    } else {
        reprovados++;
        std::cout << "[FALHA] " << descricao << "\n";
    }
}

Testes::CidadeDeTeste Testes::novaCidade(const std::string& nome) {
    CidadeDeTeste c;
    c.dono = std::make_unique<Jogador>("Teste");
    c.cidade = std::make_unique<Cidade>(nome, c.dono.get());
    return c;
}

// ------------------------------------------------------------------ Cidade
void Testes::testarCidade() {
    std::cout << "\n-- Cidade --\n";
    auto t = novaCidade();
    Cidade& c = *t.cidade;
    verificar("cidade nasce com 1000 florins", c.getFlorins() == 1000);
    verificar("cidade nasce com 10000 terras, 500 graos e 200 servos",
              c.getTerras() == 10000 && c.getGraos() == 500 && c.getServos() == 200);
    verificar("cidade nasce no ano 1400", c.getAnoAtual() == 1400);
    verificar("cidade nasce com 10% de imposto, justica Moderada, 0 soldados e 0 construcoes",
              c.getTaxaImposto() == 10 && c.getNivelJustica() == "Moderada" &&
              c.getSoldados() == 0 && c.getConstrucoes() == 0);

    verificar("removerFlorins acima do saldo devolve false", !c.removerFlorins(5000));
    verificar("... e nao altera o saldo", c.getFlorins() == 1000);
    verificar("removerFlorins de quantidade negativa e recusado", !c.removerFlorins(-10));
    c.adicionarFlorins(-50);
    verificar("adicionarFlorins negativo nao reduz o saldo", c.getFlorins() == 1000);
    verificar("removerFlorins valido debita", c.removerFlorins(300) && c.getFlorins() == 700);

    c.marcarColheitaFeita();
    c.marcarImpostosArrecadados();
    verificar("indicadores do ano ficam marcados", c.isColheitaFeita() && c.isImpostosArrecadados());
    c.avancarAno();
    verificar("avancarAno soma 1 ao ano", c.getAnoAtual() == 1401);
    verificar("avancarAno libera colheita e impostos de novo",
              !c.isColheitaFeita() && !c.isImpostosArrecadados());
}

// ----------------------------------------------------------------- Mercado
void Testes::testarMercado() {
    std::cout << "\n-- Mercado --\n";
    Mercado mercado;
    auto t = novaCidade();
    Cidade& c = *t.cidade;
    bool comprou = mercado.comprarGraos(c, 100);
    verificar("comprar 100 graos da certo", comprou);
    verificar("... e custa 200 florins", c.getFlorins() == 800);
    verificar("... e soma 100 graos", c.getGraos() == 600);

    verificar("comprar quantidade negativa e recusado", !mercado.comprarGraos(c, -5));
    verificar("comprar quantidade zero e recusado", !mercado.comprarTerras(c, 0));
    verificar("comprar sem florins suficientes e recusado", !mercado.comprarGraos(c, 100000));
    verificar("... e nao altera florins nem graos", c.getFlorins() == 800 && c.getGraos() == 600);
    verificar("vender mais graos do que tem e recusado", !mercado.venderGraos(c, 601));
    verificar("vender 100 graos devolve 200 florins",
              mercado.venderGraos(c, 100) && c.getFlorins() == 1000 && c.getGraos() == 500);
    verificar("comprar e vender terras (1 florim/hectare)",
              mercado.comprarTerras(c, 100) && c.getTerras() == 10100 && c.getFlorins() == 900 &&
              mercado.venderTerras(c, 100) && c.getTerras() == 10000 && c.getFlorins() == 1000);
    verificar("quantidade enorme nao estoura (recusada)", !mercado.comprarGraos(c, 2000000000));
}

// -------------------------------------------------------------- Agricultura
void Testes::testarAgricultura() {
    std::cout << "\n-- Agricultura --\n";
    Agricultura agricultura;
    auto t = novaCidade();
    Cidade& c = *t.cidade;
    int hectares = agricultura.processarColheita(c);
    verificar("primeira colheita do ano cultiva 1000 hectares", hectares == 1000);
    verificar("... e deixa 2000 graos (500 - 500 sementes + 2000)", c.getGraos() == 2000);
    verificar("segunda colheita no mesmo ano devolve -1", agricultura.processarColheita(c) == -1);
    verificar("... e nao altera os graos", c.getGraos() == 2000);
    c.avancarAno();
    verificar("no ano seguinte a colheita volta a valer", agricultura.processarColheita(c) > 0);

    auto u = novaCidade();
    verificar("distribuir 10% e recusado (minimo 20%)", !agricultura.distribuirAlimentos(*u.cidade, 10));
    verificar("distribuir 90% e recusado (maximo 80%)", !agricultura.distribuirAlimentos(*u.cidade, 90));
    verificar("... e nao altera graos nem servos", u.cidade->getGraos() == 500 && u.cidade->getServos() == 200);
    verificar("distribuir 50% da certo: servos crescem 5%",
              agricultura.distribuirAlimentos(*u.cidade, 50) && u.cidade->getServos() == 210 &&
              u.cidade->getGraos() == 250);
}

// ------------------------------------------------------------------ Governo
void Testes::testarGoverno() {
    std::cout << "\n-- Governo --\n";
    Governo governo;
    auto t = novaCidade();
    Cidade& c = *t.cidade;
    verificar("taxa de 60% e recusada (maximo 50%)", !governo.definirTaxaImposto(c, 60));
    verificar("taxa negativa e recusada", !governo.definirTaxaImposto(c, -1));
    verificar("... e a taxa continua 10%", c.getTaxaImposto() == 10);
    verificar("justica \"Branda\" e recusada", !governo.definirJustica(c, "Branda"));
    verificar("... e a justica continua Moderada", c.getNivelJustica() == "Moderada");
    verificar("justica Severa e aceita", governo.definirJustica(c, "Severa") && c.getNivelJustica() == "Severa");
    int arrecadado = governo.arrecadarImpostos(c);
    verificar("200 servos, 10%, Severa: arrecada 26 florins", arrecadado == 26 && c.getFlorins() == 1026);
    verificar("segunda arrecadacao no mesmo ano devolve -1", governo.arrecadarImpostos(c) == -1);
    verificar("... e nao altera os florins", c.getFlorins() == 1026);
    c.avancarAno();
    verificar("no ano seguinte a arrecadacao volta a valer", governo.arrecadarImpostos(c) == 26);
}

// -------------------------------------------------------------- Construcoes
void Testes::testarConstrucoes() {
    std::cout << "\n-- Construcoes --\n";
    Construcoes construcoes;
    auto t = novaCidade();
    Cidade& c = *t.cidade;
    c.removerServos(150);   // florins bastam (500 de 1000), servos nao (so 50)
    bool recrutou = construcoes.recrutarSoldados(c, 100);
    verificar("recrutar 100 soldados com so 50 servos e recusado", !recrutou);
    verificar("... e NAO debita florins pela metade", c.getFlorins() == 1000);
    verificar("... nem servos nem soldados", c.getServos() == 50 && c.getSoldados() == 0);
    verificar("recrutar 5 soldados custa 25 florins e 5 servos",
              construcoes.recrutarSoldados(c, 5) && c.getFlorins() == 975 && c.getServos() == 45 &&
              c.getSoldados() == 5);
    verificar("recrutar quantidade negativa e recusado", !construcoes.recrutarSoldados(c, -3));

    auto u = novaCidade();
    verificar("construir 10 da certo", construcoes.construir(*u.cidade, 10));
    verificar("... e custa 200 florins", u.cidade->getFlorins() == 800 && u.cidade->getConstrucoes() == 10);
    verificar("construir sem florins suficientes e recusado", !construcoes.construir(*u.cidade, 1000));
    verificar("... e nao altera nada", u.cidade->getFlorins() == 800 && u.cidade->getConstrucoes() == 10);
}

// ------------------------------------------------------- Jogador e Nobreza
void Testes::testarJogadorENobreza() {
    std::cout << "\n-- Jogador e Nobreza --\n";
    Nobreza nobreza;
    auto t = novaCidade();
    Cidade& c = *t.cidade;
    verificar("jogador comeca como Cavaleiro", t.dono->getTitulo() == "Cavaleiro" && !t.dono->ehMonarca());
    verificar("prestigio inicial e 300 (200 servos + 10000/100 terras)", nobreza.calcularPrestigio(c) == 300);
    verificar("florins nao entram no prestigio",
              (c.adicionarFlorins(5000), nobreza.calcularPrestigio(c) == 300));
    verificar("com prestigio 300 nao sobe de titulo", !nobreza.verificarPromocao(c) && t.dono->getNivelTitulo() == 0);
    verificar("proximo titulo do Cavaleiro exige 400", nobreza.prestigioParaProximoTitulo(*t.dono) == 400);

    c.adicionarConstrucoes(50);   // prestigio 800: bastaria para Conde e Marques
    verificar("com 50 construcoes sobe para Barao", nobreza.verificarPromocao(c) && t.dono->getTitulo() == "Barao");
    verificar("sobe no maximo um titulo por chamada (so Barao)", t.dono->getNivelTitulo() == 1);

    bool promoveuAlemDoLimite = false;
    for (int i = 0; i < 4; i++) t.dono->promover();   // Conde, Marques, Duque, Rei/Rainha
    verificar("depois de 5 promocoes e Rei/Rainha", t.dono->ehMonarca() && t.dono->getTitulo() == "Rei/Rainha");
    promoveuAlemDoLimite = t.dono->promover();
    verificar("promover para alem de Rei/Rainha e recusado", !promoveuAlemDoLimite && t.dono->ehMonarca());
    verificar("monarca nao tem proximo titulo (-1)", nobreza.prestigioParaProximoTitulo(*t.dono) == -1);
    verificar("verificarPromocao de monarca devolve false", !nobreza.verificarPromocao(c));
}

// ------------------------------------------------------------------ Partida
void Testes::testarPartida() {
    std::cout << "\n-- Partida --\n";
    Nobreza nobreza;
    Partida partida;
    Jogador* j1 = nullptr;
    Jogador* j2 = nullptr;
    {
        auto a = std::make_unique<Jogador>("Ana");
        auto b = std::make_unique<Jogador>("Beto");
        j1 = a.get();
        j2 = b.get();
        auto ca = std::make_unique<Cidade>("Bolonha", j1);
        auto cb = std::make_unique<Cidade>("Milano", j2);
        partida.adicionarCidade(std::move(a), std::move(ca));
        partida.adicionarCidade(std::move(b), std::move(cb));
    }
    verificar("primeiro turno e de Bolonha", partida.getCidadeAtual()->getNome() == "Bolonha");
    verificar("passar o turno de Bolonha NAO fecha o ano", !partida.passarTurno());
    verificar("agora e a vez de Milano", partida.getCidadeAtual()->getNome() == "Milano");
    verificar("passar o turno de Milano fecha o ano", partida.passarTurno());
    verificar("... e a vez volta para Bolonha", partida.getCidadeAtual()->getNome() == "Bolonha");

    partida.getCidades()[1]->adicionarConstrucoes(50);   // Milano ganha prestigio
    auto promovidas = partida.encerrarAno(nobreza);
    verificar("encerrarAno avanca todas as cidades",
              partida.getCidades()[0]->getAnoAtual() == 1401 && partida.getCidades()[1]->getAnoAtual() == 1401);
    verificar("... e devolve so quem foi promovido (Milano)",
              promovidas.size() == 1 && promovidas[0]->getNome() == "Milano");
    verificar("partida ainda nao terminou em 1401", !partida.terminou());
    verificar("vencedor parcial e Milano (titulo mais alto)",
              partida.determinarVencedor(nobreza)->getNome() == "Milano");

    for (int i = 0; i < 4; i++) j2->promover();   // Milano vira Rei/Rainha
    verificar("partida termina quando alguem vira Rei/Rainha", partida.terminou());

    Partida longa;
    auto jl = std::make_unique<Jogador>("Solo");
    auto cl = std::make_unique<Cidade>("Veneza", jl.get());
    longa.adicionarCidade(std::move(jl), std::move(cl));
    for (int ano = 1400; ano <= 1410; ano++) longa.encerrarAno(nobreza);
    verificar("sem Rei/Rainha, termina quando o ano passa de 1410", longa.getAnoAtual() == 1411 && longa.terminou());

    Partida cheia;
    int aceitas = 0;
    for (int i = 0; i < 5; i++) {
        auto j = std::make_unique<Jogador>("J");
        auto cid = std::make_unique<Cidade>("C", j.get());
        if (cheia.adicionarCidade(std::move(j), std::move(cid))) aceitas++;
    }
    verificar("quinta cidade e recusada (maximo 4)", aceitas == 4 && cheia.getCidades().size() == 4);
}

int Testes::executar() {
    std::cout << "===== TESTES BOLONHA E MILANO =====\n";
    testarCidade();
    testarMercado();
    testarAgricultura();
    testarGoverno();
    testarConstrucoes();
    testarJogadorENobreza();
    testarPartida();
    std::cout << "\nAprovados: " << aprovados << " | Reprovados: " << reprovados << "\n";
    return reprovados > 0 ? 1 : 0;
}

// Segundo main do projeto: so existe no executavel de testes (make test).
int main() {
    return Testes::executar();
}
