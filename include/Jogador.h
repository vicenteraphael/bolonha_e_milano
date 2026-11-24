#ifndef JOGADOR_H
#define JOGADOR_H

#include <string>

// Quem governa a cidade: nome (nunca muda) e nivel de nobreza.
// O titulo nao e escolhido de fora: so sobe um nivel por vez, com promover().
class Jogador {
private:
    static constexpr int NUM_TITULOS = 6;
    static constexpr const char* TITULOS[NUM_TITULOS] = {
        "Cavaleiro", "Barao", "Conde", "Marques", "Duque", "Rei/Rainha"};

    const std::string nome;   // final: definido uma vez no construtor
    int nivelTitulo;          // indice em TITULOS (0 = Cavaleiro)

public:
    explicit Jogador(const std::string& nome);

    std::string getNome() const;
    std::string getTitulo() const;
    int getNivelTitulo() const;
    bool promover();          // false se ja e Rei/Rainha
    bool ehMonarca() const;   // true no ultimo titulo
};

#endif
