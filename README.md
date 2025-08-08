# Snaze

[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/ao5BHHrY)

## Apresentação

Snaze é uma simulação do clássico jogo da Cobra implementada em C++. O objetivo deste projeto é programar uma cobra que navega por labirintos e busca pedaços de comida posicionados aleatoriamente, utilizando algoritmos de busca como BFS para encontrar o caminho.

A descrição completa do trabalho pode ser encontrada [aqui](docs/snake_programming_project.pdf).

Além da implementação, é necessário informar detalhes sobre a autoria do projeto e os itens realizados no arquivo [author.md](author.md).

## Funcionalidades

- Simulação de jogo Snake em múltiplos níveis.
- Labirintos customizáveis.
- Algoritmo BFS para navegação automática da cobra até o alimento.
- Gestão de vidas, pontuação e avanço de níveis.
- Interface de texto para interação e exibição do estado do jogo.

## Estrutura do Código

- `source/main.cpp`: Loop principal da simulação.
- `source/simulation.cpp/.h`: Gerenciamento dos estados do jogo, níveis, cobra e eventos.
- `source/level.cpp/.hpp`: Representação do labirinto, posição do alimento, lógica de movimentação.
- `source/snake.cpp/.hpp`: Lógica da cobra, movimento, crescimento e vidas.
- `source/player.cpp/.hpp`: Algoritmo BFS para busca do alimento.
- `source/render.cpp/.hpp`: Mensagens de boas-vindas, instruções, e exibição do progresso do jogo.

## Como Compilar e Executar

Execute os seguintes comandos no terminal:

```sh
cmake -S . -B build
cmake --build build
./build/snaze <options> <maze-file>
```

## Exemplo de Uso

```sh
./build/snaze --lives 3 --levels levels/maze.txt
```

- `<options>`: Parâmetros como número de vidas, arquivo de níveis, etc.
- `<maze-file>`: Caminho para o arquivo do labirinto.
