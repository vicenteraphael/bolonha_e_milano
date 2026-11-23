#include "Menu.h"
#include <iostream>
#include <string>
#include <utility>

namespace {
// Remove espacos no inicio e no fim (equivalente ao trim() do Java).
std::string trim(const std::string& s) {
    const char* espacos = " \t\r\n";
    std::size_t ini = s.find_first_not_of(espacos);
    if (ini == std::string::npos) return "";
    std::size_t fim = s.find_last_not_of(espacos);
    return s.substr(ini, fim - ini + 1);
}

// Converte uma linha em inteiro. Devolve false se nao for um numero inteiro valido
// (texto, "2abc", vazio ou fora do limite do int).
bool paraInteiro(const std::string& linha, int& valor) {
    std::string t = trim(linha);
    try {
        std::size_t pos = 0;
        int v = std::stoi(t, &pos);
        if (pos != t.size()) return false;
        valor = v;
        return true;
    } catch (...) {
        return false;
    }
}

const char* const NOME_PADRAO_JOGADOR = "Jogador 1";
const char* const NOME_PADRAO_CIDADE  = "Bolonha";
}  // namespace

Menu::Menu() : emExecucao(true), mercado(), agricultura(), governo(), construcoes() {}

void Menu::executar() {
    while (emExecucao) {
        exibirOpcoes();
        int opcao = lerOpcao();
        if (!emExecucao) break;   // fim da entrada (EOF)
        processarOpcao(opcao);
    }
}

void Menu::exibirOpcoes() const {
    std::cout << "\n==========================================\n"
              << "  BOLONHA E MILANO\n"
              << "  Um jogo de estrategia economica medieval\n"
              << "==========================================\n"
              << "1 - Novo jogo\n"
              << "2 - Ver status da cidade\n"
              << "3 - Mercado\n"
              << "4 - Agricultura\n"
              << "5 - Governo\n"
              << "6 - Construcoes\n"
              << "7 - Sobre o jogo\n"
              << "0 - Sair\n"
              << "Escolha uma opcao: ";
}

// Devolve -1 se a entrada nao for um numero inteiro valido.
// No fim da entrada (EOF) encerra o jogo, para nao ficar em laco infinito.
int Menu::lerOpcao() {
    std::string linha;
    if (!std::getline(std::cin, linha)) {
        emExecucao = false;
        return 0;
    }
    int valor = 0;
    return paraInteiro(linha, valor) ? valor : -1;
}

void Menu::processarOpcao(int opcao) {
    switch (opcao) {
        case 1:
            iniciarNovoJogo();
            break;
        case 2:
            exibirStatus();
            break;
        case 3:
            if (exigirPartida()) abrirMercado();
            break;
        case 4:
            if (exigirPartida()) abrirAgricultura();
            break;
        case 5:
            if (exigirPartida()) abrirGoverno();
            break;
        case 6:
            if (exigirPartida()) abrirConstrucoes();
            break;
        case 7:
            exibirSobre();
            break;
        case 0:
            emExecucao = false;
            std::cout << "Ate a proxima!\n";
            break;
        default:
            std::cout << "Opcao invalida. Tente novamente.\n";
    }
}

void Menu::iniciarNovoJogo() {
    std::string nomeJogador, nomeCidade;

    std::cout << "Digite o nome do jogador: ";
    if (!std::getline(std::cin, nomeJogador)) { emExecucao = false; return; }
    nomeJogador = trim(nomeJogador);
    if (nomeJogador.empty()) nomeJogador = NOME_PADRAO_JOGADOR;

    std::cout << "Digite o nome da cidade: ";
    if (!std::getline(std::cin, nomeCidade)) { emExecucao = false; return; }
    nomeCidade = trim(nomeCidade);
    if (nomeCidade.empty()) nomeCidade = NOME_PADRAO_CIDADE;

    auto novoJogador = std::make_unique<Jogador>(nomeJogador);
    auto novaCidade = std::make_unique<Cidade>(nomeCidade, novoJogador.get());

    // Substitui a cidade antes do jogador, para nunca existir um dono "pendurado".
    cidade = std::move(novaCidade);
    jogador = std::move(novoJogador);

    cidade->exibirStatus();
}

