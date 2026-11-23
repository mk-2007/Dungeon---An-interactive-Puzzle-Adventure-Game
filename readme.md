# Dungeon: An Interactive Puzzle-Adventure Game

Dungeon is an interactive C++ puzzle-adventure game demonstrating the practical application of core data structures through engaging gameplay. The game features four chambers, each driven by a hand-crafted custom data structure implemented from scratch without relying on STL containers.

---

## The Chambers

1. **Chamber 1: Stack Vault (Stack - LIFO)**
   - **Data Structure:** `Stack<T>` (Dynamic Array, LIFO).
   - **Objective:** Reveal the Golden Key (`1000`) hidden beneath piles of crates by moving elements to the Helper stack.
   - **Mechanics:** LIFO push/pop. Alternating signs (+ / -) required on helper, plus alternating parity (even / odd) in Hard mode.

2. **Chamber 2: The Forgotten Queue (Queue - FIFO)**
   - **Data Structure:** `Queue<T>` (Singly Linked List, FIFO).
   - **Objective:** Align elemental spirits on the conveyor belt satisfying their neighbor harmony rules.
   - **Mechanics:** Spirits board the conveyor at the rear in FIFO arrival order. Undo operation is achieved via pure queue rotation ($N-1$ dequeues and re-enqueues).

3. **Chamber 3: The Shifting Maze (Binary Search Tree)**
   - **Data Structure:** `BST` (Binary Search Tree with full 3-case node deletion and dynamic links).
   - **Objective:** (1) Navigate through branching rooms by BST value order to locate the Key room. (2) Identify and repair the corrupted node.
   - **Mechanics:** Moving left/right validates against BST ordering property. Repairing calls live `BST::remove()` and `BST::insert()`.

4. **Chamber 4: The Bridge Builder (Graph - BFS Pathfinding)**
   - **Data Structure:** `Graph` (Adjacency List representation) and `Queue<T>`.
   - **Objective:** Connect Start Island to Goal Island before exhausting the move budget.
   - **Mechanics:** Building/removing bridges directly mutates graph edges. Remote switches unlock blocked bridge sites. Connectivity is evaluated on every move via Breadth-First Search (BFS).

---

## Build and Run (2D Raylib GUI)

Compile with MinGW `g++` (C++11 or higher):

```cmd
g++ -std=c++11 src/main.cpp src/core/RaylibCompat.cpp src/core/UI.cpp src/core/Game.cpp src/core/Hub.cpp src/core/HubScene.cpp src/chambers/StackChamber.cpp src/chambers/StackVaultConsole.cpp src/chambers/StackVaultScene.cpp src/chambers/SpiritConveyerBelt.cpp src/chambers/ForgottenQueueConsole.cpp src/chambers/ForgottenQueueScene.cpp src/chambers/ShiftingMaze.cpp src/chambers/ShiftingMazeConsole.cpp src/chambers/ShiftingMazeScene.cpp src/chambers/BridgeBuilder.cpp src/chambers/BridgeBuilderConsole.cpp src/chambers/BridgeBuilderScene.cpp -Ilib/raylib/include -Llib/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -o dungeon.exe
```

### Launch Modes:

- **Launch Full GUI Game (Default):**
  ```cmd
  .\dungeon.exe
  ```
- **Direct Chamber GUI Launch (CLI flag):**
  ```cmd
  .\dungeon.exe --scene stack
  .\dungeon.exe --scene queue
  .\dungeon.exe --scene tree
  .\dungeon.exe --scene graph
  ```
- **Console TUI Fallback:**
  ```cmd
  .\dungeon.exe --console
  ```

---

## Run Data-Structure Unit Tests

```cmd
g++ -std=c++17 tests/test_stack.cpp -o test_stack.exe && .\test_stack.exe
g++ -std=c++17 tests/test_queue.cpp -o test_queue.exe && .\test_queue.exe
g++ -std=c++17 tests/test_bst.cpp -o test_bst.exe && .\test_bst.exe
g++ -std=c++17 tests/test_graph.cpp -o test_graph.exe && .\test_graph.exe
g++ -std=c++17 tests/test_shifting_maze.cpp src/chambers/ShiftingMaze.cpp -o test_shifting_maze.exe && .\test_shifting_maze.exe
g++ -std=c++17 tests/test_bridge_builder.cpp src/chambers/BridgeBuilder.cpp -o test_bridge_builder.exe && .\test_bridge_builder.exe
g++ -std=c++17 tests/test_StackChamber.cpp src/chambers/StackChamber.cpp -o test_stack_chamber.exe && .\test_stack_chamber.exe
g++ -std=c++17 tests/test_game_progress.cpp -o test_game_progress.exe && .\test_game_progress.exe
```

---

## Project Team & Architecture

- **Mahandar Kumar (25k-0883):** Team Lead · Core Engine · Raylib GUI Integration · BST + Chamber 3 (The Shifting Maze).
- **Ramiq Ehsan (25k-0863):** Data Structure Engineer · Stack & Queue · Chamber 1 (Stack Vault) + Chamber 2 (Forgotten Queue).
- **Jatin Malani (25k-3054):** Graph Engineer · Documentation Lead · Graph + Chamber 4 (The Bridge Builder).
