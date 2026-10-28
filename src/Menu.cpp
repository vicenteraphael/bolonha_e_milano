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
}  // namespace

Menu::Menu() : emExecucao(true) {}

void Menu::executar() {
    while (emExecucao) {
        exibirOpcoes();
        int opcao = lerOpcao();
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
              << "3 - Sobre o jogo\n"
              << "0 - Sair\n"
              << "Escolha uma opcao: ";
}

// Devolve -1 se a entrada nao for um numero inteiro valido.
int Menu::lerOpcao() {
    std::string linha;
    if (!std::getline(std::cin, linha)) {  // fim da entrada (EOF): encerra
        emExecucao = false;
        return 0;
    }
    try {
        std::size_t pos = 0;
        int valor = std::stoi(trim(linha), &pos);
        if (pos != trim(linha).size()) return -1;  // sobrou texto (ex.: "2abc")
        return valor;
    } catch (...) {
        return -1;
    }
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
    std::getline(std::cin, nomeJogador);
    nomeJogador = trim(nomeJogador);

    std::cout << "Digite o nome da cidade: ";
    std::getline(std::cin, nomeCidade);
    nomeCidade = trim(nomeCidade);

    auto novoJogador = std::make_unique<Jogador>(nomeJogador);
    auto novaCidade = std::make_unique<Cidade>(nomeCidade, novoJogador.get());

    // Substitui a cidade antes do jogador, para nunca existir um dono "pendurado".
    cidade = std::move(novaCidade);
    jogador = std::move(novoJogador);

    cidade->exibirStatus();
}

void Menu::exibirStatus() const {
    if (!cidade) {
        std::cout << "Nenhum jogo iniciado. Escolha 'Novo jogo' primeiro.\n";
        return;
    }
    cidade->exibirStatus();
}

void Menu::exibirSobre() const {
    std::cout << "\nBolonha e Milano e um jogo de estrategia economica medieval.\n"
              << "Voce governa uma cidade italiana, administrando terras, graos,\n"
              << "populacao e impostos, e tenta prosperar a cada ano.\n";
}