bool Menu::exigirPartida() const {
    if (!cidade) {
        std::cout << "Nao ha partida em andamento. Escolha 'Novo jogo' primeiro.\n";
        return false;
    }
    return true;
}

void Menu::exibirStatus() const {
    if (!exigirPartida()) return;
    cidade->exibirStatus();
}

void Menu::exibirSobre() const {
    std::cout << "\nBolonha e Milano e um jogo de estrategia economica e politica,\n"
              << "ambientado na Italia medieval. Voce governa uma cidade-Estado e\n"
              << "precisa equilibrar agricultura, impostos, construcoes e defesa\n"
              << "para fazer a cidade prosperar.\n";
}

// ---------------------------------------------------------------- Mercado

void Menu::exibirOpcoesMercado() const {
    std::cout << "\n--- Mercado ---\n"
              << "Preco do grao (florins/unidade):  " << mercado.getPrecoGrao() << "\n"
              << "Preco da terra (florins/hectare): " << mercado.getPrecoTerra() << "\n"
              << "Seus recursos:  " << cidade->getFlorins() << " florins, "
              << cidade->getTerras() << " hectares, "
              << cidade->getGraos() << " graos\n"
              << "1 - Comprar graos\n"
              << "2 - Vender graos\n"
              << "3 - Comprar terras\n"
              << "4 - Vender terras\n"
              << "0 - Voltar\n"
              << "Escolha uma opcao: ";
}

void Menu::abrirMercado() {
    bool noMercado = true;
    while (noMercado && emExecucao) {
        exibirOpcoesMercado();
        int opcao = lerOpcao();
        if (!emExecucao) break;
        switch (opcao) {
            case 1: comprarGraos();  break;
            case 2: venderGraos();   break;
            case 3: comprarTerras(); break;
            case 4: venderTerras();  break;
            case 0: noMercado = false; break;
            default: std::cout << "Opcao invalida. Tente novamente.\n";
        }
    }
}

// Le e valida a quantidade. Devolve -1 se nao for um numero inteiro valido
// (o Mercado tambem recusa quantidades <= 0).
int Menu::lerQuantidade() {
    return lerInteiro("Quantidade: ");
}

// Mostra o prompt e le um inteiro (-1 se invalido). Fim da entrada encerra o jogo.
int Menu::lerInteiro(const char* prompt) {
    std::cout << prompt;
    std::string linha;
    if (!std::getline(std::cin, linha)) {
        emExecucao = false;
        return -1;
    }
    int valor = 0;
    return paraInteiro(linha, valor) ? valor : -1;
}

void Menu::informarResultado(bool sucesso, const char* sucessoMsg) const {
    if (sucesso) {
        std::cout << sucessoMsg << "\n";
    } else {
        std::cout << "Operacao recusada: quantidade invalida ou recursos insuficientes.\n";
    }
}

void Menu::comprarGraos() {
    int quantidade = lerQuantidade();
    if (!emExecucao) return;
    informarResultado(mercado.comprarGraos(*cidade, quantidade), "Graos comprados com sucesso.");
}

void Menu::venderGraos() {
    int quantidade = lerQuantidade();
    if (!emExecucao) return;
    informarResultado(mercado.venderGraos(*cidade, quantidade), "Graos vendidos com sucesso.");
}

void Menu::comprarTerras() {
    int quantidade = lerQuantidade();
    if (!emExecucao) return;
    informarResultado(mercado.comprarTerras(*cidade, quantidade), "Terras compradas com sucesso.");
}

void Menu::venderTerras() {
    int quantidade = lerQuantidade();
    if (!emExecucao) return;
    informarResultado(mercado.venderTerras(*cidade, quantidade), "Terras vendidas com sucesso.");
}

