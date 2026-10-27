#include "algorithms.hpp"
#include <cmath>
using namespace std;



/// @brief Extrai os nós do mapa (já transformado em uma matriz) para uma lista de nós contendo as respectivas
///     Coordenadas (linha, coluna) desses nós
/// @param maze O mapa do jogo, representado como uma matriz de strings 
/// @return Retorna a lista de grafos extraída do mapa do jogo
unordered_map<char, Node> nodesFromMaze(const vector<string>& maze) {
    unordered_map<char, Node> graph; // Lista de nós ordenadas por char

    // Percorre por todas as linhas e colunas da matriz 
    for (int row = 0; row < maze.size(); row++) {
        for (int col = 0; col < maze[row].size(); col++) {
            char ch = maze[row][col]; // Pega o caractere da linha e coluna atual
            // Se ele for um nó, i.e., uma letra, então adiciona esse nó com as suas respectivas coordenadas
            if (ch >= 'A' && ch <= 'Z') graph[ch] =  Node {ch, row, col, {}}; // na lista de nós
        }
    }

    return graph; // Retorna a lista de nós do mapa 
}

/// @brief Conecta os nós do grafo entre si 
/// @param nodes Lista contendo os nós do grafo 
void connectGraph(unordered_map<char, Node>& nodes) {
    // Função lambda para conectar dois nós com uma aresta
    auto addEdge = [&](char u, char v) {
        nodes[u].neighbors.push_back(&nodes[v]);
        nodes[v].neighbors.push_back(&nodes[u]);
    };  

    // Tradução das conexões do mapa em ligações de nós (arestas)
    addEdge('S', 'C'); addEdge('S', 'A');
    addEdge('A', 'E'); addEdge('A', 'B');
    addEdge('C', 'I'); addEdge('C', 'D');
    addEdge('E', 'F'); addEdge('E', 'G');
    addEdge('G', 'H');
    addEdge('I', 'J'); addEdge('I', 'L');
    addEdge('J', 'K'); addEdge('K', 'L'); 
    addEdge('L', 'M');
    addEdge('M', 'N'); addEdge('M', 'Q');
    addEdge('N', 'O');
    addEdge('O', 'P'); 
    addEdge('P', 'Q'); 
    addEdge('Q', 'R');
    addEdge('R', 'Z'); 
    addEdge('Z', 'T');
    addEdge('T', 'U');
    addEdge('U', 'V');
    addEdge('V', 'W'); addEdge('V', 'X');
    addEdge('X', 'Y');     
}

/// @brief Transforma a linha e coluna do nó em uma coordenada de pixels (x,y) ajustada   
/// @param node O nó em específico a ser convertido em coordenadas (x,y)
/// @return Retorna o vetor coordenada (x,y)
Vector2 nodeToCoord(Node* node) {
    return {30.0f + node->col * TILE_SIZE, 30.0f + node->row * TILE_SIZE};
}


/// @brief Algoritmo de menor ancestral comum para encontrar a bifurcação (ponto de encontro)
///     Entre dois nós — sem isso, o player tava cruzando o mapa pra lá e pra cá 
/// @param from O nó de origem
/// @param to O nó de destino
/// @param parent Uma lista de pares de nós: <pai, filho> 
/// @param path O caminho entre os dois nós (origem e destino)
void pathBetween(Node* from, Node* to, unordered_map<Node*, Node*>& parent, vector<Vector2>& path) {
    // Percorre e adiciona todos os nós-ancestrais a partir do nó de origem (from) até a raiz
    vector<Node*> fromAncestors;
    for (Node* current = from; current != nullptr; current = parent[current]) fromAncestors.push_back(current);

    // Percorre e adiciona todos os nós-ancetrais a partir do nó de destino (to) até a raiz
    vector<Node*> toAncestors;
    for (Node* current = to; current != nullptr; current = parent[current]) toAncestors.push_back(current); 

    // Encontra a bifurcação (menor ancestral em comum) entre o nó de origem e o nó de destino 
    Node* lca = nullptr;
    for (Node* fromA : fromAncestors) {
        for (Node* toA : toAncestors) {
            if (fromA == toA) { // Quando encontra o nó em comum, sai do primeiro loop
                lca = fromA;
                break; 
            }
        }
        // Se tiver encontrado o nó em comum, sai do segundo loop  
        if (lca != nullptr) break;
    }

    // Retrocede do nó de origem (from) até a nó da bifurcação (lca)
    for (Node* current = from; current != lca; current = parent[current]) {
        path.push_back(nodeToCoord(parent[current]));
    }

    // Retrocede, encontrando o camino do nó de destino (to) até o nó da bifurcação (lca)
    vector<Node*> forwardPath;
    for (Node* current = to; current != lca; current = parent[current]) {
        forwardPath.push_back(current);
    }
    // Então, avança do nó de bifurcação (lca) até o nó de destino (to) 
    for (int i = forwardPath.size() - 1; i >= 0; i--) {
        path.push_back(nodeToCoord(forwardPath[i]));
    }
}



