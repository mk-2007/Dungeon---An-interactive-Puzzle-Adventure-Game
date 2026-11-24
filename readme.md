# 🏰 Dungeon: An Interactive Puzzle-Adventure Game
### FAST National University of Computer and Emerging Sciences (FAST-NUCES)
**Course:** Data Structures & Algorithms (CS) · **Section:** G  

---

<div align="center">

![C++](https://img.shields.io/badge/Language-C%2B%2B11%20%2F%20C%2B%2B17-00599C?logo=cplusplus&logoColor=white)
![Raylib](https://img.shields.io/badge/Graphics-Raylib%205.0-white?logo=raylib&logoColor=black)
![DSA Engine](https://img.shields.io/badge/DSA-Pure%20Custom%20(No%20STL)-success)
![Platform](https://img.shields.io/badge/Platform-Windows%20(MinGW)-0078D6?logo=windows&logoColor=white)
![Architecture](https://img.shields.io/badge/Design-Decoupled%20Scene%20Engine-blueviolet)

</div>

---

> [!NOTE]
> **Project Overview & Philosophy**  
> **Dungeon** is an interactive puzzle-adventure game designed to transform theoretical data structures into practical, interactive game mechanics. Instead of observing static textbook demonstrations or dry console menus, players must apply the fundamental operations and invariant rules of **Stacks**, **Queues**, **Binary Search Trees**, and **Graphs** to solve interconnected dungeon chambers and earn their escape.

> [!IMPORTANT]
> **Zero-STL Custom Data Structure Guarantee**  
> All underlying data structures (`Stack<T>`, `Queue<T>`, `BST`, and `Graph`) are engineered completely **from scratch** using raw dynamic memory, manual pointer manipulation, and explicit resource management. Standard Template Library (STL) containers are strictly prohibited in the `src/ds/` core layer.

---

## 👥 Project Team & Responsibilities

| Student Name | Roll Number | Primary Module & Responsibilities |
| :--- | :--- | :--- |
| **Mahandar Kumar** | **25k-0883** | **Team Lead · Core Engine & Raylib GUI Integration**<br>• Designed modular `Scene` lifecycle and high-DPI `UI` rendering engine.<br>• Engineered custom `BST` with 3-case node deletion.<br>• Authored **Chamber 3: The Shifting Maze** (Logic, Console & 2D GUI). |
| **Ramiq Ehsan** | **25k-0863** | **Data Structure Engineer (Stack & Queue)**<br>• Implemented dynamic `Stack<T>` and linked-list `Queue<T>`.<br>• Authored **Chamber 1: Stack Vault** (Logic, Console & 2D GUI).<br>• Authored **Chamber 2: The Forgotten Queue** (Logic, Console & 2D GUI). |
| **Jatin Malani** | **25k-3054** | **Graph & Documentation Lead**<br>• Implemented Adjacency List `Graph` and BFS pathfinding.<br>• Authored **Chamber 4: The Bridge Builder** (Logic, Console & 2D GUI).<br>• Authored project documentation, specifications, and test suites. |

---

## 🏛️ The Four Dungeon Chambers

```
                           ┌───────────────────────────────┐
                           │      THE DUNGEON HUB          │
                           │   Story Mode & Practice Mode  │
                           └───────────────┬───────────────┘
           ┌───────────────────────┬───────┴───────┬───────────────────────┐
           ▼                       ▼               ▼                       ▼
 ┌───────────────────┐   ┌───────────────────┐   ┌───────────────────┐   ┌───────────────────┐
 │ CHAMBER 1: STACK  │   │ CHAMBER 2: QUEUE  │   │  CHAMBER 3: BST   │   │ CHAMBER 4: GRAPH  │
 │    Stack Vault    │   │  Forgotten Queue  │   │   Shifting Maze   │   │  Bridge Builder   │
 └───────────────────┘   └───────────────────┘   └───────────────────┘   └───────────────────┘
```

### 1. Chamber 1: The Stack Vault
* **Underlying Data Structure:** `Stack<T>` (Dynamic Array Implementation, LIFO).
* **Game Objective:** Uncover and retrieve the hidden **Golden Key (`1000`)** buried at the base of the crate stacks.
* **Game Mechanics & Invariants:**
  * **LIFO Access:** Only the topmost crate of any column can be popped or inspected.
  * **Helper Stack Transfer:** To dig down to the key, crates must be pushed onto a designated Helper stack.
  * **Alternating Sign Rule:** Crates placed on the helper must alternate signs (positive $\rightarrow$ negative $\rightarrow$ positive).
  * **Parity Constraint (Hard Mode):** Crates must additionally alternate parity (even $\rightarrow$ odd $\rightarrow$ even).

```
         [ STACK 1 ]      [ STACK 2 ]      [ STACK 3 ]      [ HELPER STACK ]
       ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌──────────────────┐
Top ──>│  Crate +6   │  │  Crate -7   │  │  Crate -10  │  │                  │ <── PUSH / POP
       ├─────────────┤  ├─────────────┤  ├─────────────┤  │  (Alternating    │     Strict LIFO
       │  Crate -5   │  │  Crate +6   │  │  Crate +9   │  │   Sign & Parity) │
       ├─────────────┤  ├─────────────┤  ├─────────────┤  │                  │
       │  Crate +4   │  │  Crate -2   │  │  Crate -8   │  │                  │
       ├─────────────┤  ├─────────────┤  ├─────────────┤  │                  │
Base ──│  Crate +3   │  │  Crate +1   │  │  KEY 1000!  │  │                  │
       └─────────────┘  └─────────────┘  └─────────────┘  └──────────────────┘
```

---

### 2. Chamber 2: The Forgotten Queue
* **Underlying Data Structure:** `Queue<T>` (Singly Linked List Implementation, FIFO).
* **Game Objective:** Harmonize chaotic elemental spirits on an ancient conveyor belt so that every adjacent spirit pair satisfies mutual harmony conditions.
* **Game Mechanics & Invariants:**
  * **FIFO Arrival:** Spirits board the conveyor strictly at the rear (`enqueue`) and exit from the front (`dequeue`).
  * **Elemental Affinity Rules:** 7 spirit archetypes (Fire, Water, Earth, Air, Light, Shadow, Neutral) with specific adjacency rules (e.g., Fire cannot be next to Water unless buffered by Earth or Air).
  * **Queue-Rotation Undo:** In keeping with strict FIFO constraints, the **Undo** operation does not mutate arbitrary pointers; it rotates the queue by performing $(N-1)$ successive `dequeue()` and `enqueue()` steps.

```
       DEQUEUE (Exit)                                             ENQUEUE (Entry)
            ◄───────  [Sp. 1] ◄── [Sp. 2] ◄── [Sp. 3] ◄── [Sp. 4]  ◄───────
                       Front                                Rear
                     ┌───────────────────────────────────────────┐
                     │   CONVEYOR BELT (Linked List Queue)       │
                     └───────────────────────────────────────────┘
```

---

### 3. Chamber 3: The Shifting Maze
* **Underlying Data Structure:** `BST` (Binary Search Tree with dynamic pointers and full 3-case deletion).
* **Game Objective:**
  1. Navigate through branching mystical rooms using BST ordering properties to find the Key chamber.
  2. Detect an artificially corrupted node in the tree and repair it to stabilize the maze.
* **Game Mechanics & Invariants:**
  * **Binary Search Decisions:** From room $V$, choosing the **Left Door** is valid only if the target is $< V$; choosing the **Right Door** is valid only if the target is $> V$.
  * **3-Strike Mistake Counter:** Making three incorrect navigational decisions triggers a cave-in and resets the room.
  * **Live BST Repair:** When the corrupted node is reached, the player executes a repair action that invokes live `BST::remove()` (handling nodes with 0, 1, or 2 children via in-order successor) followed by `BST::insert()` to restore tree balance.

```
                                  [ Room 50 ]
                                 /           \
                     [ Room 30 ]               [ Room 70 ]
                    /           \             /           \
              [ Room 20 ]   [ Room 40 ]  [ Room 60 ]  [ Room 80 ]
                                              ▲
                                      (Corrupted Node!)
                                  Fixed via BST Deletion & Re-insertion
```

---

### 4. Chamber 4: The Bridge Builder
* **Underlying Data Structure:** `Graph` (Adjacency List Representation) evaluated via `Queue<T>` Breadth-First Search.
* **Game Objective:** Construct an active bridge route connecting **Start Island (0)** to **Goal Island** within a strict move budget.
* **Game Mechanics & Invariants:**
  * **Dynamic Edge Mutation:** Building a bridge calls `Graph::addEdge()`; removing a bridge calls `Graph::removeEdge()`.
  * **Switch-Gated Sites:** Certain critical bridge slots are locked behind remote energy switches. Toggling a switch activates the slot, consuming 1 move.
  * **Real-Time BFS Pathfinding:** On every action, an internal Breadth-First Search traverses island adjacency lists to determine whether a valid path exists, visually illuminating the connected route.

```
            [ Island 0: START ] ═════════ [ Island 1 ]
                     │                           ║
                     │ (Locked Bridge)           ║ (Active Bridge)
                     ▼                           ▼
            [ Island 2: SWITCH ] ════════ [ Island 3: GOAL ]
```

---

## 🎮 Game Controls & Keyboard Shortcuts

| Input | Context | Action |
| :--- | :--- | :--- |
| **Left Mouse Click** | Universal | Interact with doors, buttons, crates, spirits, switches, and tree nodes. |
| **`F11`** or **`Alt + Enter`** | GUI Window | Toggle **Fullscreen Mode** with letterboxed aspect ratio preservation. |
| **`F12`** | GUI Window | Capture a clean high-resolution screenshot saved to `screenshot.png`. |
| **Window Resize** | GUI Window | Drag borders to freely resize; virtual 1000×650 canvas auto-scales cleanly. |
| **`Esc`** | GUI Window | Return to the Hub scene or exit. |

---

## 🛠️ Build and Execution Guide

### Prerequisites
* **Compiler:** MinGW-w64 / GCC supporting C++11 or higher.
* **Libraries:** Raylib 5.0 (static library included in `lib/raylib/` for zero-install building on Windows).

### 1. Compile the Game
Open PowerShell or Command Prompt in the repository root:

```cmd
g++ -std=c++11 src/main.cpp src/core/RaylibCompat.cpp src/core/UI.cpp src/core/Game.cpp src/core/Hub.cpp src/core/HubScene.cpp src/chambers/StackChamber.cpp src/chambers/StackVaultConsole.cpp src/chambers/StackVaultScene.cpp src/chambers/SpiritConveyerBelt.cpp src/chambers/ForgottenQueueConsole.cpp src/chambers/ForgottenQueueScene.cpp src/chambers/ShiftingMaze.cpp src/chambers/ShiftingMazeConsole.cpp src/chambers/ShiftingMazeScene.cpp src/chambers/BridgeBuilder.cpp src/chambers/BridgeBuilderConsole.cpp src/chambers/BridgeBuilderScene.cpp -Ilib/raylib/include -Llib/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -o dungeon.exe
```

### 2. Launch Modes

* **Full 2D Graphical Adventure (Default):**
  ```cmd
  .\dungeon.exe
  ```

* **Direct Chamber Launch (Quick-Play GUI Flags):**
  ```cmd
  .\dungeon.exe --scene stack    # Launch Chamber 1: Stack Vault
  .\dungeon.exe --scene queue    # Launch Chamber 2: Forgotten Queue
  .\dungeon.exe --scene tree     # Launch Chamber 3: The Shifting Maze
  .\dungeon.exe --scene graph    # Launch Chamber 4: The Bridge Builder
  ```

* **Console Terminal Mode (TUI Fallback):**
  ```cmd
  .\dungeon.exe --console
  .\dungeon.exe --console --scene stack
  ```

---

## 🧪 Comprehensive Unit Test Suite

The project includes unit test binaries verifying every underlying data structure and chamber state machine:

```cmd
# 1. Stack Data Structure Test
g++ -std=c++17 tests/test_stack.cpp -o test_stack.exe && .\test_stack.exe

# 2. Queue Data Structure Test
g++ -std=c++17 tests/test_queue.cpp -o test_queue.exe && .\test_queue.exe

# 3. Binary Search Tree Test
g++ -std=c++17 tests/test_bst.cpp -o test_bst.exe && .\test_bst.exe

# 4. Graph & BFS Test
g++ -std=c++17 tests/test_graph.cpp -o test_graph.exe && .\test_graph.exe

# 5. Stack Vault Chamber Logic Test
g++ -std=c++17 tests/test_StackChamber.cpp src/chambers/StackChamber.cpp -o test_stack_chamber.exe && .\test_stack_chamber.exe

# 6. Shifting Maze Chamber Logic Test
g++ -std=c++17 tests/test_shifting_maze.cpp src/chambers/ShiftingMaze.cpp -o test_shifting_maze.exe && .\test_shifting_maze.exe

# 7. Bridge Builder Chamber Logic Test
g++ -std=c++17 tests/test_bridge_builder.cpp src/chambers/BridgeBuilder.cpp -o test_bridge_builder.exe && .\test_bridge_builder.exe

# 8. Dungeon Progress & Story Mode Integration Test
g++ -std=c++17 tests/test_game_progress.cpp -o test_game_progress.exe && .\test_game_progress.exe
```

---

## 📊 Alignment with Project Proposal

| Proposal Requirement | Implementation in Dungeon | Verification Status |
| :--- | :--- | :--- |
| **Pure Custom Data Structures** | `Stack<T>` (dynamic array), `Queue<T>` (singly linked list), `BST` (nodes with pointers), `Graph` (adjacency list). Zero STL. | ✅ Verified via dedicated unit tests. |
| **Interactive 2D GUI** | Developed using Raylib 5.0 with custom decoupled scene management, dual high-res vector typography, and responsive letterboxing. | ✅ Full GUI with mouse interaction and animations. |
| **Dual Gameplay Modes** | **Story Mode** (sequential unlocks: Stack $\rightarrow$ Queue $\rightarrow$ Tree $\rightarrow$ Graph) and **Practice Mode** (free chamber select). | ✅ Configurable via Hub menu. |
| **Data Structure-Driven Mechanics** | Player moves directly execute `push()`, `pop()`, `enqueue()`, `dequeue()`, `BST::remove()`, `BST::insert()`, `addEdge()`, and BFS traversal. | ✅ Gameplay fails when invariant rules are broken. |
| **Progressive Difficulty** | Easy vs. Hard modes for chambers (e.g. alternating sign + parity on Stack, tighter move limits and dual switches on Graph). | ✅ Interactive toggles and verified puzzle constraints. |
| **Visual Feedback & Diagnostics** | Immediate toast notifications, color-coded status badges, highlighted current positions, and dynamic graph/tree rendering. | ✅ Real-time feedback on valid and invalid actions. |
| **Platform Portability** | Runs natively on Windows with bundled precompiled static Raylib library and MinGW GCC shims. | ✅ Zero external setup required. |

---

<div align="center">
<b>Developed with passion for Data Structures & Algorithms · Fall 2026</b>
</div>
