#pragma once
#include "types.hpp"
#include "raylib.h"
#include <vector>
#include <iostream>
#include <fstream>

// Constantes para tela do jogo
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int TILE_SIZE = 20;

// Constantes para botões do jogo (Coluna 1: Buscas Cegas)
const Rectangle BTN_BFS = {50, 200, 550, 50};
const Rectangle BTN_DFS = {50, 280, 550, 50};
const Rectangle BTN_DLS = {50, 360, 550, 50};
const Rectangle BTN_IDS = {50, 440, 550, 50};

// Constantes para botões do jogo (Coluna 2: Buscas com Custo e Heurística)
const Rectangle BTN_UCS = {650, 200, 550, 50};
const Rectangle BTN_GREEDY = {650, 280, 550, 50};
const Rectangle BTN_ASTAR = {650, 360, 550, 50};

// Funções de desenho do jogo 
void drawMenuScreen(const Vector2 mousePos);
void drawGameScreen(const Vector2 mousePos, std::vector<std::string>& maze);

// Função para carregar o mapa (transcrito para um arquivo de texto)
std::vector<std::string> loadForestMaze(const std::string& filePath);
