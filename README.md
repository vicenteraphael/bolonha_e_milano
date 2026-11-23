# Bolonha e Milano

Jogo de estratégia econômica medieval em linha de comando, escrito em **C++17**.
Projeto do 4º bimestre (SPOTDES), construído em 8 etapas incrementais.

## Estado atual: Etapa 5

- **Etapa 1:** estrutura inicial com as classes `Main` e `Menu` (menu em laço, com validação de entrada).
- **Etapa 2:** classes `Jogador` e `Cidade`. "Novo jogo" cria um jogador e uma cidade de verdade; "Ver status da cidade" mostra os dados.
- **Etapa 3:** economia. A `Cidade` ganha florins (1000), terras (10000 hectares) e grãos (500), e a nova classe `Mercado` compra e vende grãos e terras a preço fixo.
- **Etapa 4:** agricultura e população. A `Cidade` ganha servos (200) e a nova classe `Agricultura` processa a colheita e distribui alimentos (a população cresce 5% ou passa fome).
- **Etapa 5:** impostos e governo. A `Cidade` ganha `taxaImposto` (10%) e `nivelJustica` ("Moderada"), que são configurações (só getter/setter), e a nova classe `Governo` arrecada impostos e valida a taxa (0 a 50) e o nível de justiça.

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
├── casos-de-uso/   diagrama de casos de uso (cumulativo, um por etapa: .png, .svg e .puml)
├── uml/            diagrama de classes (cumulativo, um por etapa: .png, .svg, .dot e .puml)
├── include/        cabeçalhos (.h), um por classe
│   ├── Menu.h
│   ├── Jogador.h
│   ├── Cidade.h
│   ├── Mercado.h
│   ├── Agricultura.h
│   └── Governo.h
└── src/            implementações (.cpp), um por classe
    ├── Main.cpp
    ├── Menu.cpp
    ├── Jogador.cpp
    ├── Cidade.cpp
    ├── Mercado.cpp
    ├── Agricultura.cpp
    └── Governo.cpp
```

## Classes

| Classe    | Responsabilidade |
|-----------|------------------|
| `Main`    | Ponto de entrada: cria o `Menu` e manda executar. |
| `Menu`    | Exibe as opções, lê e valida a entrada, guarda a partida atual (jogador e cidade) e delega para os serviços. |
| `Jogador` | Guarda o nome (final) e o título de nobreza (começa como "Cavaleiro"). |
| `Cidade`  | Guarda nome, ano (1400), dono, florins, terras, grãos, servos, taxa de imposto e nível de justiça. Cada recurso tem `adicionarX` (sempre funciona) e `removerX` (devolve `false` se não houver quantidade suficiente). |
| `Mercado` | Compra e vende grãos (2 florins/unidade) e terras (1 florim/hectare). Recebe a `Cidade` como parâmetro, sem guardar referência a ela. |
| `Agricultura` | `processarColheita` (hectares = min(terras, servos × 5); sementes = hectares ÷ 2; produção = hectares × 2) e `distribuirAlimentos` (percentual de 20 a 80). Também recebe a `Cidade` como parâmetro. |
| `Governo` | `arrecadarImpostos` (base = servos × taxa ÷ 100, vezes o fator da justiça: Justa 0,8 · Moderada 1,0 · Severa 1,3), `definirTaxaImposto` (0 a 50) e `definirJustica` ("Justa", "Moderada" ou "Severa"). Também recebe a `Cidade` como parâmetro. |

## Opções do menu

1. Novo jogo (nomes em branco usam "Jogador 1" e "Bolonha")
2. Ver status da cidade
3. Mercado (comprar/vender grãos, comprar/vender terras)
4. Agricultura (colher e distribuir alimentos)
5. Governo (arrecadar impostos, definir taxa, definir nível de justiça)
6. Construções (Etapa 6)
7. Sobre o jogo
0. Sair

As opções 2 a 6 avisam quando ainda não há partida em andamento.

## Commits e Tags

Mensagens no formato `Etapa N: descrição`.
