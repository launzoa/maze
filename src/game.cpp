#include "game.hpp"
using namespace std;

/// @brief Desenha a tela de menu do jogo 
/// @param mousePos O vetor coordenada do ponto (x,y) do cursor do mouse
void drawMenuScreen(Vector2 mousePos) {
    DrawText("Maze Game!", 30, 30, 50, DARKGREEN); // Escreve o título do menu 
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
void drawGameScreen(Vector2 mousePos, GameState& state) {
    auto& maze = state.maze;

    // Função lambda para desenhar um tile 16x16 esticado para N x M (e.g., 20x20)
    auto drawTile = [](const Texture2D& tex, int x, int y) { 
        Rectangle source = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
        Rectangle dest = { (float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE };
        DrawTexturePro(tex, source, dest, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
    };

    // Percorre a matriz do mapa
    for (int row = 0; row < maze.size(); row++) {
        for (int col = 0; col < maze[row].size(); col++) {
            // Cria as posições para os tiles 20x20, fazendo uma projeção da matriz
            int posX = 20 + col * TILE_SIZE;
            int posY = 20 + row * TILE_SIZE;
            char tile = maze[row][col];

            // Se o tile for de chão/caminho (',')
            if (tile == ',') drawTile(state.groundTexture, posX, posY);
            // Se o tile for um grafos ('A'-'Z')
            else if (tile >= 'A' && tile <= 'Z') drawTile(state.grassFlowerTexture, posX, posY);
            else {// Se não, desenha as grama (para '.') 
                drawTile(state.grassTexture, posX, posY);
                // Se o tile for árvore ('#')
                if (tile == '#') drawTile(state.treeTexture, posX, posY);
            }
        }
    }

    // Barra lateral direta do jogo 
    DrawRectangle(850, 20, 400, 680, Fade(LIGHTGRAY, 0.3f));
    DrawRectangleLines(850, 20, 400, 680, DARKGRAY);

    // Botão de voltar para o Menu
    Color btnColor = CheckCollisionPointRec(mousePos, BTN_BACK_MENU) ? GRAY : LIGHTGRAY;
    DrawRectangleRec(BTN_BACK_MENU, btnColor);
    DrawRectangleLinesEx(BTN_BACK_MENU, 2, BLACK);
    DrawText("< Back to Menu", BTN_BACK_MENU.x + 35, BTN_BACK_MENU.y + 14, 24, BLACK);
}

/// @brief Atualiza 1 frame do jogo para webassembly 
void updateDrawFrame(void* arg) {
    GameState* state = static_cast<GameState*>(arg); // Força a criação um estado vazio (como ponteiro)

    Vector2 mousePos = GetMousePosition(); // Pega a posição do mouse

    if (state->currentScreen == Screen::MENU) {
        // Verifica se o botão do algoritmo de BFS foi clickado 
        if (CheckCollisionPointRec(mousePos, BTN_BFS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentScreen = Screen::GAME;
            vector<Vector2> bfsPath = bfs(&state->nodes['S'], 'Y');
            state->player.setPath(bfsPath);
        }
        // Verifica se o botão do algoritmo de DFS foi clickado
        else if (CheckCollisionPointRec(mousePos, BTN_DFS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentScreen = Screen::GAME;
            vector<Vector2> dfsPath = dfs(&state->nodes['S'], 'Y');
            state->player.setPath(dfsPath);
        }
        // Verifica se o botão do algoritmo de DLS foi clickado (com limite 6)
        else if (CheckCollisionPointRec(mousePos, BTN_DLS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentScreen = Screen::GAME;
            vector<Vector2> dlsPath = dls(&state->nodes['S'], 'Y', 6);
            state->player.setPath(dlsPath);
        }
        // Verifica se o botão do algoritmo de IDS foi clickado
        else if (CheckCollisionPointRec(mousePos, BTN_IDS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentScreen = Screen::GAME;
            vector<Vector2> idsPath = ids(&state->nodes['S'], 'Y');
            state->player.setPath(idsPath);
        }
        // Verifica se o botão do algoritmo de UCS foi clickado
        else if (CheckCollisionPointRec(mousePos, BTN_UCS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentScreen = Screen::GAME;
            vector<Vector2> ucsPath = ucs(&state->nodes['S'], 'Y');
            state->player.setPath(ucsPath);
        }
        // Verifica se o botão do algoritmo Greedy (Gulosa) foi clickado
        else if (CheckCollisionPointRec(mousePos, BTN_GREEDY) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentScreen = Screen::GAME;
            vector<Vector2> greedyPath = greedy(&state->nodes['S'], &state->nodes['Y']);
            state->player.setPath(greedyPath);
        }
        // Verifica se o botão do algoritmo A* foi clickado
        else if (CheckCollisionPointRec(mousePos, BTN_ASTAR) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentScreen = Screen::GAME;
            vector<Vector2> astarPath = astar(&state->nodes['S'], &state->nodes['Y']);
            state->player.setPath(astarPath);
        }
    }
    // Durante a tela de jogo
    else if (state->currentScreen == Screen::GAME) {
        state->player.update(GetFrameTime()); // Atualiza a posição a cada frame

        // Se clicou no botão de voltar para o menu
        if (CheckCollisionPointRec(mousePos, BTN_BACK_MENU) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentScreen = Screen::MENU; // Volta para o menu
            state->player.waypoints.clear(); // Limpa o trajeto antigo
        }
    }

    BeginDrawing(); // Começa a desenhar os elementos visuais da tela  
    ClearBackground(RAYWHITE); // Fundo branco (clássico do raylib)

    // Transição entre as telas
    switch(state->currentScreen) { 
        case Screen::MENU: // Se a tela atual for de menu, desenha a tela de menu
            drawMenuScreen(mousePos); 
        break;

        case Screen::GAME: // Caso contrário, a tela atual a ser desenhada é a do jogo
            drawGameScreen(mousePos, *state);
            state->player.drawPlayer(state->playerTexture);
        break;
    }

    EndDrawing(); // Fim do modo de desenho
}


/// @brief Carrega o arquivo .txt do mapa para um vetor de strings
/// @param filePath O caminho do arquivo .txt 
/// @return Retorna uma matriz de strings representando o mapa carregado na memória
vector<string> loadMaze(const string& filePath) {
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