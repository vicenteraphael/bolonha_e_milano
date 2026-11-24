#include "Persistencia.h"
#include <fstream>
#include <limits>
#include <stdexcept>
#include <map>
#include <sstream>
#include <vector>
#include "Governo.h"

namespace {
using Secao = std::map<std::string, std::string>;

std::string aparar(const std::string& s) {
    const char* espacos = " \t\r\n";
    std::size_t ini = s.find_first_not_of(espacos);
    if (ini == std::string::npos) return "";
    return s.substr(ini, s.find_last_not_of(espacos) - ini + 1);
}

// Inteiro estrito (sem lixo no fim) dentro de [minimo, maximo].
bool lerInt(const Secao& s, const std::string& chave, int minimo, int maximo, int& valor,
            std::string& erro) {
    auto it = s.find(chave);
    if (it == s.end()) { erro = "campo ausente: " + chave; return false; }
    try {
        std::size_t pos = 0;
        long long v = std::stoll(it->second, &pos);
        if (pos != it->second.size() || v < minimo || v > maximo) throw std::out_of_range("");
        valor = static_cast<int>(v);
        return true;
    } catch (...) {
        erro = "valor invalido em " + chave + ": \"" + it->second + "\"";
        return false;
    }
}

bool lerTexto(const Secao& s, const std::string& chave, std::string& valor, std::string& erro) {
    auto it = s.find(chave);
    if (it == s.end() || it->second.empty()) { erro = "campo ausente ou vazio: " + chave; return false; }
    valor = it->second;
    return true;
}
}  // namespace

constexpr const char* Persistencia::CABECALHO;

void Persistencia::salvar(const Partida& partida, std::ostream& saida) const {
    saida << CABECALHO << "\n"
          << "vez=" << partida.getIndiceAtual() << "\n";
    for (const auto& c : partida.getCidades()) {
        EstadoCidade e = c->getEstado();
        saida << "[cidade]\n"
              << "jogador=" << c->getDono()->getNome() << "\n"
              << "titulo=" << c->getDono()->getNivelTitulo() << "\n"
              << "nome=" << c->getNome() << "\n"
              << "ano=" << e.ano << "\n"
              << "florins=" << e.florins << "\n"
              << "terras=" << e.terras << "\n"
              << "graos=" << e.graos << "\n"
              << "servos=" << e.servos << "\n"
              << "soldados=" << e.soldados << "\n"
              << "construcoes=" << e.construcoes << "\n"
              << "taxa=" << c->getTaxaImposto() << "\n"
              << "justica=" << c->getNivelJustica() << "\n"
              << "colheita=" << (e.colheitaFeita ? 1 : 0) << "\n"
              << "impostos=" << (e.impostosArrecadados ? 1 : 0) << "\n";
    }
}

