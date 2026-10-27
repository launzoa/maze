#pragma once 
#include "raylib.h"
#include <vector>
#include <string>
#include <unordered_map>

// Enum para representar cada tela do jogo, i.e., a tela de menu ou do jogo
enum class Screen {
    MENU = 0,
    GAME = 1
};

// Esturutra que representa um nó no grafo
struct Node {
    char name;
    int row;
    int col;
    std::vector<Node*> neighbors; 
};

// Estrutura para representar o jogador
struct Player {
    Vector2 pos; // A coordeanada (x, y) da sua posição atual
    float speed = 140.0f; // A velocidade com que ele se move por segundo
    std::vector<Vector2> waypoints; // O caminho de um determinado algoritmo de busca 
    int index = 0; // O indíce para percorrer os nós dos do caminho


    /// @brief Informa o jogador do caminho percorrido pelo algoritmo de busca  
    /// @param path O caminho encontrado pelo algoritmo de busca
    void setPath(const std::vector<Vector2>& path) {
        waypoints = path; // Define o caminho do algoritmo 

        if (!waypoints.empty()) { // E inicializa o jogador no caminho
            pos = waypoints[0];
            index = 1;
        }
    }


    /// @brief Atualiza o movimento do jogador de um grafo para outro 
    /// @param deltaTime O tempo atual repassado do jogo em relação ao FPS
    void update(float deltaTime) {
        // Se não há mais nós para percorrer, acaba o caminho 
        if (waypoints.empty() || index >= waypoints.size()) return;

        Vector2 target = waypoints[index]; // Pega o próximo nó a ser percorrido

        // Se o deslocamento for para a direita 
        if (pos.x < target.x) { 
            pos.x += speed * deltaTime;

            if (pos.x >= target.x) pos.x = target.x;
        }
        // Se o deslocamento for para a esquerda
        else if (pos.x > target.x) {
            pos.x -= speed * deltaTime; 

            if (pos.x <= target.x) pos.x = target.x;
        }
        // Se o deslocamento for para cima
        if (pos.y < target.y) {
            pos.y += speed * deltaTime;

            if (pos.y >= target.y) pos.y = target.y;
        }
        // Se o deslocamento for para baixo
        else if (pos.y > target.y) {
            pos.y -= speed * deltaTime; 

            if (pos.y <= target.y) pos.y = target.y;
        }

        // Após o jogador ter sido deslocado, passa o índice para o próximo nó
        if (pos.x == target.x && pos.y == target.y) index++;
    }


    /// @brief Desenha o jogador em sua posição (x,y)
    void drawPlayer(const Texture2D& playerTexture) {
        // Desenha a textura do jogador com base nos valores iniciais e nos valores transformados 
        Rectangle source = {0, 0, (float)playerTexture.width, (float)playerTexture.height}; 
        Rectangle dest = {pos.x - 20, pos.y - 25, 40, 40};
        DrawTexturePro(playerTexture, source, dest, Vector2{0, 0}, 0.0f, WHITE);
    }
};

// Esturutra para representar o estado do jogo (para emscripten, webassembly)
struct GameState {
    std::vector<std::string> maze;
    Texture2D playerTexture;

    Texture2D groundTexture;
    Texture2D grassTexture;
    Texture2D grassFlowerTexture;
    Texture2D treeTexture;

    Screen currentScreen = Screen::MENU;
    Player player;
    std::unordered_map<char, Node> nodes;
};