# 🚀 AI Lab Test Quick Revision (Plain English Algorithms)

> **Quick Summary Sheet:** Concise, step-by-step algorithms in plain English for all 7 lab problems. No code clutter.

---

## 📌 Index of Problems
1. [P1: Tic-Tac-Toe Winner Checker](#1-tic-tac-toe-winner-checker)
2. [P2: Water Jug Problem (BFS)](#2-water-jug-problem-bfs)
3. [P3: Smart Parking System (2D Grid BFS)](#3-smart-parking-system-2d-grid-bfs)
4. [P4: Smart Cleaning Robot (Simple Reflex vs. Model-Based)](#4-smart-cleaning-robot-simple-reflex-vs-model-based)
5. [P5: Delivery Robot (Goal-Based BFS vs. Utility-Based Cost Search)](#5-delivery-robot-goal-based-bfs-vs-utility-based-cost-search)
6. [P6: Cost-Aware Delivery Robot (Uniform Cost Search - UCS)](#6-cost-aware-delivery-robot-uniform-cost-search---ucs)
7. [P7: Rescue Robot (Depth-Limited Search - DLS)](#7-rescue-robot-depth-limited-search---dls)
8. [Algorithm Comparison Cheat Sheet](#8-algorithm-comparison-cheat-sheet)

---

## 1. Tic-Tac-Toe Winner Checker

### 🎯 Goal:
Given a 3×3 matrix of `'X'`, `'O'`, and `'_'` (empty), determine if Player X won, Player O won, it is a Draw, or the Game is still in progress.

### 📋 Plain English Algorithm:
1. **Input Matrix**: Read the 3×3 grid of characters from the user.
2. **Check Rows**: Loop through row $i = 0, 1, 2$. If all 3 cells in a row are identical and not `'_'`, return that player as the winner.
3. **Check Columns**: Loop through column $j = 0, 1, 2$. If all 3 cells in a column are identical and not `'_'`, return that player as the winner.
4. **Check Diagonals**:
   - Main Diagonal: Check `(0,0)`, `(1,1)`, and `(2,2)`.
   - Anti Diagonal: Check `(0,2)`, `(1,1)`, and `(2,0)`.
   - If all 3 cells match and are not `'_'`, return that player as the winner.
5. **Check Empty Cells**: If no player won, scan the entire grid:
   - If any cell contains `'_'`, output: *"Game still in progress"*.
   - If no cell is `'_'` and no winner exists, output: *"Draw"*.

---

## 2. Water Jug Problem (BFS)

### 🎯 Goal:
Given Jug $A$ (capacity $a = 4\text{L}$) and Jug $B$ (capacity $b = 3\text{L}$), find the minimum sequence of steps to measure exactly $c = 2\text{L}$.

### ⚙️ State & 6 Allowed Operations:
State is represented as $(x, y)$, where $x$ is water in Jug A, and $y$ is water in Jug B.
1. **Fill A**: $(a, y) \to (4, y)$
2. **Fill B**: $(x, b) \to (x, 3)$
3. **Empty A**: $(0, y)$
4. **Empty B**: $(x, 0)$
5. **Pour A to B**: Transfer amount $t = \min(x, b - y) \implies (x - t, y + t)$
6. **Pour B to A**: Transfer amount $t = \min(y, a - x) \implies (x + t, y - t)$

### 📋 Plain English Algorithm (BFS):
1. **Initialize**:
   - Create a FIFO Queue `Q` and insert starting state `(0, 0)`.
   - Mark `(0, 0)` as `visited` in a map/set.
   - Use a `parent` map to track where each state came from.
2. **Search Loop**: While `Q` is not empty:
   - Dequeue the front state `(x, y)`.
   - **Goal Check**: If $x == c$ (or $y == c$), stop the search!
   - Generate all **6 possible next states** using the operations above.
   - For each next state:
     - If it has **not been visited**:
       - Mark it as `visited`.
       - Record `parent[next] = current`.
       - Enqueue `next` into `Q`.
3. **Reconstruct Path**:
   - Start from the goal state and follow `parent` pointers backwards until `(0, 0)`.
   - Reverse the sequence to display the step-by-step trace from start to goal.

---

## 3. Smart Parking System (2D Grid BFS)

### 🎯 Goal:
In a 2D parking grid, find the **nearest reachable available parking spot** (`'A'`) starting from entrance (`'E'`).

### 🗺️ Symbols:
- `E`: Entrance (Start)
- `A`: Available Parking (Goal)
- `R`: Road (Traversable)
- `O`: Occupied Space (Obstacle)
- `X`: Wall / Blocked (Obstacle)

### 📋 Plain English Algorithm (BFS):
1. **Locate Entrance**: Scan the grid to find coordinates $(startR, startC)$ of `'E'`.
2. **Initialize**:
   - Push $(startR, startC)$ into a FIFO Queue `Q`.
   - Mark `visited[startR][startC] = true`.
   - Create a 2D `parent` array initialized to `(-1, -1)`.
3. **Explore Level by Level**: While `Q` is not empty:
   - Dequeue front cell $(r, c)$.
   - **Goal Check**: If `grid[r][c] == 'A'`, save $(r, c)$ as the goal and break!
   - For all 4 directions (**Up, Down, Left, Right**):
     - Calculate neighbor $(nr, nc)$.
     - **Valid Check**: If inside grid bounds, not visited, and cell is NOT `'X'` and NOT `'O'`:
       - Mark `visited[nr][nc] = true`.
       - Store `parent[nr][nc] = (r, c)`.
       - Push $(nr, nc)$ into `Q`.
4. **Reconstruct & Print**:
   - If goal found: trace `parent` backwards from goal to `'E'`, reverse the list, and print the route and total movements ($\text{movements} = \text{path size} - 1$).
   - If queue becomes empty without finding `'A'`: print *"No reachable parking found"*.

---

## 4. Smart Cleaning Robot (Simple Reflex vs. Model-Based)

### 🎯 Goal:
Clean all dirty cells (`'D'`) in a 2D grid while avoiding blocked walls (`'X'`). Compare a **Simple Reflex Agent** (no memory) vs. a **Model-Based Agent** (memory + BFS).

---

### 🤖 Agent 1: Simple Reflex Agent (No Memory)
- **Idea**: Operates strictly on current cell condition and fixed movement rules.
- **Algorithm**:
  1. Start at robot position $(r, c)$.
  2. Loop (up to a step limit, e.g. 1000 moves, to prevent infinite loops):
     - **Condition 1 (Clean)**: If `grid[r][c] == 'D'`, clean it $\to$ change to `'C'`, increment `cleaned`, and continue.
     - **Condition 2 (Move)**: If current cell is clean, try moving in fixed priority order: **Up $\to$ Right $\to$ Down $\to$ Left**.
       - Take the *first* neighbor that is inside grid bounds and not `'X'`.
       - Update robot position $(r, c)$, increment `moves`, and break to next step.
     - If no valid move is possible, stop.
  3. Output: Total dirty cells cleaned and movement count.

---

### 🧠 Agent 2: Model-Based Reflex Agent (With Memory & Planning)
- **Idea**: Remembers visited state and uses BFS to find the shortest route to the nearest dirty cell.
- **Algorithm**:
  1. Start at robot position $(r, c)$.
  2. While there are still dirty cells remaining:
     - If current cell is `'D'`, clean it $\to$ change to `'C'`, increment `cleaned`, and continue.
     - **Plan Path**: Run a local BFS starting from $(r, c)$ to find the shortest path to the nearest remaining `'D'`.
     - If no dirty cell is reachable, stop.
     - **Step Along Path**: Move the robot **one step** along the planned BFS path $(r = path[0].first, c = path[0].second)$, increment `moves`.
  3. Output: Total dirty cells cleaned, total movements, and display the final cleaned grid.

---

## 5. Delivery Robot (Goal-Based BFS vs. Utility-Based Cost Search)

### 🎯 Goal:
Robot starts at $S$, picks up package at $P$, and delivers it to destination $G$.

---

### 📦 Part A: Goal-Based Agent (Shortest Steps via BFS)
- **Goal**: Minimize total **number of movement steps**.
- **Algorithm**:
  1. **Stage 1 ($S \to P$)**: Run BFS starting from $S$ with target $P$. Reconstruct path and count movements.
  2. **Stage 2 ($P \to G$)**: Run BFS starting from $P$ with target $G$. Reconstruct path and count movements.
  3. **Total Movements** = $(\text{moves } S \to P) + (\text{moves } P \to G)$.

---

### ⚖️ Part B: Utility-Based Agent (Lowest Total Cost via UCS / Dijkstra)
- **Goal**: Minimize total **utility cost** using the multi-factor formula:
  $$\text{Cost} = 0.4 \times \text{Distance} + 0.3 \times \text{Energy} + 0.2 \times \text{Risk} + 0.1 \times \text{Traffic}$$
  - Normal cells: $\text{Cost} = 0.4(1) + 0.3(1) + 0.2(1) + 0.1(1) = 1.0$.
  - Shortcut/Risky cells $(2,6)$ and $(2,7)$: Energy = 5, Risk = 4, Traffic = 3 $\implies \text{Cost} = 3.0$.
- **Algorithm**:
  1. **Stage 1 ($S \to P$)**: Run Uniform Cost Search (Dijkstra) using a Min-Heap priority queue to find the lowest-cost path from $S$ to $P$.
  2. **Stage 2 ($P \to G$)**: Run Uniform Cost Search from $P$ to $G$.
  3. **Total Cost** = $(\text{cost } S \to P) + (\text{cost } P \to G)$.
  4. **Key Finding**: The Utility Agent may choose a route with *more steps* to avoid expensive/risky cells, achieving a lower overall cost.

---

## 6. Cost-Aware Delivery Robot (Uniform Cost Search - UCS)

### 🎯 Goal:
Find the route with minimum total path cost from $S$ to $G$ in a terrain grid with varying entry costs.

### 💰 Terrain Entry Costs:
- `S` (Start) = $0$
- `G` (Goal) = $1$
- `R` (Road) = $1$
- `M` (Mud / Rough) = $3$
- `T` (Traffic Congested) = $5$
- `X` (Blocked) = Not traversable

### 📋 Plain English Algorithm (UCS):
1. **Initialize**:
   - Create a Min-Heap Priority Queue `PQ` storing elements of `(cost, position)`.
   - Maintain a 2D array `gCost[10][10]` initialized to $\infty$ (large number like `1e9`).
   - Maintain a 2D array `visited[10][10]` = `false`.
   - Set `gCost[start] = 0` and push `(0, start)` into `PQ`.
2. **Expansion Loop**: While `PQ` is not empty:
   - Extract the element with the **lowest cost** $(currCost, curr)$ from `PQ`.
   - If `visited[curr]` is already true, skip it (outdated entry).
   - Mark `visited[curr] = true` and record `curr` in the `expansionOrder` list.
   - **Goal Check**: If `curr == goal`, stop the search!
   - For all 4 directions (**Up, Down, Left, Right**):
     - Calculate neighbor cell $(nr, nc)$.
     - If inside bounds and not `'X'`:
       - Compute `newCost = currCost + cost(grid[nr][nc])`.
       - If `newCost < gCost[nr][nc]`:
         - Update `gCost[nr][nc] = newCost`.
         - Store `parent[nr][nc] = curr`.
         - Push `(newCost, (nr, nc))` into `PQ`.
3. **Reconstruct Path**:
   - Trace `parent` from $G$ back to $S$, reverse the path, and display:
     - Nodes expanded (in order).
     - Minimum-cost route.
     - Total movements and total path cost.

---

## 7. Rescue Robot (Depth-Limited Search - DLS)

### 🎯 Goal:
Navigate from $S$ to $G$ using Depth-First Search with a hard depth cutoff $L$ to demonstrate how depth limits bound exploration.

### 🧭 Fixed Successor Order (Mandatory):
**Right $\to$ Down $\to$ Left $\to$ Up**

### 📋 Plain English Algorithm (Recursive DLS):
1. **Function**: `DLS(curr, goal, depth, limit, path)`
2. **Step 1 - Record & Goal Test**:
   - Add `curr` to the `expandedNodes` list.
   - If `curr == goal`: save `solution = path` and return `SUCCESS` ($1$).
3. **Step 2 - Cutoff Test**:
   - If `depth == limit`: return `CUTOFF` ($0$).
4. **Step 3 - Recursive Exploration**:
   - Set a flag `cutoffFound = false`.
   - For each move in fixed order (**Right, Down, Left, Up**):
     - Calculate $(nr, nc)$.
     - If inside grid, not `'X'`, and **NOT on current recursion path** (`!onPath[nr][nc]`):
       - Mark `onPath[nr][nc] = true`.
       - Push $(nr, nc)$ to `path`.
       - Call `result = DLS((nr, nc), goal, depth + 1, limit, path)`.
       - **Backtrack**: Pop $(nr, nc)$ from `path` and set `onPath[nr][nc] = false`.
       - If `result == SUCCESS`, immediately return `SUCCESS`.
       - If `result == CUTOFF`, set `cutoffFound = true`.
5. **Step 4 - Return Outcome**:
   - If `cutoffFound == true`, return `CUTOFF` ($0$).
   - Otherwise, return `FAILURE` ($-1$).

### 🧪 Two Test Runs:
- **Run 1 ($L = 18$)**: Terminate with **CUTOFF** (solution requires 22 steps, which exceeds 18).
- **Run 2 ($L = 25$)**: Successfully reaches goal with **SUCCESS** and prints the 22-move route.

---

## 8. Algorithm Comparison Cheat Sheet

| Feature | BFS (Breadth-First) | UCS (Uniform Cost) | DLS (Depth-Limited) |
| :--- | :--- | :--- | :--- |
| **Data Structure** | FIFO Queue | Min-Heap Priority Queue | Recursion Stack |
| **Node Expansion Order** | Shallowest node first | Smallest path cost $g(n)$ first | Deepest node first up to $L$ |
| **Successor Order** | Any (typically U, D, L, R) | By edge cost | **Fixed: Right, Down, Left, Up** |
| **Optimal For...** | **Step count** (uniform cost) | **Arbitrary non-negative cost** | Not optimal (returns 1st path found) |
| **Returns** | Path or Failure | Minimum-cost path or Failure | `SUCCESS`, `CUTOFF`, or `FAILURE` |

---
*All the best for your Lab Test!* 🚀
