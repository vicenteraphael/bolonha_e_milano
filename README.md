# Bolonha e Milano

Jogo de estratégia econômica medieval em linha de comando, escrito em **C++17**.
Projeto do 4º bimestre (SPOTDES), construído em 8 etapas incrementais.

## Estado atual: Etapa 3

- **Etapa 1:** estrutura inicial com as classes `Main` e `Menu` (menu em laço, com validação de entrada).
- **Etapa 2:** classes `Jogador` e `Cidade`. "Novo jogo" cria um jogador e uma cidade de verdade; "Ver status da cidade" mostra os dados.
- **Etapa 3:** economia. A `Cidade` ganha florins (1000), terras (10000 hectares) e grãos (500), e a nova classe `Mercado` compra e vende grãos e terras a preço fixo.

## Como compilar e executar

Requisitos: `g++` com suporte a C++17 e `make` (opcional).

Com Make:

```
make run
```

Sem Make:

```
mkdir -p build
g++ -std=c++17 -Wall -Wextra -I include -o build/bolonhaemilano src/*.cpp
./build/bolonhaemilano
```

Para limpar os arquivos gerados: `make clean`.

## Estrutura do repositório

```
.
├── README.md
├── Makefile
├── .gitignore
├── LICENSE
├── casos-de-uso/   diagrama de casos de uso (cumulativo)
├── uml/            diagrama de classes (cumulativo)
├── include/        cabeçalhos (.h), um por classe
│   ├── Menu.h
│   ├── Jogador.h
│   ├── Cidade.h
│   └── Mercado.h
└── src/            implementações (.cpp), um por classe
    ├── Main.cpp
    ├── Menu.cpp
    ├── Jogador.cpp
    ├── Cidade.cpp
    └── Mercado.cpp
```

## Classes

| Classe    | Responsabilidade |
|-----------|------------------|
| `Main`    | Ponto de entrada: cria o `Menu` e manda executar. |
| `Menu`    | Exibe as opções, lê e valida a entrada, guarda a partida atual (jogador e cidade) e delega para os serviços. |
| `Jogador` | Guarda o nome (final) e o título de nobreza (começa como "Cavaleiro"). |
| `Cidade`  | Guarda nome, ano (1400), dono, florins, terras e grãos. Cada recurso tem `adicionarX` (sempre funciona) e `removerX` (devolve `false` se não houver quantidade suficiente). |
| `Mercado` | Compra e vende grãos (2 florins/unidade) e terras (1 florim/hectare). Recebe a `Cidade` como parâmetro, sem guardar referência a ela. |

## Opções do menu

1. Novo jogo (nomes em branco usam "Jogador 1" e "Bolonha")
2. Ver status da cidade
3. Mercado (comprar/vender grãos, comprar/vender terras)
4. Agricultura (Etapa 4)
5. Governo (Etapa 5)
6. Construções (Etapa 6)
7. Sobre o jogo
0. Sair

As opções 2 a 6 avisam quando ainda não há partida em andamento.

## Commits e Tags

Mensagens no formato `Etapa N: descrição`.