/// @brief Verifica se um nó está presente em uma lista 
/// @param list Lista de nós 
/// @return Caso o nó procurado esteja presente na lista, retorna falso. Caso contrário, retorna verdadeiro
bool notVisited(char name, const vector<Node*>& list) {
    // Percorre por todos os nós da lista
    for (Node* node : list) {
        if (name == node->name) return false; // Se o elemento procurado estiver na lista, retorna falso  
    }
    // Se percorrer toda a lista e não encontrar o elemento, então retorna verdadeiro 
    return true;
}


/// @brief Algoritmo de busca em largura 
/// @param start Nó de começo/início do grafo
/// @param goal Valor objetivo (meta) a ser buscado no grafo
/// @return Retorna uma lista de coordenadas para o player percorrer
vector<Vector2> bfs(Node* start, char goal){
    vector<Node*> open; // Fila representando os nós em aberto, i.e., a serem ou sendo explorados
    vector<Node*> closed; // Lista de nós fechados, i.e., nós já explorados
    open.push_back(start); // Adiciona o nó de início na fila de exploração

    vector<Vector2> path; // Lista contendo o caminho da busca, i.e., todas as coordenadas (x,y) dos nós buscados
    unordered_map<Node*, Node*> parent; // Lista ordenada de dois nós: pai e filho, para o backtracking visual 
    parent[start] = nullptr; // A raiz (nó de início) não tem pais
    Node* lastNode = start; // Mantém o último nó explorado 
    path.push_back(nodeToCoord(start)); // Adiciona a coordenada do nó de começo na lista do caminho 

    while (!open.empty()) { // Enquanto ainda há nós a serem explorados
        Node* current = open.front();  // Pega o primeiro nó da fila
        open.erase(open.begin()); // E remove ele da lista de exploração 
        closed.push_back(current); // Adicionada ele na lista de explorados
        // Faz o player andar fisicamente (retrocedendo se precisar) até o nó atual e adicionando a sua coordenada
        pathBetween(lastNode, current, parent, path); // Na lista de caminho 
        lastNode = current; // Atualiza o último nó

        // Verifica se encontrou o objetivo
        if (current->name == goal) break; // Se sim, saí da busca 

        // Se não, percorre por todos os nós vizinhos do grafo atual
        for (Node* neighbor : current->neighbors) { 
            // Adicionando os nós ainda não visitados na lista de explorados
            if (notVisited(neighbor->name, open) && notVisited(neighbor->name, closed)) {
                parent[neighbor] = current; // Define o nó atual como o pai do vizinho encontrado 
                open.push_back(neighbor); // E adiciono o vizinho na lista de exploração
            }
        }
    }

    return path; // Retorna o caminho dos nós
}