// ------------------------------------------------------------ Agricultura

void Menu::exibirOpcoesAgricultura() const {
    std::cout << "\n--- Agricultura ---\n"
              << "Seus recursos:  " << cidade->getTerras() << " hectares, "
              << cidade->getServos() << " servos, "
              << cidade->getGraos() << " graos\n"
              << "1 - Colher\n"
              << "2 - Distribuir alimentos\n"
              << "0 - Voltar\n"
              << "Escolha uma opcao: ";
}

void Menu::abrirAgricultura() {
    bool naAgricultura = true;
    while (naAgricultura && emExecucao) {
        exibirOpcoesAgricultura();
        int opcao = lerOpcao();
        if (!emExecucao) break;
        switch (opcao) {
            case 1: colher();               break;
            case 2: distribuirAlimentos();  break;
            case 0: naAgricultura = false;  break;
            default: std::cout << "Opcao invalida. Tente novamente.\n";
        }
    }
}

// So delega para a Agricultura e imprime o resultado.
void Menu::colher() {
    int graosAntes = cidade->getGraos();
    int hectares = agricultura.processarColheita(*cidade);
    std::cout << "Colheita realizada: " << hectares << " hectares cultivados.\n"
              << "Graos: " << graosAntes << " -> " << cidade->getGraos() << "\n";
}

void Menu::distribuirAlimentos() {
    std::cout << "Percentual de graos a distribuir ("
              << agricultura.getPercentualMinimo() << " a "
              << agricultura.getPercentualMaximo() << ").\n";
    int percentual = lerInteiro("Percentual: ");
    if (!emExecucao) return;

    int graosAntes = cidade->getGraos();
    int servosAntes = cidade->getServos();
    if (!agricultura.distribuirAlimentos(*cidade, percentual)) {
        std::cout << "Operacao recusada: o percentual deve estar entre "
                  << agricultura.getPercentualMinimo() << " e "
                  << agricultura.getPercentualMaximo() << ".\n";
        return;
    }
    std::cout << "Alimentos distribuidos: "
              << graosAntes - cidade->getGraos() << " graos consumidos.\n"
              << "Graos: " << graosAntes << " -> " << cidade->getGraos() << "\n"
              << "Servos: " << servosAntes << " -> " << cidade->getServos() << "\n";
}

// ---------------------------------------------------------------- Governo

void Menu::exibirOpcoesGoverno() const {
    std::cout << "\n--- Governo ---\n"
              << "Seus recursos:  " << cidade->getFlorins() << " florins, "
              << cidade->getServos() << " servos\n"
              << "Configuracao:   imposto " << cidade->getTaxaImposto()
              << "%, justica " << cidade->getNivelJustica() << "\n"
              << "1 - Arrecadar impostos\n"
              << "2 - Definir taxa de imposto\n"
              << "3 - Definir nivel de justica\n"
              << "0 - Voltar\n"
              << "Escolha uma opcao: ";
}

void Menu::abrirGoverno() {
    bool noGoverno = true;
    while (noGoverno && emExecucao) {
        exibirOpcoesGoverno();
        int opcao = lerOpcao();
        if (!emExecucao) break;
        switch (opcao) {
            case 1: arrecadarImpostos();   break;
            case 2: definirTaxaImposto();  break;
            case 3: definirJustica();      break;
            case 0: noGoverno = false;     break;
            default: std::cout << "Opcao invalida. Tente novamente.\n";
        }
    }
}

// So delega para o Governo e imprime o resultado.
void Menu::arrecadarImpostos() {
    int florinsAntes = cidade->getFlorins();
    int arrecadado = governo.arrecadarImpostos(*cidade);
    std::cout << "Impostos arrecadados: " << arrecadado << " florins.\n"
              << "Florins: " << florinsAntes << " -> " << cidade->getFlorins() << "\n";
}

