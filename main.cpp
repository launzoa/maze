#include "raylib.h"
#include "types.hpp"
#include "game.hpp"
#include "algorithms.hpp"
using namespace std;


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Forest"); // Inicializando a janela 
    SetTargetFPS(60); // Setando o FPS

    vector<string> forestMaze = loadForestMaze("assets/viridian-forest.txt"); // Carregando o mapa
    Texture2D playerTexture = LoadTexture("assets/red.gif"); // Carregando a texture do jogador

    GameScreen currentScreen = GameScreen::MENU; // Inicializando a tela no menu
    Player player; // Inicializando o jogador

    unordered_map<char, Node> nodes = nodesFromMaze(forestMaze);
    connectGraph(nodes);

    while (!WindowShouldClose()) { // Rodando os aspectos gráficos do jogo

        Vector2 mousePos = GetMousePosition(); // Pega a posição do mouse
 
        // Verifica se o botão do algoritmo de BFS foi clickado 
        if (CheckCollisionPointRec(mousePos, BTN_BFS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentScreen = GameScreen::GAME;
            vector<Vector2> bfsPath = bfs(&nodes['S'], 'Y');
            player.setPath(bfsPath);
        }

        // Verifica se o botão do algoritmo de DFS foi clickado
        if (CheckCollisionPointRec(mousePos, BTN_DFS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentScreen = GameScreen::GAME;
            vector<Vector2> dfsPath = dfs(&nodes['S'], 'Y');
            player.setPath(dfsPath);
        }

        // Verifica se o botão do algoritmo de DLS foi clickado (com limite 6)
        if (CheckCollisionPointRec(mousePos, BTN_DLS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentScreen = GameScreen::GAME;
            vector<Vector2> dlsPath = dls(&nodes['S'], 'Y', 6);
            player.setPath(dlsPath);
        }

        // Verifica se o botão do algoritmo de IDS foi clickado
        if (CheckCollisionPointRec(mousePos, BTN_IDS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentScreen = GameScreen::GAME;
            vector<Vector2> idsPath = ids(&nodes['S'], 'Y');
            player.setPath(idsPath);
        }

        // Verifica se o botão do algoritmo de UCS foi clickado
        if (CheckCollisionPointRec(mousePos, BTN_UCS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentScreen = GameScreen::GAME;
            vector<Vector2> ucsPath = ucs(&nodes['S'], 'Y');
            player.setPath(ucsPath);
        }

        // Verifica se o botão do algoritmo Greedy (Gulosa) foi clickado
        if (CheckCollisionPointRec(mousePos, BTN_GREEDY) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentScreen = GameScreen::GAME;
            vector<Vector2> greedyPath = greedy(&nodes['S'], &nodes['Y']);
            player.setPath(greedyPath);
        }

        // Verifica se o botão do algoritmo A* foi clickado
        if (CheckCollisionPointRec(mousePos, BTN_ASTAR) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentScreen = GameScreen::GAME;
            vector<Vector2> astarPath = astar(&nodes['S'], &nodes['Y']);
            player.setPath(astarPath);
        }

        // Durante o jogo, atualiza as novas posições do jogador constantemente 
        if (currentScreen == GameScreen::GAME) player.update(GetFrameTime());

        BeginDrawing(); // Começa a desenhar os elementos visuais da tela  
            ClearBackground(RAYWHITE); // Fundo branco (clássico do raylib)

            // Transição entre as telas
            switch(currentScreen) { 
                case GameScreen::MENU: // Se a tela atual for de menu, desenha a tela de menu
                    drawMenuScreen(mousePos); 

                    break;

                case GameScreen::GAME: // Caso contrário, a tela atual a ser desenhada é a do jogo
                    drawGameScreen(mousePos, forestMaze);
                    player.drawPlayer(playerTexture);

                    break;
            }

        EndDrawing(); // Fim do modo de desenho
    }

    UnloadTexture(playerTexture); // Descarrega as texturas
    CloseWindow(); // Fecha a janela

    return 0;
}