# Bolonha e Milano

Jogo de estratégia econômica medieval em linha de comando, escrito em **C++17**.
Projeto do 4º bimestre (SPOTDES), construído em 8 etapas incrementais.

## Estado atual: Etapa 8 (entrega final) + melhorias extras

- **Etapa 1:** estrutura inicial com as classes `Main` e `Menu` (menu em laço, com validação de entrada).
- **Etapa 2:** classes `Jogador` e `Cidade`. "Novo jogo" cria um jogador e uma cidade de verdade; "Ver status da cidade" mostra os dados.
- **Etapa 3:** economia. A `Cidade` ganha florins (1000), terras (10000 hectares) e grãos (500), e a nova classe `Mercado` compra e vende grãos e terras a preço fixo.
- **Etapa 4:** agricultura e população. A `Cidade` ganha servos (200) e a nova classe `Agricultura` processa a colheita e distribui alimentos (a população cresce 5% ou passa fome).
- **Etapa 5:** impostos e governo. A `Cidade` ganha `taxaImposto` (10%) e `nivelJustica` ("Moderada"), que são configurações (só getter/setter), e a nova classe `Governo` arrecada impostos e valida a taxa (0 a 50) e o nível de justiça.
- **Etapa 6:** construções e soldados. A `Cidade` ganha `soldados` e `construcoes` (0 iniciais, recursos acumuláveis) e a nova classe `Construcoes` recruta soldados (5 florins + 1 servo cada, validando os dois recursos antes de alterar qualquer um) e constrói (20 florins cada).
- **Etapa 7:** rodadas, múltiplos jogadores e vitória. A nova classe `Partida` guarda de 1 a 4 cidades, controla a vez e fecha o ano quando o último jogador encerra o turno. A `Cidade` ganha os indicadores "já colheu / já arrecadou neste ano" (colher e arrecadar passam a valer uma vez por ano). A nova classe `Nobreza` calcula o prestígio e promove o governante (no máximo um título por ano). Vence quem virar Rei/Rainha primeiro, ou o melhor colocado ao fim de 1410.
- **Etapa 8:** integração, testes e entrega. Nova classe `Testes` (com `main` próprio, sem biblioteca externa) no padrão preparar → executar → verificar, com casos de erro para cada classe de regra. O `Menu` foi refatorado sem mudar o comportamento: `lerInteiro()` concentra a leitura/conversão e `exigirJogoEmAndamento()` concentra o aviso "Nao ha partida em andamento". Diagramas finais, incluindo um diagrama de sequência.
- **Extras (além do enunciado):**
  - **Salvar/carregar** a partida em arquivo `.txt` (classe `Persistencia`).
  - **Limpeza de tela** a cada interação e **pausa** "Pressione ENTER para continuar. . ." (classe `Terminal`).
  - O cabeçalho do menu passa a mostrar o ano e de quem é a vez.

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

### Modo simples (sem limpar tela e sem pausas)

```
./build/bolonhaemilano --simples
```

Com `--simples` o jogo não limpa a tela nem pede ENTER. É o modo indicado para rodar com entrada redirecionada (`./build/bolonhaemilano --simples < roteiro.txt`); no modo normal cada pausa consumiria uma linha do roteiro. A limpeza de tela usa sequências ANSI: funciona em terminais Linux/macOS, Git Bash/MSYS2 e no Windows 10+ (o programa liga o modo virtual do console).

## Como rodar os testes

```
make test
```

Sem Make:

```
mkdir -p build
g++ -std=c++17 -Wall -Wextra -I include -I tests -o build/testes $(ls src/*.cpp | grep -v Main.cpp) tests/Testes.cpp
./build/testes
```

Cada verificação imprime `[OK]` ou `[FALHA]` com a descrição da regra. No fim aparece `Aprovados: N | Reprovados: M` (hoje: 108 e 0); se alguma falhar, o programa termina com código 1. A classe `Testes` tem o seu próprio `main` e usa tudo de `src/` exceto o `Main.cpp` do jogo; nenhuma classe do jogo conhece `Testes`.

## Estrutura do repositório

```
.
├── README.md
├── Makefile
├── .gitignore
├── LICENSE
├── casos-de-uso/   diagrama de casos de uso (cumulativo, um por etapa: .png, .svg e .puml)
├── uml/            diagramas de classes (cumulativos, um por etapa: .png, .svg, .dot e .puml) e de sequência (Etapa 8 e carregar jogo); os diagramas "extras" mostram o projeto completo
├── tests/          testes automatizados (Testes.h e Testes.cpp, com main próprio)
├── include/        cabeçalhos (.h), um por classe
│   ├── Menu.h
│   ├── Jogador.h
│   ├── Cidade.h
│   ├── Mercado.h
│   ├── Agricultura.h
│   ├── Governo.h
│   ├── Construcoes.h
│   ├── Nobreza.h
│   ├── Partida.h
│   ├── Terminal.h
│   └── Persistencia.h
└── src/            implementações (.cpp), um por classe
    ├── Main.cpp
    ├── Menu.cpp
    ├── Jogador.cpp
    ├── Cidade.cpp
    ├── Mercado.cpp
    ├── Agricultura.cpp
    ├── Governo.cpp
    ├── Construcoes.cpp
    ├── Nobreza.cpp
    ├── Partida.cpp
    ├── Terminal.cpp
    └── Persistencia.cpp
```