std::unique_ptr<Partida> Persistencia::carregar(std::istream& entrada, std::string& erro) const {
    const int MAX_INT = std::numeric_limits<int>::max();
    std::string linha;
    if (!std::getline(entrada, linha) || aparar(linha) != CABECALHO) {
        erro = "nao e um arquivo de jogo valido (cabecalho incorreto)";
        return nullptr;
    }

    // Le "chave=valor" em secoes: a primeira e o cabecalho geral, depois uma por [cidade].
    std::vector<Secao> secoes(1);
    int numLinha = 1;
    while (std::getline(entrada, linha)) {
        numLinha++;
        linha = aparar(linha);
        if (linha.empty()) continue;
        if (linha == "[cidade]") { secoes.emplace_back(); continue; }
        std::size_t igual = linha.find('=');
        if (igual == std::string::npos || igual == 0) {
            erro = "linha " + std::to_string(numLinha) + " mal formada";
            return nullptr;
        }
        secoes.back()[aparar(linha.substr(0, igual))] = aparar(linha.substr(igual + 1));
    }

    int quantidade = static_cast<int>(secoes.size()) - 1;
    Partida modelo;   // so para ler o limite de jogadores
    if (quantidade < 1 || quantidade > modelo.getMaximoJogadores()) {
        erro = "quantidade de cidades invalida (de 1 a " + std::to_string(modelo.getMaximoJogadores()) + ")";
        return nullptr;
    }
    int vez = 0;
    if (!lerInt(secoes[0], "vez", 0, quantidade - 1, vez, erro)) return nullptr;

    auto partida = std::make_unique<Partida>();
    Governo governo;
    int anoDaPartida = 0;
    for (int i = 1; i <= quantidade; i++) {
        const Secao& s = secoes[i];
        std::string nomeJogador, nomeCidade;
        int titulo, taxa, colheita, impostos;
        EstadoCidade e{};
        // Titulo 0..4: uma partida salva nunca tem um Rei/Rainha (o jogo ja teria acabado).
        if (!lerTexto(s, "jogador", nomeJogador, erro) || !lerTexto(s, "nome", nomeCidade, erro) ||
            !lerInt(s, "titulo", 0, 4, titulo, erro) ||
            !lerInt(s, "ano", 1400, 1410, e.ano, erro) ||
            !lerInt(s, "florins", 0, MAX_INT, e.florins, erro) ||
            !lerInt(s, "terras", 0, MAX_INT, e.terras, erro) ||
            !lerInt(s, "graos", 0, MAX_INT, e.graos, erro) ||
            !lerInt(s, "servos", 0, MAX_INT, e.servos, erro) ||
            !lerInt(s, "soldados", 0, MAX_INT, e.soldados, erro) ||
            !lerInt(s, "construcoes", 0, MAX_INT, e.construcoes, erro) ||
            !lerInt(s, "taxa", 0, MAX_INT, taxa, erro) ||
            !lerInt(s, "colheita", 0, 1, colheita, erro) ||
            !lerInt(s, "impostos", 0, 1, impostos, erro)) {
            erro = "cidade " + std::to_string(i) + ": " + erro;
            return nullptr;
        }
        std::string justica;
        if (!lerTexto(s, "justica", justica, erro)) { erro = "cidade " + std::to_string(i) + ": " + erro; return nullptr; }
        e.colheitaFeita = colheita == 1;
        e.impostosArrecadados = impostos == 1;

        if (i == 1) anoDaPartida = e.ano;
        if (e.ano != anoDaPartida) { erro = "as cidades estao em anos diferentes"; return nullptr; }

        auto jogador = std::make_unique<Jogador>(nomeJogador);
        for (int n = 0; n < titulo; n++) jogador->promover();
        auto cidade = std::make_unique<Cidade>(nomeCidade, jogador.get());
        // Taxa e justica passam pelas regras do Governo (mesma validacao do jogo).
        if (!cidade->restaurarEstado(e) || !governo.definirTaxaImposto(*cidade, taxa) ||
            !governo.definirJustica(*cidade, justica)) {
            erro = "cidade " + std::to_string(i) + ": valores fora das regras do jogo";
            return nullptr;
        }
        partida->adicionarCidade(std::move(jogador), std::move(cidade));
    }
    partida->restaurarVez(vez);
    return partida;
}

bool Persistencia::salvarArquivo(const Partida& partida, const std::string& caminho,
                                 std::string& erro) const {
    std::ofstream arquivo(caminho);
    if (!arquivo) { erro = "nao foi possivel abrir \"" + caminho + "\" para escrita"; return false; }
    salvar(partida, arquivo);
    arquivo.flush();
    if (!arquivo) { erro = "falha ao gravar \"" + caminho + "\""; return false; }
    return true;
}

std::unique_ptr<Partida> Persistencia::carregarArquivo(const std::string& caminho,
                                                       std::string& erro) const {
    std::ifstream arquivo(caminho);
    if (!arquivo) { erro = "arquivo \"" + caminho + "\" nao encontrado"; return nullptr; }
    return carregar(arquivo, erro);
}