/// @brief Algoritmo de busca em profundidade 
/// @param start O nó de começo (início) do grafo
/// @param goal O valor objetivo (meta) da busca no grafo
/// @return Retorna um vetor de coordenadas (x, y) contendo o caminho da busca do algoritmo
vector<Vector2> dfs(Node* start, char goal) {
    vector<Node*> open; // Pilha de nós a serem explorados  
    vector<Node*> closed; // Lista contendo 
    open.push_back(start); // Adiciona o elemento de começo do grafo para ser explorado  
    vector<Vector2> path; // Lista de coordenadas para cada nó do mapa

    while (!open.empty()) { // Enquanto ainda tiver nós a serem explorados
        Node* current = open.back(); // Pega o último nó da pilha e o define como o atual
        path.push_back(nodeToCoord(current)); // Adiciona a coordenada desse nó na lista de coordenadas (caminho)
        
        if (current->name == goal) return path; // Se o objetivo for encontrado, finaliza o caminho

        for (Node* neighbor : current->neighbors) { // Percorre por todos os nós vizinhos do nó atual
            // Se esse nó ainda não tiver sido explorado 
            if (notVisited(neighbor->name, open) && notVisited(neighbor->name, closed)) { 
                open.push_back(current); // Adiciona novamente o nó atual lista de aberto (obs: isso é só por
                // Uma questão visual, para que o player volte na casa e não saia cruzando o mapa desgovernado)
                open.push_back(neighbor); // Adiciona o vizinho na lista de nós a serem explorados
            }
        }       

        // O ideal seria remover da lista de abertos no começo, mas isso quebraria a lógica visual do negócio.
        // Faz mais sentido apenas remover quando ele for ou se tornar uma folha, permitindo o backtracking visual
        // Do player enquanto cruza o labirinto 
        if (current->name == open.back()->name) {  // Se o nó não tiver mais nenhum vizinho novo a ser explorado
            closed.push_back(current); // Coloca ele na lista de já explorados
            open.pop_back(); // E remove ele da lista de exploração
        }
    }

    return path; // Retorno caso todos os nós acessíveis sejam explorados
}

/// @brief Algoritmo de busca em profundidade com limite
/// @param start Nó de começo da busca
/// @param goal O objetivo-meta a ser pesquisada
/// @param limit Limite de profundidade 
/// @param found Ponteiro opcional para sinalizar se a meta foi encontrada
/// @return Retorna o vetor de grafos percorrido pelo algoritmo de busca
vector<Vector2> dls(Node* start, char goal, int limit, bool* found) {
    // A premissa é praticamente a mesma do que fizemos em uma DFS, com a adição de um fator limite para os filhos
    vector<Node*> open; // Pilha de nós a serem exploradas
    vector<Node*> closed; // Lista de nós exploradas
    open.push_back(start); // Adiciona o nó inicial no grafo
    vector<Vector2> path; // Lista de coordenadas para o caminho percorrido pela busca do algoritmo
    unordered_map<Node*, int> depth; // Mapa (dicionário) contendo as profundidade dos nós
    depth[start] = 0;

    while (!open.empty()) { // Enquanto ainda tiver nós a serem explorados
        Node* current = open.back(); // Pega o último nó adicionado
        path.push_back(nodeToCoord(current)); // Adiciona a sua coordenada na lista de caminho
        if (current->name == goal) {
            if (found) *found = true; // Sinaliza que o objetivo foi encontrado
            return path; // Se o objetivo for encontrado, retorna o caminho de busca do algoritmo 
        }

        if (depth[current] < limit) { // Enquanto a busca não tiver atingido o limite de profundidade, adiciona os nós vizinhos para percorrer
            for (Node* neighbor : current->neighbors) { // Percorre por todos os nós vizinhos do nó atual
                if (notVisited(neighbor->name, open) && notVisited(neighbor->name, closed)) { // Se esse nó ainda não tiver sido explorado 
                    depth[neighbor] = depth[current] + 1; // Adiciona no dicionário a profundidade o nonovo nó como a de seu pai + 1
                    open.push_back(current); // Adiciona novamente o nó atual lista de aberto (obs: isso é só por
                    // Uma questão visual, para que o player volte na casa e não saia cruzando o mapa desgovernado)
                    open.push_back(neighbor); // Adiciona o vizinho na lista de nós a serem explorados
                }
            }    
        }

        if (current->name == open.back()->name) { // Se o nó não tiver mais nenhum vizinho novo adicionado por ele (folha)
            closed.push_back(current); // Coloca na lista de já explorados
            open.pop_back(); // Remove da pilha de nós em exploração 
        }
    }

    return path; // Se o limite for atingido e não encontrar um final, retorno o caminho percorrido até o momento
}

/// @brief Algoritmo de busca em profundidade iterativa
/// @param start O nó de começo do grafo
/// @param goal O objetivo (meta) a ser buscado no grafo
/// @return Um vetor de coordenadas representando o caminho de busca do algoritmo 
vector<Vector2> ids(Node* start, char goal) {
    // É uma premissa parecida com o DLS, com a diferença que o limite é incremental 
    int limit = 0; // Limite de profundidade da iteração
    vector<Vector2> fullPath; // O conjunto dos caminhos para as diversas iterações do DLS 
    bool found = false;

    while (limit < 50 && !found) { // Faz uma DLS com limit incremental até encontrar a meta
        vector<Vector2> path = dls(start, goal, limit, &found); // Executa o DLS com o limite atual da iteração
        fullPath.insert(fullPath.end(), path.begin(), path.end()); // Acumula os caminhos (para visualização gráfica)       
        if (found) break; // Encerra assim que o objetivo for alcançado
        limit++; // Incrementa o limite
    }

    return fullPath;  // Retorna o vetor de coordenadas acumuladas das diversas buscas iterativas
}


/// @brief Calcula a distância euclidiana em tiles entre dois nós
/// @param a Nó de origem
/// @param b Nó de destino
/// @return Distância euclidiana em tiles
float distance(Node* a, Node* b) {
    float dRow = (float)(a->row - b->row);
    float dCol = (float)(a->col - b->col);

    return sqrt(dRow * dRow + dCol * dCol);
}

/// @brief Função heurística baseada na distância em linha reta até o nó objetivo
/// @param a Nó atual
/// @param goal Nó objetivo
/// @return Estimativa em linha reta até o objetivo
float heuristic(Node* a, Node* goal) {
    return distance(a, goal);
}

/// @brief Busca de Custo Uniforme (Dijkstra)
/// @param start Nó inicial da busca
/// @param goal Identificador da meta (char)
/// @return Vetor de coordenadas para movimentação do jogador
vector<Vector2> ucs(Node* start, char goal) {
    unordered_map<Node*, Node*> parent; // Mapa (dicionário) para marcar o parentesco
    parent[start] = nullptr; // Nó de começo não tem pai

    unordered_map<Node*, float> costSoFar; // Dicionário para armazenar o custo até agora 
    costSoFar[start] = 0.0f; // Custo do de começo é nulo

    vector<Node*> open; // Lista de nós em aberto (a serem explorados)
    open.push_back(start); // Adiciona o nó de começo a ser explorado
    vector<Node*> closed; // Lista de nós em fechado (já explorados)
    vector<Vector2> path; // Lista de coordenadas para o caminho

    Node* lastNode = start;
    path.push_back(nodeToCoord(start));

    while (!open.empty()) { // Enquanto ainda há nós a serem explorados
        int bestIndex = 0; // Variável para armazenar o índice
        for (int i = 1; i < open.size(); i++) {  
            // Encontra o nó com menor custo acumulado g(n) na lista de abertos
            if (costSoFar[open[i]] < costSoFar[open[bestIndex]]) {
                bestIndex = i; // MAntém o seu índice
            }
        }

        Node* current = open[bestIndex]; // Pega o nó atual como o melhor nó a ser explroado em aberto
        open.erase(open.begin() + bestIndex); // Elimina ele da lista em aberto
        closed.push_back(current); // Adiciona ele na lista em fechado

        // O movimento visual com backtracking pelo algoritmo de menor ancestral comum
        pathBetween(lastNode, current, parent, path);
        lastNode = current;

        if (current->name == goal) break; // Se o objetivo for encontrado

        for (Node* neighbor : current->neighbors) { // Percorre pelos vizinhos do nó
            float newCost = costSoFar[current] + distance(current, neighbor); // Calcula o custo do nó, i.e., f = g + h

            if (notVisited(neighbor->name, closed)) { // Adiciona os vizinhos ainda não visitados
                if (notVisited(neighbor->name, open) || newCost < costSoFar[neighbor]) { // E/ou com custo inferior
                    costSoFar[neighbor] = newCost; // Calcula o novo custo
                    parent[neighbor] = current;  // Define o seu novo (ou não) pai

                    if (notVisited(neighbor->name, open)) {  // Se o nó não tiver sido visitado (para o caso de entrar por custo)
                        open.push_back(neighbor); // Adiciona na lista de a serem explorados
                    }
                }
            }
        }
    }

    return path; // Retorna o caminho da busca
}