## Classes

| Classe    | Responsabilidade |
|-----------|------------------|
| `Main`    | Ponto de entrada: cria o `Menu` e manda executar. |
| `Menu`    | Exibe as opções, lê e valida a entrada, guarda a `Partida` e a cidade da vez e delega para os serviços. |
| `Jogador` | Guarda o nome (final) e o nível de título (Cavaleiro, Barão, Conde, Marquês, Duque, Rei/Rainha). Só sobe um nível por vez, com `promover()`. |
| `Cidade`  | Guarda nome, ano (1400), dono, florins, terras, grãos, servos, taxa de imposto, nível de justiça, soldados, construções e os indicadores "colheita feita / impostos arrecadados" do ano (zerados por `avancarAno`). Cada recurso tem `adicionarX` (sempre funciona) e `removerX` (devolve `false` se não houver quantidade suficiente). |
| `Mercado` | Compra e vende grãos (2 florins/unidade) e terras (1 florim/hectare). Recebe a `Cidade` como parâmetro, sem guardar referência a ela. |
| `Agricultura` | `processarColheita` (hectares = min(terras, servos × 5); sementes = hectares ÷ 2; produção = hectares × 2) e `distribuirAlimentos` (percentual de 20 a 80). Só colhe uma vez por ano (devolve -1 se já colheu). Também recebe a `Cidade` como parâmetro. |
| `Governo` | `arrecadarImpostos` (base = servos × taxa ÷ 100, vezes o fator da justiça: Justa 0,8 · Moderada 1,0 · Severa 1,3), `definirTaxaImposto` (0 a 50) e `definirJustica` ("Justa", "Moderada" ou "Severa"). Só arrecada uma vez por ano (devolve -1 se já arrecadou). Também recebe a `Cidade` como parâmetro. |
| `Construcoes` | `recrutarSoldados` (5 florins + 1 servo por soldado; valida florins **e** servos antes de debitar qualquer um) e `construir` (20 florins por construção). Também recebe a `Cidade` como parâmetro. |
| `Nobreza` | Prestígio = construções × 10 + soldados × 2 + servos + terras ÷ 100 (florins não contam). Promove o governante quando o prestígio alcança o mínimo do próximo título (400, 550, 700, 850, 1000), no máximo um título por chamada. |
| `Persistencia` | `salvar`/`carregar` (e as versões `salvarArquivo`/`carregarArquivo`). Ao carregar, valida TUDO antes de entregar a partida: cabeçalho, faixas dos valores, título (nunca Rei/Rainha), cidades no mesmo ano, e a taxa e a justiça pelas regras do `Governo`. Arquivo inválido devolve `nullptr` com a explicação e a partida atual é mantida. |
| `Terminal` | `limparTela()` (ANSI) e `pausar()` ("Pressione ENTER para continuar. . ."). `setAtivo(false)` desliga os dois (modo `--simples`). |
| `Testes` | Testes automatizados (`make test`): `verificar`, `novaCidade` e um grupo de testes por classe de regra, incluindo salvar/carregar (ida e volta, arquivo real e 13 tipos de arquivo inválido) e o `Terminal`. Existe só para a verificação; o jogo funciona igual sem ela. |
| `Partida` | Guarda de 1 a 4 jogadores/cidades, controla a vez (`passarTurno`), fecha o ano (`encerrarAno`), detecta o fim de jogo (`terminou`: alguém virou Rei/Rainha ou o ano passou de 1410) e escolhe o vencedor (título mais alto; empate: maior prestígio). |

## Opções do menu

1. Novo jogo (1 a 4 jogadores; nomes em branco usam "Jogador N" e Bolonha, Milano, Florenca, Veneza)
2. Ver status da cidade
3. Mercado (comprar/vender grãos, comprar/vender terras)
4. Agricultura (colher e distribuir alimentos)
5. Governo (arrecadar impostos, definir taxa, definir nível de justiça)
6. Construções (recrutar soldados, construir)
7. Encerrar turno (o último jogador da rodada fecha o ano)
8. Ver classificação
9. Salvar jogo (pede o nome do arquivo; ENTER = `savegame.txt`; `.txt` é acrescentado se faltar)
10. Carregar jogo (substitui a partida atual, só depois de validar o arquivo)
11. Sobre o jogo
0. Sair

As opções 2 a 9 avisam quando ainda não há partida em andamento.

## Formato do arquivo de save

Texto simples, uma `chave=valor` por linha, com uma seção `[cidade]` para cada jogador:

```
BOLONHA_E_MILANO_SAVE v1
vez=1
[cidade]
jogador=Ana
titulo=1
nome=Bolonha
ano=1401
florins=900
terras=10000
graos=500
servos=200
soldados=4
construcoes=2
taxa=20
justica=Severa
colheita=1
impostos=0
[cidade]
...
```

`vez` é a posição (começando em 0) de quem joga agora; `titulo` é o nível (0 = Cavaleiro até 4 = Duque); `colheita` e `impostos` valem 1 se já foram feitos no ano.

## Commits

Mensagens no formato `Etapa N: descrição`. As melhorias extras entram num commit à parte (`Extras: ...`).
