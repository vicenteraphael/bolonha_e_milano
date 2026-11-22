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

Menu::Menu() : emExecucao(true), mercado() {}

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
            if (exigirPartida()) exibirNaoImplementado("Agricultura", 4);
            break;
        case 5:
            if (exigirPartida()) exibirNaoImplementado("Governo", 5);
            break;
        case 6:
            if (exigirPartida()) exibirNaoImplementado("Construcoes", 6);
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

void Menu::exibirNaoImplementado(const char* nome, int etapa) const {
    std::cout << nome << " ainda nao esta disponivel (Etapa " << etapa << ").\n";
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
    std::cout << "Quantidade: ";
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
