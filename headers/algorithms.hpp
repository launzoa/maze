#pragma once 
#include "raylib.h"
#include "types.hpp"
#include "game.hpp"
#include <vector>
#include <iostream>
#include <unordered_map>


// Função para localizar os nós na matriz 
std::unordered_map<char, Node> nodesFromMaze(const std::vector<std::string>& maze); 
// Função para conectar os grafos do mapa em um grafo 
void connectGraph(std::unordered_map<char, Node>& nodes);
// Função para transformar a localização do nó em uma coordenada (x,y)
Vector2 nodeToCoord(Node* node);
// Função para encontrar o caminho bifurcado entre dois nós
void pathBetween(Node* from, Node* to, std::unordered_map<Node*, Node*>& parent, std::vector<Vector2>& path);
// Função para verificar se um nó não está em alguma lista 
bool notVisited(char name, const std::vector<Node*>& list);

// Funções de busca em grafos
std::vector<Vector2> bfs(Node* start, char goal);
std::vector<Vector2> dfs(Node* start, char goal);
std::vector<Vector2> dls(Node* start, char goal, int limit, bool* found = nullptr);
std::vector<Vector2> ids(Node* start, char goal);

// Funções de custo e heurística
float distance(Node* a, Node* b);
float heuristic(Node* a, Node* goal);

// Buscas informadas e com custo
std::vector<Vector2> ucs(Node* start, char goal);
std::vector<Vector2> greedy(Node* start, Node* goal);
std::vector<Vector2> astar(Node* start, Node* goal);