#ifndef JOGADOR_H
#define JOGADOR_H

#include <string>

// Quem governa a cidade: nome (nunca muda) e titulo de nobreza ("Cavaleiro").
class Jogador {
private:
    const std::string nome;   // final: definido uma vez no construtor
    std::string titulo;       // sobe de nivel a partir da Etapa 7

public:
    explicit Jogador(const std::string& nome);

    std::string getNome() const;
    std::string getTitulo() const;
    void setTitulo(const std::string& titulo);
};

#endif
