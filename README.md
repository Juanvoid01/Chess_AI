# Chess-AI

Chess Engine based on search with Negamax algorithm (similar to minimax).

#### Search :

* Enhanced with alpha-beta pruning.
 
* Transposition table using zobrist hashing.

* Quiescence search extension on captures.

* Iterative Deepening

#### Evaluation :

* Materialistic evaluation.
* Differences between middlegame and endgame.

### Table of Contents
- [What I learnt](#what-i-learnt)
- [Development](#development)
- [How to use it](#how-to-use-it)
- [References](#references)

### What I learnt

- Graphics in OpenGL
- Shaders and Textures
- C++11 features
- Move semantics
- Smart Pointers
- Advanced algorithms
- Minimax
- Alpha beta pruning
- Dynamic programming
- Transposition tables

![alt text](https://github.com/Juanvoid01/Chess_AI/blob/main/res/photos/chessPhoto.png?raw=true)


### Development

-[x] Creating the graphics engine 

    * Abstracting OpenGL into classes
    * Cache of uniforms optimization

-[x] Chess Engine

    * Calculates all the legal moves for each
      chess position
    
    * Chess rules are simple, but there are a lot of  edge cases to prove.

    * See more info in #references

-[x] Factibility test, Perft function

    * We have to compare the results of
      chess engine with perft chess function

-[x] Chess AI with Negamax Algorithm

    * We generate the game tree and use an evaluation function in each position.

    * The difference with minimax is that our evaluation function is symmetric, every node tries to maximize the player with the turn.

-[x] Chess AI optimization with alpha-beta pruning

    * We reduce the number of nodes to visit in the tree pruning nodes with the worst evaluation.
    
    * Enhance of performance with transpositions, using dynamic programming. we store the evaluation of the position so we can use it later if we encounter the same position.

-[x] Transposition Tables with Zobrist hashing.

    * We make a lookup table of chess positions represented with a zobrist 64 bit string, to store the evaluation.

-[x] Iterative deepening
    
    *First we search the best move in 1 depth, the 2 depth, 3, 4 ...  Until we received a signal to stop, then we return the best move found so far.

### How to use it

1. Clone the repository
2. Build with the CMake script
3. Run the executable in ./build/chess_AI

4. To make the bot to move, press 'p'

### References

* Chess Programming wiki : <br>
https://www.chessprogramming.org/Main_Page

* Generate Legal Moves : <br>
https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/

* Minimax Algorithm : <br>
https://en.wikipedia.org/wiki/Minimax

* Alpha-Beta pruning : <br>
https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning

* Zobrist hashing : <br>
https://en.wikipedia.org/wiki/Zobrist_hashing