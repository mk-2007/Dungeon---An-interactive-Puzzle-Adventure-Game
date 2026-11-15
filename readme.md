# Dungeon: An Interactive Puzzle-Adventure Game

Dungeon is a C++ data-structures project. Its planned chambers use custom
implementations of a stack, queue, binary search tree, and graph to determine
the puzzle rules.

## Project status

The team is building and testing the data structures and game logic before
starting the graphical interface.

- **Stack and Queue:** implemented in `src/ds/Stack.h` and `src/ds/Queue.h`.
- **Binary Search Tree:** implemented in `src/ds/BST.h` with standalone tests in
  `tests/test_bst.cpp`. It supports insertion, removal, search, height,
  in-order traversal, and clearing. Duplicate values are ignored; an empty
  tree has height `-1`.
- **Graph:** implemented in `src/ds/Graph.h` with standalone tests in
  `tests/test_graph.cpp`.
- **Shifting Maze logic:** implemented in `src/chambers/ShiftingMaze.h` and
  `src/chambers/ShiftingMaze.cpp`, with tests in
  `tests/test_shifting_maze.cpp`. Players follow left/right BST branches toward
  a target, correct one designated value by removing and reinserting it, and
  win after both objectives are complete. Three incorrect choices lose the
  round.
- **Game shell, GUI, and other chambers:** not implemented yet.

This branch contains Mahandar's BST and Shifting Maze logic. Stack and Queue
are Ramiq's modules; the Graph, Bridge Builder, and project documentation are
Jatin's assigned modules.

## Run the data-structure tests

Run these commands from the repository root with a C++17 compiler such as
MinGW-w64 g++:

```powershell
g++ -std=c++17 tests/test_stack.cpp -o test_stack.exe
.\test_stack.exe

g++ -std=c++17 tests/test_queue.cpp -o test_queue.exe
.\test_queue.exe

g++ -std=c++17 tests/test_bst.cpp -o test_bst.exe
.\test_bst.exe

g++ -std=c++17 tests/test_graph.cpp -o test_graph.exe
.\test_graph.exe

g++ -std=c++17 tests/test_shifting_maze.cpp src/chambers/ShiftingMaze.cpp -o test_shifting_maze.exe
.\test_shifting_maze.exe
```

Each test is a separate console program. The game build and GUI instructions
will be added when the game shell is available.