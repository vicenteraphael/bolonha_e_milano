#include "Testes.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include "Agricultura.h"
#include "Construcoes.h"
#include "Governo.h"
#include "Jogador.h"
#include "Mercado.h"
#include "Nobreza.h"
#include "Partida.h"
#include "Persistencia.h"
#include "Terminal.h"

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

    EstadoCidade ruim = c.getEstado();
    ruim.florins = -1;
    verificar("restaurarEstado com recurso negativo e recusado", !c.restaurarEstado(ruim));
    ruim = c.getEstado();
    ruim.ano = 1399;
    verificar("restaurarEstado com ano antes de 1400 e recusado", !c.restaurarEstado(ruim));
    verificar("... e a cidade nao muda", c.getFlorins() == 700 && c.getAnoAtual() == 1401);
    EstadoCidade bom = c.getEstado();
    bom.florins = 123; bom.soldados = 7; bom.colheitaFeita = true;
    verificar("restaurarEstado valido aplica os valores",
              c.restaurarEstado(bom) && c.getFlorins() == 123 && c.getSoldados() == 7 && c.isColheitaFeita());
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

// ------------------------------------------------- Persistencia (save/load)
namespace {
// Texto de um save valido (2 cidades), para os testes de erro mexerem em uma linha.
std::string saveBase() {
    return "BOLONHA_E_MILANO_SAVE v1\nvez=1\n"
           "[cidade]\njogador=Ana\ntitulo=1\nnome=Bolonha\nano=1401\nflorins=900\nterras=10000\n"
           "graos=500\nservos=200\nsoldados=4\nconstrucoes=2\ntaxa=20\njustica=Severa\ncolheita=1\nimpostos=0\n"
           "[cidade]\njogador=Beto\ntitulo=0\nnome=Milano\nano=1401\nflorins=1000\nterras=10000\n"
           "graos=500\nservos=200\nsoldados=0\nconstrucoes=0\ntaxa=10\njustica=Moderada\ncolheita=0\nimpostos=0\n";
}

std::string trocar(std::string texto, const std::string& de, const std::string& para) {
    std::size_t pos = texto.find(de);
    if (pos != std::string::npos) texto.replace(pos, de.size(), para);
    return texto;
}
}  // namespace

