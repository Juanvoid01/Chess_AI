# Chess-AI

Project in progress... 50%

### Table of Contents
- [What I learnt](#what-i-learnt)
- [Development](#development)
- [How to use it](#how-to-use-it)
- [References](#references)

### What I learnt so far...

- Graphics in OpenGL
- C++11 features
- Move semantics
- Smart Pointers
- Shaders and Textures


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

-[ ] Factibility test

    * We have to compare the results of
      chess engine with perft chess function

-[ ] Chess AI with Minimax Algorithm

    * We generate the game tree and use an evaluation function in each position.

-[ ] Chess AI optimization with alpha-beta pruning

    * We reduce the number of nodes to visit in the tree pruning nodes with the worst evaluation.
    
    * Enhance of performance with transpositions, using dynamic programming. we store the evaluation of the position so we can use it later if we encounter the same position.


### How to use it

1. Clone the repository
2. Build with the CMake script
3. Run the executable in ./build/chess_AI

### References

* Chess Programming wiki : <br>
https://www.chessprogramming.org/Main_Page

* Generate Legal Moves : <br>
https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/

* Minimax Algorithm : <br>
https://en.wikipedia.org/wiki/Minimax

* Alpha-Beta pruning : <br>
https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning