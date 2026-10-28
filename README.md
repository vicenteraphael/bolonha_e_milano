# Bolonha e Milano

Jogo de estratégia econômica medieval em linha de comando, escrito em **C++17**.
Projeto do 4º bimestre (SPOTDES), construído em 8 etapas incrementais.

## Estado atual: Etapa 1

- **Etapa 1:** estrutura inicial com as classes `Main` e `Menu` (menu em laço, com validação de entrada).
- **Etapa 2:** classes `Jogador` e `Cidade`. "Novo jogo" cria um jogador e uma cidade de verdade, e "Ver status da cidade" mostra os dados (ou avisa que nenhum jogo foi iniciado).

## Como compilar e executar

Requisitos: `g++` com suporte a C++17 e `make` (opcional).

Com Make:

```
make run
```

Sem Make:

```
mkdir -p build
g++ -std=c++17 -Wall -Wextra -I src -I include -o build/bolonhaemilano src/*.cpp
./build/bolonhaemilano
```

Para limpar os arquivos gerados: `make clean`.

## Estrutura do repositório

```
.
├── README.md
├── Makefile
├── .gitignore
├── uml/            diagrama de classes (cumulativo)
└── src/            código-fonte (.cpp, um por classe)
|    ├── Main.cpp
|    ├── Menu.cpp
└── include/            código-fonte (.h, um por classe)
|    ├── Menu.h
├── casos-de-uso/   diagrama de casos de uso (cumulativo)
```

## Classes

| Classe    | Responsabilidade |
|-----------|------------------|
| `Main`    | Ponto de entrada: cria o `Menu` e manda executar. |
| `Menu`    | Exibe as opções, lê e valida a escolha e controla o laço principal. |
| `Jogador` | Guarda o nome e o título de nobreza (começa como "Cavaleiro"). |
| `Cidade`  | Guarda o nome, o ano atual (começa em 1400) e o jogador dono. |

## Opções do menu

1. Novo jogo
2. Ver status da cidade
3. Sobre o jogo
0. Sair

## Commits

Mensagens no formato `Etapa N: descrição`.