void Testes::testarPersistencia() {
    std::cout << "\n-- Persistencia (salvar/carregar) --\n";
    Persistencia persistencia;
    std::string erro;

    // Preparar: partida de 2 jogadores com estado "mexido".
    Partida original;
    for (const char* nome : {"Ana", "Beto"}) {
        auto j = std::make_unique<Jogador>(nome);
        auto c = std::make_unique<Cidade>(std::string(nome) == "Ana" ? "Bolonha" : "Milano", j.get());
        original.adicionarCidade(std::move(j), std::move(c));
    }
    Cidade& ana = *original.getCidades()[0];
    ana.getDono()->promover();
    ana.removerFlorins(100);
    ana.adicionarSoldados(4);
    ana.adicionarConstrucoes(2);
    ana.setTaxaImposto(20);
    ana.setNivelJustica("Severa");
    ana.marcarColheitaFeita();
    original.passarTurno();   // vez de Milano

    // Executar: salvar e carregar de volta.
    std::stringstream arquivo;
    persistencia.salvar(original, arquivo);
    std::unique_ptr<Partida> lida = persistencia.carregar(arquivo, erro);

    verificar("carregar o que foi salvo da certo", lida != nullptr);
    if (!lida) return;
    verificar("mesma quantidade de cidades", lida->getCidades().size() == 2);
    const Cidade& a2 = *lida->getCidades()[0];
    EstadoCidade e1 = ana.getEstado(), e2 = a2.getEstado();
    verificar("recursos da cidade voltam iguais",
              e1.ano == e2.ano && e1.florins == e2.florins && e1.terras == e2.terras &&
              e1.graos == e2.graos && e1.servos == e2.servos && e1.soldados == e2.soldados &&
              e1.construcoes == e2.construcoes);
    verificar("indicadores do ano voltam iguais",
              a2.isColheitaFeita() && !a2.isImpostosArrecadados());
    verificar("taxa e justica voltam iguais", a2.getTaxaImposto() == 20 && a2.getNivelJustica() == "Severa");
    verificar("nome da cidade e do jogador voltam iguais",
              a2.getNome() == "Bolonha" && a2.getDono()->getNome() == "Ana");
    verificar("titulo do jogador volta igual (Barao)", a2.getDono()->getTitulo() == "Barao");
    verificar("a vez volta igual (Milano)", lida->getCidadeAtual()->getNome() == "Milano");

    // Arquivo de verdade (salvar -> carregar -> apagar).
    std::string caminho = "bolonha_teste_save.tmp.txt";   // na pasta atual; apagado no fim
    bool gravou = persistencia.salvarArquivo(original, caminho, erro);
    std::unique_ptr<Partida> doArquivo = persistencia.carregarArquivo(caminho, erro);
    std::remove(caminho.c_str());
    verificar("salvar e carregar em arquivo .txt de verdade",
              gravou && doArquivo && doArquivo->getCidades()[0]->getFlorins() == 900);

    // Erros: nada de partida pela metade.
    verificar("arquivo inexistente e recusado",
              !persistencia.carregarArquivo("nao_existe_123.txt", erro) && !erro.empty());
    auto carregarTexto = [&](const std::string& texto) {
        std::istringstream in(texto);
        erro.clear();
        return persistencia.carregar(in, erro);
    };
    verificar("save base de teste e valido", carregarTexto(saveBase()) != nullptr);
    verificar("cabecalho incorreto e recusado",
              !carregarTexto(trocar(saveBase(), "SAVE v1", "SAVE v9")) && !erro.empty());
    verificar("arquivo vazio e recusado", !carregarTexto(""));
    verificar("florins negativos sao recusados", !carregarTexto(trocar(saveBase(), "florins=900", "florins=-5")));
    verificar("valor nao numerico e recusado", !carregarTexto(trocar(saveBase(), "florins=900", "florins=abc")));
    verificar("justica inexistente e recusada", !carregarTexto(trocar(saveBase(), "Severa", "Branda")));
    verificar("taxa acima do maximo (60%) e recusada", !carregarTexto(trocar(saveBase(), "taxa=20", "taxa=60")));
    verificar("titulo Rei/Rainha num save e recusado", !carregarTexto(trocar(saveBase(), "titulo=1", "titulo=5")));
    verificar("vez fora da lista e recusada", !carregarTexto(trocar(saveBase(), "vez=1", "vez=2")));
    verificar("cidades em anos diferentes sao recusadas",
              !carregarTexto(trocar(saveBase(), "ano=1401\nflorins=900", "ano=1402\nflorins=900")));
    verificar("campo ausente e recusado", !carregarTexto(trocar(saveBase(), "servos=200\nsoldados=4", "soldados=4")));
    verificar("linha mal formada e recusada", !carregarTexto(trocar(saveBase(), "vez=1", "vez 1")));
    verificar("save sem nenhuma cidade e recusado", !carregarTexto("BOLONHA_E_MILANO_SAVE v1\nvez=0\n"));
    std::string cinco = "BOLONHA_E_MILANO_SAVE v1\nvez=0\n";
    std::string cidadeTxt = saveBase().substr(saveBase().find("[cidade]"));
    cidadeTxt = cidadeTxt.substr(0, cidadeTxt.find("[cidade]", 1));
    for (int i = 0; i < 5; i++) cinco += cidadeTxt;
    verificar("save com 5 cidades e recusado (maximo 4)", !carregarTexto(cinco));
}

// ----------------------------------------------- Terminal (limpar e pausar)
void Testes::testarTerminal() {
    std::cout << "\n-- Terminal (limpar tela e pausar) --\n";
    bool estavaAtivo = Terminal::isAtivo();

    Terminal::setAtivo(true);
    std::ostringstream saida;
    Terminal::limparTela(saida);
    verificar("limparTela envia a sequencia ANSI de limpeza", saida.str().find("\033[2J") != std::string::npos);

    std::istringstream entrada("\nsobra\n");
    std::ostringstream aviso;
    bool continuou = Terminal::pausar(entrada, aviso);
    verificar("pausar mostra a mensagem do ENTER",
              aviso.str().find("Pressione ENTER para continuar. . .") != std::string::npos);
    verificar("pausar consome so uma linha", continuou && entrada.peek() == 's');
    std::istringstream vazia("");
    std::ostringstream descarte;
    verificar("pausar sem entrada (EOF) devolve false", !Terminal::pausar(vazia, descarte));

    Terminal::setAtivo(false);
    std::ostringstream mudo;
    Terminal::limparTela(mudo);
    std::istringstream intacta("linha\n");
    bool seguiu = Terminal::pausar(intacta, mudo);
    verificar("desligado: nao limpa nem escreve nada", mudo.str().empty());
    verificar("desligado: pausar nao le a entrada", seguiu && intacta.peek() == 'l');

    Terminal::setAtivo(estavaAtivo);
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
    testarPersistencia();
    testarTerminal();
    std::cout << "\nAprovados: " << aprovados << " | Reprovados: " << reprovados << "\n";
    return reprovados > 0 ? 1 : 0;
}

// Segundo main do projeto: so existe no executavel de testes (make test).
int main() {
    return Testes::executar();
}
