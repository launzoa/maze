#pragma once
#include "types.hpp"
#include "algorithms.hpp"
#include "raylib.h"
#include <vector>
#include <iostream>
#include <fstream>

// Constantes para tela do jogo
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int TILE_SIZE = 20;

// Retângulos para os botões do jogo (primeira coluna)
const Rectangle BTN_BFS = {50, 200, 550, 50};
const Rectangle BTN_DFS = {50, 280, 550, 50};
const Rectangle BTN_DLS = {50, 360, 550, 50};
const Rectangle BTN_IDS = {50, 440, 550, 50};

// Retângulos para os botões do jogo (segunda coluna)
const Rectangle BTN_UCS = {650, 200, 550, 50};
const Rectangle BTN_GREEDY = {650, 280, 550, 50};
const Rectangle BTN_ASTAR = {650, 360, 550, 50};

// Botão para retornar ao menu durante o jogo
const Rectangle BTN_BACK_MENU = { 880, 40, 350, 50 };

// Funções de desenho do jogo 
void drawMenuScreen(const Vector2 mousePos);
void drawGameScreen(const Vector2 mousePos, GameState& state);
void updateDrawFrame(void* arg);

// Função para carregar o mapa (transcrito para um arquivo de texto)
std::vector<std::string> loadMaze(const std::string& filePath);
