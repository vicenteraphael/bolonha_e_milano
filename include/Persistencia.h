#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <iosfwd>
#include <memory>
#include <string>
#include "Partida.h"

// Salva e carrega a partida em arquivo de texto (.txt, uma chave=valor por linha).
// Servico: nao guarda a Partida, ela chega como parametro. Ao carregar, TUDO e
// validado (formato, faixas e regras do Governo) antes de entregar a partida; se algo
// estiver errado devolve nullptr e explica em 'erro', sem tocar na partida atual.
class Persistencia {
private:
    static constexpr const char* CABECALHO = "BOLONHA_E_MILANO_SAVE v1";

public:
    // Versoes com stream (usadas pelos testes e pelas versoes com arquivo).
    void salvar(const Partida& partida, std::ostream& saida) const;
    std::unique_ptr<Partida> carregar(std::istream& entrada, std::string& erro) const;

    // Versoes com arquivo. salvarArquivo devolve false e preenche 'erro' se nao der para gravar.
    bool salvarArquivo(const Partida& partida, const std::string& caminho, std::string& erro) const;
    std::unique_ptr<Partida> carregarArquivo(const std::string& caminho, std::string& erro) const;
};

#endif
