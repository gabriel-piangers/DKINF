# DKINF

Trabalho final da disciplina INF01202 – Algoritmos e Programação.

<hr>

## Descrição do projeto
O projeto implementa um jogo inspirado no clássico Donkey Kong usando a linguagem C e a biblioteca gráfica Raylib. O objetivo foi desenvolver um jogo funcional com carregamento dinâmico de mapas e sistema de ranking, além da implementação da lógica principal do jogo.

## Estrutura do projeto
- `assets/`: imagens e texturas usadas no jogo.
- `maps/`: arquivos texto dos níveis.
- `src/`: código-fonte em C.
- `src/headers/`: cabeçalhos com constantes, estruturas e protótipos.
- `compile.sh`: script de compilação para Linux.
- `bin/placar.bin`: arquivo binário do ranking (gerado em execução).

### Representação dos elementos no mapa
- **Mapa**: carregado de arquivos texto em `maps/mapaX.txt`, onde cada caractere representa um tile ou entidade.
  - `Z`: piso/parede.
  - `H`, `S` `D`: escada.
  - `F`: fim de fase (baú).
  - `P`: posição inicial do jogador.
  - `E`: inimigo do tipo fantasma.
  - `W`: inimigo do tipo fogo.

### Interação dos componentes
- `main.c` controla o fluxo do jogo: inicializa Raylib, carrega recursos, processa estados, atualiza o jogo e desenha a tela.
- `map.c` é responsável pela inicialização e por verificações no mapa.
- `player.c` trata entradas do usuário, aplica física e movimentação ao jogador (corrida, pulo e dash) e desenha as animações do personagem.
- `enemies.c` implementa inimigos com comportamentos diferentes:
  - fantasmas: se movem horizontalmente e invertem direção ao encontrar parede ou despenhadeiro;
  - pedras: caem pela tela e são removidas ao sair da área visível.
  - fogo: não faz nada mas é perigoso.
- `ranking.c` calcula o índice de posicionamento com base no tempo e salva os melhores resultados em um arquivo binário.
- `ui.c` implementa as funções que desenham a interface gráfica do jogo.

### Estruturas e funções principais
<details open>
<summary> Player </summary>

```C
typedef struct {
    Vector2 position;
    Vector2 velocity;
    PlayerState state;
    Texture2D texture;
    int framePos; // intervalo de [0, PLAYER_ANIMATION_FRAMES]
    int direction; // 1 -> facing right, -1 -> facing left
    float dashTimer;
    int lifes;
} Player;
```
</details>

<details open>
<summary> Enemy </summary>

```C
typedef struct {
    Vector2 position;
    EnemyType type;
    int direction; // 1 -> moving right, -1 -> moving left, 0 -> no movement
    float rotation;
    Texture2D texture;
    bool isAlive;
} Enemy;
```
</details>

<details open>
<summary> RankScore </summary>

```C
typedef struct {
    char name[MAX_PLAYER_NAME];
    float time;
} RankScore;
```
</details>

- Funções principais:
  - `LoadResources()`, `UnloadResources()` em `src/main.c`.
  - `LoadMap(int level)`, `DrawMap()`, `CheckCollisionWithTile(Rectangle rect, char tile)` em `src/map.c`.
  - `UpdatePlayer()`, `DrawPlayer()`, `GetPlayerRect()`, `PlayerReachedGoal()` em `src/player.c`.
  - `UpdateEnemies()`, `DrawEnemies()`, `SpawnBoulder()` em `src/enemies.c`.
  - `LoadRanking()`, `SaveScore(RankScore newScore, int index)` em `src/ranking.c`.
  - `DrawMenu()`, `DrawInput()`, `DrawRanking()` em `src/ui.c`.

## Como usar
### Linux
1. Instale a Raylib (se necessário):
   - `git clone https://github.com/raysan5/raylib.git`
   - `cd raylib/src`
   - `make PLATFORM=PLATFORM_DESKTOP`
   - `sudo make install`
   - `sudo ldconfig`
2. Na pasta do projeto, execute:
   - `sh compile.sh`
3. Inicie o jogo com:
   - `./game`

### Windows + VSCode
1. Instale Raylib e ajuste o caminho do compilador no VSCode, se necessário.
2. Abra o projeto no VSCode.
3. Pressione F5 para compilar e executar.

## Jogabilidade
- Controle o jogador com as teclas de movimento (`W`, `A`, `S`, `D`).
- Pule com `SPACE` e utilize o dash com `SHIFT`
- Alcance o objetivo `F` evitando inimigos e quedas.
- Ao completar o nível, insira seu nome para gravar o tempo no ranking.
- O ranking exibe os 10 melhores tempos em ordem crescente.

