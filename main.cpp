#include "raylib.h"
#include "types.hpp"
#include "game.hpp"
#include "algorithms.hpp"
using namespace std;

#ifdef __EMSCRIPTEN__
    #include <emscripten/emscripten.h>
#endif

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze"); // Inicializando a janela 

    GameState state;
    state.maze = loadMaze("assets/maze.txt"); // Carregando o mapa

    // Carregando as texturas para o jogo
    state.playerTexture = LoadTexture("assets/duck.png"); 
    state.groundTexture = LoadTexture("assets/ground.png"); 
    state.grassTexture = LoadTexture("assets/grass.png");
    state.grassFlowerTexture = LoadTexture("assets/grassFlower.png");
    state.treeTexture = LoadTexture("assets/tree.png");
    
    state.nodes = nodesFromMaze(state.maze); // Inicializando os grafos 
    connectGraph(state.nodes); // Conecta os grafos

#ifdef __EMSCRIPTEN__ // Versão WebAssembly
    emscripten_set_main_loop_arg(updateDrawFrame, &state, 0, 1);
#else  // Versão local do jogo (Linux)
    SetTargetFPS(60); // Setando o FPS

    while (!WindowShouldClose()) { // Enquanto a janela não for fechada
        updateDrawFrame(&state); // Atualiza o jogo
    }
#endif
    // Descarrega as texturas
    UnloadTexture(state.playerTexture); 
    UnloadTexture(state.groundTexture);
    UnloadTexture(state.grassTexture);
    UnloadTexture(state.grassFlowerTexture);
    UnloadTexture(state.treeTexture);

    CloseWindow(); // Fecha a janela

    return 0;
}