/// @brief Busca Gulosa (Greedy Best-First Search)
/// Prioriza o nó com menor estimativa heurística h(n) até o objetivo
/// @param start Nó inicial da busca
/// @param goal Nó objetivo
/// @return Vetor de coordenadas para movimentação do jogador
vector<Vector2> greedy(Node* start, Node* goal) {
    unordered_map<Node*, Node*> parent;
    parent[start] = nullptr;

    vector<Node*> open = {start};
    vector<Node*> closed;
    vector<Vector2> path;

    Node* lastNode = start;
    path.push_back(nodeToCoord(start));

    while (!open.empty()) {
        // Encontra o nó com menor h(n) até a meta na lista de abertos
        int bestIndex = 0;
        for (int i = 1; i < (int)open.size(); i++) {
            if (heuristic(open[i], goal) < heuristic(open[bestIndex], goal)) {
                bestIndex = i;
            }
        }
        Node* current = open[bestIndex];
        open.erase(open.begin() + bestIndex);
        closed.push_back(current);

        // Movimento visual com backtracking suave via menor ancestral comum
        pathBetween(lastNode, current, parent, path);
        lastNode = current;

        if (current->name == goal->name) break;

        for (Node* neighbor : current->neighbors) {
            if (notVisited(neighbor->name, open) && notVisited(neighbor->name, closed)) {
                parent[neighbor] = current;
                open.push_back(neighbor);
            }
        }
    }

    return path;
}

/// @brief Busca A* (A-Star Search)
/// Combina o custo acumulado g(n) com a estimativa heurística h(n): f(n) = g(n) + h(n)
/// @param start Nó inicial da busca
/// @param goal Nó objetivo
/// @return Vetor de coordenadas para movimentação do jogador
vector<Vector2> astar(Node* start, Node* goal) {
    unordered_map<Node*, Node*> parent;
    parent[start] = nullptr;

    unordered_map<Node*, float> costSoFar;
    costSoFar[start] = 0.0f;

    vector<Node*> open = {start};
    vector<Node*> closed;
    vector<Vector2> path;

    Node* lastNode = start;
    path.push_back(nodeToCoord(start));

    while (!open.empty()) {
        // Encontra o nó com menor f(n) = g(n) + h(n) na lista de abertos
        int bestIndex = 0;
        float bestF = costSoFar[open[0]] + heuristic(open[0], goal);
        for (int i = 1; i < (int)open.size(); i++) {
            float f = costSoFar[open[i]] + heuristic(open[i], goal);
            if (f < bestF) {
                bestF = f;
                bestIndex = i;
            }
        }
        Node* current = open[bestIndex];
        open.erase(open.begin() + bestIndex);
        closed.push_back(current);

        // Movimento visual com backtracking suave via menor ancestral comum
        pathBetween(lastNode, current, parent, path);
        lastNode = current;

        if (current->name == goal->name) break;

        for (Node* neighbor : current->neighbors) {
            float newCost = costSoFar[current] + distance(current, neighbor);

            if (notVisited(neighbor->name, closed)) {
                bool inOpen = !notVisited(neighbor->name, open);
                if (!inOpen || newCost < costSoFar[neighbor]) {
                    costSoFar[neighbor] = newCost;
                    parent[neighbor] = current;
                    if (!inOpen) {
                        open.push_back(neighbor);
                    }
                }
            }
        }
    }

    return path;
}
