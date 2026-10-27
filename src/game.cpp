#include "game.hpp"
using namespace std;

/// @brief Desenha a tela de menu do jogo 
/// @param mousePos O vetor coordenada do ponto (x,y) do cursor do mouse
void drawMenuScreen(Vector2 mousePos) {
    DrawText("Viridian Forest Maze!", 30, 30, 50, DARKGREEN); // Escreve o título do menu 
    DrawText("Choose your graph algorithm to play:", 30, 100, 35, BLACK); // Escreve o sub-título do menu

    auto drawBtn = [&](Rectangle btn, const char* label) {
        Color c = CheckCollisionPointRec(mousePos, btn) ? GRAY : LIGHTGRAY;
        DrawRectangleRec(btn, c);
        DrawRectangleLinesEx(btn, 2, BLACK);
        DrawText(label, btn.x + 20, btn.y + 12, 24, BLACK);
    };

    // Coluna 1: Buscas Cegas (Uninformed Search)
    drawBtn(BTN_BFS, "Breadth First Search (BFS)");
    drawBtn(BTN_DFS, "Depth First Search (DFS)");
    drawBtn(BTN_DLS, "Depth Limited Search (DLS - Lim 6)");
    drawBtn(BTN_IDS, "Iterative Deepening Search (IDS)");

    // Coluna 2: Buscas Informadas e de Custo (Informed / Cost Search)
    drawBtn(BTN_UCS, "Uniform Cost Search (UCS)");
    drawBtn(BTN_GREEDY, "Greedy Best-First Search");
    drawBtn(BTN_ASTAR, "A* Search (A-Star)");
}


/// @brief Desenha a tela do jogo
/// @param mousePos O vetor coordenada do ponto (x,y) do cursor do mouse
/// @param maze O vetor de strings ('.', '#', etc.) usado para modelar o mapa do jogo 
void drawGameScreen(Vector2 mousePos, vector<string>& maze) {
    // Percorre por cada string do mapa
    for (int row = 0; row < maze.size(); row++) {
        for (int col = 0; col < maze[row].size(); col++) {
            // Cada string do mapa é representada por um bloco de 20x20 (tamanho do TILE_SIZE), com um offset de 20 para os eixos x, y
            int posX = 20 + col * TILE_SIZE;
            int posY = 20 + row * TILE_SIZE;

            // Mapeia e desenha para cada símbolo, seu significado no mapa  
            if (maze[row][col] == '#') DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, DARKGREEN); // "#" é árvore
            else if (maze[row][col] == 'w') DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, LIME); // "w" é grama
            else {
                DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, BEIGE); // "." é chão, no entanto, o grafo também possui um chão sobre ele 
                if (maze[row][col] >= 'A' && maze[row][col] <= 'Z') DrawCircle(posX + 10, posY + 10, 8, BLACK); // "A-Z" é desenhado como o grafo
            }
        }
    }
}

/// @brief Carrega o arquivo .txt do mapa para um vetor de strings
/// @param filePath O caminho do arquivo .txt 
/// @return Retorna uma matriz de strings representando o mapa carregado na memória
vector<string> loadForestMaze(const string& filePath) {
    vector<string> maze; // O vetor do mapa do jogo 
    ifstream file(filePath); // Caminho para o arquivo do mapa 

    if (!file.is_open()) { // Se abrir o arquivo não tiver sido possível
        cerr << "[ERROR] not possible open the file: " << filePath << "\n";
        return maze;
    }

    string line; // Caso o arquivo tenha sido aberto corretamente
    while (getline(file, line)) { // Percorre por cada linha do .txt
        if (!line.empty()) maze.push_back(line); // Adicionando o conteúdo das linhas não vazias no vetor do mapa 
    }

    file.close(); // Fecha o arquivo
    return maze; // Retorna o vetor do mapa
}