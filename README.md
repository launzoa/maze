# Maze Game — Graph Search Algorithms Visualizer

Esse projeto é uma simulador de **algoritmos de busca em grafos** em um ambiente inspirado no mapa clássico de *Viridian Forest* (Pokémon FireRed/LeafGreen). O projeto foi desenvolvido em **C++17** com a biblioteca gráfica **[Raylib](https://www.raylib.com/)** e compilado para a Web através de **WebAssembly (Emscripten)**.

👉 **[Clique aqui para jogar direto no navegador!](https://launzoa.github.io/maze/)**

---

## Sobre o Projeto

A ideia desse projeto surgiu a partir do estudo de um projeto acadêmico da materia de Inteligência Artificial. No entanto, o foco do conteúdo em questão, é mais voltado para a Teoria dos Grafos, especificamente na modelagem de espaços de busca e resolução de problemas por meio de agentes exploradores.

Meu objetivo principal com o projeto foi tentar transformar os algoritmos de busca — geralmente ensinados apenas em matrizes ou grafos de ponteiros de nós — em um jogo visual e didático. O jogador pode escolher entre os diferentes algoritmos e assistir em tempo real ao patinho navegando pelos caminhos da floresta, visualizando na prática o comportamento de expansão, backtracking e convergência de cada abordagem. 

---

## Algoritmos de Busca Implementados

Os algoritmos estão divididos em duas categorias de resolução de problemas:

### 1. Buscas Cegas (Uninformed Search)
Algoritmos que não tem conhecimento prévio sobre a distância ou posição relativa do objetivo, guiando-se estritamente pela estrutura topológica do grafo:

* **Breadth-First Search (BFS):** Busca em largura. Explora os nós nível por nível a partir da raiz, garantindo encontrar o caminho com o menor número de passos (arestas).
* **Depth-First Search (DFS):** Busca em profundidade. Mergulha por um ramo até atingir nós folha antes de retroceder (*backtracking*), sendo eficiente em consumo de memória, mas sem garantia de caminho mínimo.
* **Depth-Limited Search (DLS):** Busca em profundidade limitada (configurada com limite de 6 níveis), uma versão alternativa do DFS que evita que a exploração caia em laços infinitos ou ramos excessivamente profundos (daí o motivo do limite de profundidade).
* **Iterative Deepening Search (IDS):** Aprofundamento iterativo. Executa o DLS incrementando progressivamente o limite de profundidade (0, 1, 2, ...).

### 2. Buscas Informadas (Informed)
Algoritmos que utilizam estimativas de custo e funções de avaliação $f(n)$ para priorizar caminhos mais promissores:

* **Uniform Cost Search (UCS):** Busca de custo uniforme (popularmente conhecida como Dijkstra) com $f(n) = g(n)$, onde $g(n)$ é a distância euclidiana acumulada das arestas percorridas. Garante encontrar a rota de menor custo real.
* **Greedy Best-First Search:** Busca gulosa guiada somente pela função heurística $f(n) = h(n)$, onde $h(n)$ é a distância euclidiana direta em linha reta até o nó alvo.
* **A\* Search (A-Star):** O famoso A*, combinando o custo acumulado com a heurística através de $f(n) = g(n) + h(n)$. 

---

## Estrutura das Branches

* **`main`:** Código-fonte em C++17, headers, mapa, assets pixel art (árvores, grama, flores, pato), `CMakeLists.txt` e template para o emscripten (`shell.html`).
* **`gh-pages`:** Contém os binários de WebAssembly (`index.html`, `index.js`, `index.wasm` e `index.data`) para hospedagem no GitHub Pages.

---

## Como Compilar e Executar Localmente

### Pré-requisitos
* Compilador C++17 (GCC ou Clang)
* CMake (>= 3.16)

---

### 1. Versão Nativa Desktop (Linux)
Requer a biblioteca `raylib` instalada no sistema operacional.

```bash
# Configura o diretório de build
cmake -B build

# Compila o executável nativo
cmake --build build

# Executa o jogo
./build/app
```

---

### 2. Versão WebAssembly (Web / Navegador)
Requer o [emsdk (Emscripten)](https://github.com/emscripten-core/emsdk) instalado e ativado no terminal.

```bash
# Configura o projeto com a toolchain do Emscripten (utilizando o Raylib embutido)
emcmake cmake -B build-web -DPLATFORM=Web

# Compila para WebAssembly (.wasm, .js, .html, .data)
cmake --build build-web

# Inicia um servidor HTTP local para testar no navegador
python3 -m http.server 8080 --directory build-web
```
Abra seu navegador em `http://localhost:8080`.

---

Desenvolvido por **[launzoa](https://github.com/launzoa)**.