void Menu::definirTaxaImposto() {
    std::cout << "A taxa deve estar entre 0 e " << governo.getTaxaMaxima() << " (%).\n";
    int taxa = lerInteiro("Nova taxa: ");
    if (!emExecucao) return;

    if (governo.definirTaxaImposto(*cidade, taxa)) {
        std::cout << "Taxa de imposto definida: " << cidade->getTaxaImposto() << "%.\n";
    } else {
        std::cout << "Operacao recusada: a taxa deve estar entre 0 e "
                  << governo.getTaxaMaxima() << ".\n";
    }
}

// O nivel de justica e escolhido num submenu numerado, nunca digitado livremente.
void Menu::definirJustica() {
    std::cout << "\nNivel de justica:\n"
              << "1 - Justa (arrecada 20% a menos)\n"
              << "2 - Moderada (arrecadacao padrao)\n"
              << "3 - Severa (arrecada 30% a mais)\n";
    int escolha = lerInteiro("Escolha uma opcao: ");
    if (!emExecucao) return;

    std::string nivel;
    switch (escolha) {
        case 1: nivel = "Justa";    break;
        case 2: nivel = "Moderada"; break;
        case 3: nivel = "Severa";   break;
        default:
            std::cout << "Opcao invalida. O nivel de justica nao foi alterado.\n";
            return;
    }
    if (governo.definirJustica(*cidade, nivel)) {
        std::cout << "Nivel de justica definido: " << cidade->getNivelJustica() << ".\n";
    } else {
        std::cout << "Operacao recusada.\n";
    }
}

// ------------------------------------------------------------ Construcoes

void Menu::exibirOpcoesConstrucoes() const {
    std::cout << "\n--- Construcoes ---\n"
              << "Custo:          soldado = " << construcoes.getPrecoSoldado()
              << " florins + 1 servo; construcao = " << construcoes.getPrecoConstrucao()
              << " florins\n"
              << "Seus recursos:  " << cidade->getFlorins() << " florins, "
              << cidade->getServos() << " servos, "
              << cidade->getSoldados() << " soldados, "
              << cidade->getConstrucoes() << " construcoes\n"
              << "1 - Recrutar soldados\n"
              << "2 - Construir\n"
              << "0 - Voltar\n"
              << "Escolha uma opcao: ";
}

void Menu::abrirConstrucoes() {
    bool naConstrucoes = true;
    while (naConstrucoes && emExecucao) {
        exibirOpcoesConstrucoes();
        int opcao = lerOpcao();
        if (!emExecucao) break;
        switch (opcao) {
            case 1: recrutarSoldados();     break;
            case 2: construir();            break;
            case 0: naConstrucoes = false;  break;
            default: std::cout << "Opcao invalida. Tente novamente.\n";
        }
    }
}

// So delega para Construcoes e imprime o resultado.
void Menu::recrutarSoldados() {
    int quantidade = lerQuantidade();
    if (!emExecucao) return;

    int florinsAntes = cidade->getFlorins();
    int servosAntes = cidade->getServos();
    if (construcoes.recrutarSoldados(*cidade, quantidade)) {
        std::cout << "Soldados recrutados: " << quantidade << ".\n"
                  << "Florins: " << florinsAntes << " -> " << cidade->getFlorins() << "\n"
                  << "Servos: " << servosAntes << " -> " << cidade->getServos() << "\n"
                  << "Soldados: " << cidade->getSoldados() << "\n";
    } else {
        std::cout << "Operacao recusada: quantidade invalida ou faltam florins/servos.\n";
    }
}

void Menu::construir() {
    int quantidade = lerQuantidade();
    if (!emExecucao) return;

    int florinsAntes = cidade->getFlorins();
    if (construcoes.construir(*cidade, quantidade)) {
        std::cout << "Construcoes erguidas: " << quantidade << ".\n"
                  << "Florins: " << florinsAntes << " -> " << cidade->getFlorins() << "\n"
                  << "Construcoes: " << cidade->getConstrucoes() << "\n";
    } else {
        std::cout << "Operacao recusada: quantidade invalida ou faltam florins.\n";
    }
}
