# 🎓 AI Lab Test Complete Preparation Guide (Semester 5)

> **Don't Panic!** Everything covered in your 3 labs is organized below into **crystal-clear explanations, short cheat sheets, viva Q&As, and easy-to-memorize code implementations in both C++ and Python**.

---

## 📌 Table of Contents
1. [Exam Quick Syllabus & Overview](#1-exam-quick-syllabus--overview)
2. [Core AI Theory & Viva Cheat Sheet](#2-core-ai-theory--viva-cheat-sheet)
3. [Universal 2D Grid Template](#3-universal-2d-grid-template)
4. [Lab 1: Problem Solving Using AI](#4-lab-1-problem-solving-using-ai)
   - [P1: Tic-Tac-Toe Winner Checker](#p1-tic-tac-toe-winner-checker)
   - [P2: Water Jug Problem (BFS)](#p2-water-jug-problem-bfs)
   - [P3: Smart Parking System (2D Grid BFS)](#p3-smart-parking-system-2d-grid-bfs)
5. [Lab 2: Intelligent Agent Architectures](#5-lab-2-intelligent-agent-architectures)
   - [P4: Smart Cleaning Robot (Simple Reflex vs. Model-Based)](#p4-smart-cleaning-robot-simple-reflex-vs-model-based)
   - [P5: Delivery Robot (Goal-Based BFS vs. Utility-Based Cost Search)](#p5-delivery-robot-goal-based-bfs-vs-utility-based-cost-search)
6. [Lab 3: Uninformed Search Methods](#6-lab-3-uninformed-search-methods)
   - [P6: Cost-Aware Delivery Robot (Uniform Cost Search - UCS)](#p6-cost-aware-delivery-robot-uniform-cost-search---ucs)
   - [P7: Rescue Robot (Depth-Limited Search - DLS)](#p7-rescue-robot-depth-limited-search---dls)
7. [Comparison of Search Algorithms (Exam Summary Table)](#7-comparison-of-search-algorithms-exam-summary-table)
8. [Common Exam Pitfalls & Tips](#8-common-exam-pitfalls--tips)

---

## 1. Exam Quick Syllabus & Overview

| Lab | Topic | Concepts | Algorithms |
| :--- | :--- | :--- | :--- |
| **Lab 1** | Problem Solving using AI | Matrix Representation, State Space Formulation | Direct Matrix Checks, Breadth-First Search (BFS) |
| **Lab 2** | Intelligent Agents | Simple Reflex, Model-Based Reflex, Goal-Based, Utility-Based | Priority Movement, Memory + BFS, Multi-Stage BFS, Dijkstra / Weighted Search |
| **Lab 3** | Uninformed Search | Path Cost Minimization, Depth Cutoffs | Uniform Cost Search (UCS / Min-Heap), Depth-Limited Search (DLS / Recursion) |

---

## 2. Core AI Theory & Viva Cheat Sheet

### A. What is an Intelligent Agent?
An **Agent** is anything that perceives its **Environment** through **Sensors** and acts upon that environment through **Actuators**.
- **Agent Function**: Abstract mathematical mapping from percept sequence to action: $f: P^* \to A$.
- **PEAS Framework**:
  - **P**erformance measure (Success criteria)
  - **E**nvironment (Workspace / Grid)
  - **A**ctuators (Motors / Wheels / Cleaners)
  - **S**ensors (Cameras / Grid Percepts)

### B. The 4 Agent Types (Lab 2 Core Focus)
```
1. Simple Reflex Agent:
   Percept ---> [Condition-Action Rules (IF-THEN)] ---> Action
   (NO memory of history. Can get stuck in infinite 2-cell loops!)

2. Model-Based Reflex Agent (Reflex with State):
   Percept ---> [Internal State / Memory of World] ---> Action
   (Remembers visited cells and cleaned status to avoid redundant moves.)

3. Goal-Based Agent:
   Percept ---> [Goal State Target (e.g. reach P then G)] ---> Action
   (Chooses actions to reach a specific destination; uses shortest-hop BFS.)

4. Utility-Based Agent:
   Percept ---> [Utility/Cost Function (Distance + Energy + Risk + Traffic)] ---> Action
   (Chooses the highest utility / lowest total cost route, even if it takes more steps!)
```

### C. Search Problem Formulation (5 Components)
1. **Initial State**: $S_0$ (e.g., $(0, 0)$ or $(0, 0)$ jug levels).
2. **Actions**: Available moves from current state (e.g., `Up, Down, Left, Right` or `Fill, Empty, Pour`).
3. **Transition Model**: `Result(s, a)` returns next state.
4. **Goal Test**: Check if current state satisfies target condition (e.g., `jug == 2` or `cell == 'G'`).
5. **Path Cost**: Sum of step costs along path $g(n)$.

---

## 3. Universal 2D Grid Template

All grid programs in Lab 1, Lab 2, and Lab 3 share the **exact same core template**:

### C++ Direction Arrays & Boundary Check:
```cpp
// 4 Direction Vectors: Up, Down, Left, Right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

bool isValid(int r, int c, int rows, int cols, const vector<vector<char>>& grid) {
    return (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] != 'X');
}
```

### Path Reconstruction (From Goal back to Start):
```cpp
vector<pair<int, int>> path;
pair<int, int> curr = goal;
while (curr != make_pair(-1, -1)) {
    path.push_back(curr);
    curr = parent[curr.first][curr.second];
}
reverse(path.begin(), path.end());
```

---

## 4. Lab 1: Problem Solving Using AI

---

### P1: Tic-Tac-Toe Winner Checker
- **Goal**: Read a 3x3 board matrix of `'X'`, `'O'`, and `'_'` and determine whether Player X won, Player O won, it is a Draw, or the Game is in progress.
- **Logic**:
  1. Check all 3 rows.
  2. Check all 3 columns.
  3. Check 2 diagonals (main diagonal `(0,0),(1,1),(2,2)` and anti-diagonal `(0,2),(1,1),(2,0)`).
  4. If no line of 3 matching symbols exists:
     - If any cell is `'_'`, return `"None"` (in progress).
     - Else return `"Draw"`.
- **Time Complexity**: $O(1)$ for a fixed 3x3 board ($O(N)$ lines checked for an $N \times N$ board).

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

string checkWinner(const vector<vector<char>>& b) {
    // Check 3 rows and 3 columns
    for (int i = 0; i < 3; i++) {
        if (b[i][0] != '_' && b[i][0] == b[i][1] && b[i][1] == b[i][2])
            return string(1, b[i][0]);
        if (b[0][i] != '_' && b[0][i] == b[1][i] && b[1][i] == b[2][i])
            return string(1, b[0][i]);
    }
    // Check 2 diagonals
    if (b[0][0] != '_' && b[0][0] == b[1][1] && b[1][1] == b[2][2])
        return string(1, b[0][0]);
    if (b[0][2] != '_' && b[0][2] == b[1][1] && b[1][1] == b[2][0])
        return string(1, b[0][2]);

    // Check for empty cell
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (b[i][j] == '_') return "None";

    return "Draw";
}

int main() {
    vector<vector<char>> board(3, vector<char>(3));
    cout << "Enter 3x3 board row by row (X, O, _):\n";
    for (int i = 0; i < 3; i++) {
        cout << "Row " << (i + 1) << ": ";
        for (int j = 0; j < 3; j++) cin >> board[i][j];
    }

    string res = checkWinner(board);
    if (res == "Draw") cout << "Result: The game is a Draw.\n";
    else if (res == "None") cout << "Result: No winner yet, game still in progress.\n";
    else cout << "Result: Player '" << res << "' wins!\n";
    return 0;
}
```

---

### P2: Water Jug Problem (BFS)
- **Problem**: 4-liter Jug $A$ and 3-liter Jug $B$. Target: Get exactly 2 liters in either jug.
- **6 Transition Rules from State $(x, y)$**:
  1. **Fill A**: $(a, y) = (4, y)$
  2. **Fill B**: $(x, b) = (x, 3)$
  3. **Empty A**: $(0, y)$
  4. **Empty B**: $(x, 0)$
  5. **Pour A to B**: Transfer amount $t = \min(x, b - y) \implies (x - t, y + t)$
  6. **Pour B to A**: Transfer amount $t = \min(y, a - x) \implies (x + t, y - t)$
- **Why BFS?** BFS explores states level-by-level and guarantees finding the **shortest sequence of steps**.

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;

using State = pair<int, int>; // (jugA, jugB)

vector<State> solveWaterJug(int a, int b, int c) {
    queue<State> q;
    map<State, State> parent;
    map<State, bool> visited;

    State start = {0, 0};
    q.push(start);
    visited[start] = true;
    State goal = {-1, -1};

    while (!q.empty()) {
        State curr = q.front();
        q.pop();
        int x = curr.first, y = curr.second;

        if (x == c || y == c) {
            goal = curr;
            break;
        }

        vector<State> nextStates = {
            {a, y},                                      // Fill A
            {x, b},                                      // Fill B
            {0, y},                                      // Empty A
            {x, 0},                                      // Empty B
            {x - min(x, b - y), y + min(x, b - y)},       // Pour A -> B
            {x + min(y, a - x), y - min(y, a - x)}        // Pour B -> A
        };

        for (auto& next : nextStates) {
            if (!visited[next]) {
                visited[next] = true;
                parent[next] = curr;
                q.push(next);
            }
        }
    }

    vector<State> path;
    if (goal.first != -1) {
        State curr = goal;
        while (curr != start) {
            path.push_back(curr);
            curr = parent[curr];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }
    return path;
}

int main() {
    int a = 4, b = 3, c = 2;
    vector<State> path = solveWaterJug(a, b, c);

    cout << "Water Jug Problem: " << a << "-li and " << b << "-li jugs\n\n";
    cout << left << setw(6) << "Step" << setw(12) << "4-li jug" << setw(12) << "3-li jug" << "\n";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << left << setw(6) << i << setw(12) << path[i].first << setw(12) << path[i].second << "\n";
    }
    cout << "\nGoal reached: " << c << " liters obtained in " << (path.size() - 1) << " steps.\n";
    return 0;
}
```

---

### P3: Smart Parking System (2D Grid BFS)
- **Problem**: 2D Grid with:
  - `E`: Entrance (Start)
  - `A`: Available Parking Space (Goal)
  - `O`: Occupied space (Obstacle)
  - `X`: Wall / Blocked (Obstacle)
  - `R`: Road (Traversable)
- **Why BFS?** Since each step has equal unit cost, BFS finds the **nearest** reachable available parking spot `'A'`.

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int main() {
    vector<string> demo = {
        "E R R X A X X R R R",
        "R X R R R X R X R R",
        "A R R X O R R O R A",
        "R R X R R R X R R R",
        "X R R R X R R R X R",
        "R R O R R X R R R R",
        "R X R R R R X R O R",
        "A R R X R R R R R A",
        "R R R R O R R X R R",
        "X R R R R R R R A R"
    };

    int rows = 10, cols = 10;
    vector<vector<char>> grid(rows, vector<char>(cols));
    int startR = -1, startC = -1;

    for (int i = 0; i < rows; i++) {
        int col = 0;
        for (char ch : demo[i]) {
            if (ch != ' ') {
                grid[i][col] = ch;
                if (ch == 'E') { startR = i; startC = col; }
                col++;
            }
        }
    }

    queue<pair<int, int>> q;
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));

    q.push({startR, startC});
    visited[startR][startC] = true;
    pair<int, int> goal = {-1, -1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (grid[r][c] == 'A') {
            goal = {r, c};
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (!visited[nr][nc] && grid[nr][nc] != 'X' && grid[nr][nc] != 'O') {
                    visited[nr][nc] = true;
                    parent[nr][nc] = {r, c};
                    q.push({nr, nc});
                }
            }
        }
    }

    if (goal.first == -1) {
        cout << "No reachable parking space found.\n";
        return 0;
    }

    vector<pair<int, int>> path;
    pair<int, int> curr = goal;
    while (curr != make_pair(-1, -1)) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());

    cout << "Nearest available parking space: (" << goal.first << ", " << goal.second << ")\n";
    cout << "Number of movements required: " << (path.size() - 1) << "\n";
    cout << "Route: ";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << "(" << path[i].first << ", " << path[i].second << ")";
        if (i + 1 < (int)path.size()) cout << " -> ";
    }
    cout << "\n";
    return 0;
}
```

---

## 5. Lab 2: Intelligent Agent Architectures

---

### P4: Smart Cleaning Robot (Simple Reflex vs. Model-Based)
- **Environment**: Grid with `S` (Start), `D` (Dirty), `C` (Clean), `X` (Wall).
- **Comparison**:
  - **Simple Reflex Agent**:
    - **Rule**: If current cell is `'D'` $\to$ **CLEAN** (`'C'`).
    - Otherwise, move in fixed priority order: **Up $\to$ Right $\to$ Down $\to$ Left**.
    - **Key Flaw**: Has NO memory of past visits. It easily gets stuck oscillating back and forth between two cells and has high repeated visits!
  - **Model-Based Reflex Agent**:
    - **Rule**: If current cell is `'D'` $\to$ **CLEAN** (`'C'`).
    - Uses internal state (visited map + BFS) to find shortest path to the **nearest remaining dirty cell**.
    - **Result**: Much fewer movements, fewer repeated visits, and reliably cleans all reachable dirty cells.

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
using namespace std;

const int ROWS = 10, COLS = 10;
int dr[] = {-1, 0, 1, 0}; // Up, Right, Down, Left
int dc[] = {0, 1, 0, -1};

int countDirty(const vector<vector<char>>& g) {
    int cnt = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (g[i][j] == 'D') cnt++;
    return cnt;
}

// 1. Simple Reflex Agent
void runSimpleReflex(vector<vector<char>> g, int r, int c) {
    int totalDirty = countDirty(g), cleaned = 0, mov = 0, actions = 0;
    map<pair<int, int>, int> visits;
    visits[{r, c}] = 1;

    for (int step = 0; step < 1000 && cleaned < totalDirty; step++) {
        if (g[r][c] == 'D') {
            g[r][c] = 'C';
            cleaned++; actions++;
            continue;
        }
        bool moved = false;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && g[nr][nc] != 'X') {
                r = nr; c = nc;
                mov++; actions++;
                visits[{r, c}]++;
                moved = true;
                break;
            }
        }
        if (!moved) break;
    }

    int repeated = 0;
    for (auto& [pt, cnt] : visits) if (cnt > 1) repeated += (cnt - 1);

    cout << "--- Simple Reflex Agent ---\n";
    cout << "Dirty cells cleaned: " << cleaned << "\nMovements: " << mov
         << "\nTotal actions: " << actions << "\nRepeated visits: " << repeated << "\n\n";
}

// Helper: BFS for Model-Based Agent to find nearest 'D'
vector<pair<int, int>> findNearestDirty(const vector<vector<char>>& g, int sR, int sC) {
    queue<pair<int, int>> q;
    vector<vector<bool>> vis(ROWS, vector<bool>(COLS, false));
    vector<vector<pair<int, int>>> parent(ROWS, vector<pair<int, int>>(COLS, {-1, -1}));

    q.push({sR, sC});
    vis[sR][sC] = true;
    pair<int, int> target = {-1, -1};

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (g[r][c] == 'D') { target = {r, c}; break; }

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && g[nr][nc] != 'X' && !vis[nr][nc]) {
                vis[nr][nc] = true;
                parent[nr][nc] = {r, c};
                q.push({nr, nc});
            }
        }
    }
    if (target.first == -1) return {};

    vector<pair<int, int>> path;
    pair<int, int> curr = target;
    while (curr != make_pair(sR, sC)) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());
    return path;
}

// 2. Model-Based Reflex Agent
void runModelBased(vector<vector<char>> g, int r, int c) {
    int totalDirty = countDirty(g), cleaned = 0, mov = 0, actions = 0;
    map<pair<int, int>, int> visits;
    visits[{r, c}] = 1;

    while (cleaned < totalDirty) {
        if (g[r][c] == 'D') {
            g[r][c] = 'C';
            cleaned++; actions++;
            continue;
        }
        auto path = findNearestDirty(g, r, c);
        if (path.empty()) break;

        r = path[0].first; c = path[0].second;
        mov++; actions++;
        visits[{r, c}]++;
    }

    int repeated = 0;
    for (auto& [pt, cnt] : visits) if (cnt > 1) repeated += (cnt - 1);

    cout << "--- Model-Based Reflex Agent ---\n";
    cout << "Dirty cells cleaned: " << cleaned << "\nMovements: " << mov
         << "\nTotal actions: " << actions << "\nRepeated visits: " << repeated << "\n\n";
}
```

---

### P5: Delivery Robot (Goal-Based vs. Utility-Based)
- **Problem**: Robot must navigate from Start (`S`) $\to$ Package (`P`) $\to$ Goal (`G`).
- **Goal-Based Agent**: Uses BFS to find the path with the **minimum number of movements** (step count).
- **Utility-Based Agent**: Uses Uniform Cost Search / Dijkstra to find the path with the **minimum total cost** using the multi-attribute cost function:
  $$\text{Cost} = 0.4 \times \text{Distance} + 0.3 \times \text{Energy} + 0.2 \times \text{Risk} + 0.1 \times \text{Traffic}$$
  - **Key Observation**: The Utility-Based Agent may choose a route with **more physical steps** to avoid high-risk or congested road shortcuts!

#### C++ Cost Function & Search:
```cpp
double getCellCost(int r, int c) {
    double dist = 1.0, energy = 1.0, risk = 1.0, traffic = 1.0;
    if ((r == 2 && c == 7) || (r == 2 && c == 6)) { // Congested shortcut
        energy = 5.0; risk = 4.0; traffic = 3.0;
    }
    return (0.4 * dist) + (0.3 * energy) + (0.2 * risk) + (0.1 * traffic);
}
```

---

## 6. Lab 3: Uninformed Search Methods

---

### P6: Cost-Aware Delivery Robot (Uniform Cost Search - UCS)
- **Concept**: Explores nodes in increasing order of accumulated path cost $g(n)$ using a **Min-Heap Priority Queue** (`std::priority_queue<..., greater>`).
- **Terrain Costs**:
  - `S` = 0 (Start)
  - `G` = 1 (Goal)
  - `R` = 1 (Normal road)
  - `M` = 3 (Moderate/rough)
  - `T` = 5 (Traffic congested)
  - `X` = Blocked
- **Optimality**: UCS guarantees optimal cost if all step costs $c \ge \epsilon > 0$.

#### C++ Core UCS Loop:
```cpp
using Node = pair<int, pair<int, int>>; // (g(n), (r, c))
priority_queue<Node, vector<Node>, greater<Node>> pq;
vector<vector<int>> gCost(ROWS, vector<int>(COLS, 1e9));

pq.push({0, start});
gCost[start.first][start.second] = 0;

while (!pq.empty()) {
    auto [cost, curr] = pq.top(); pq.pop();
    int r = curr.first, c = curr.second;

    if (expanded[r][c]) continue; // Skip stale entries
    expanded[r][c] = true;
    expansionOrder.push_back(curr);

    if (curr == goal) break;

    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i], nc = c + dc[i];
        if (isValid(nr, nc)) {
            int newCost = cost + getCost(grid[nr][nc]);
            if (newCost < gCost[nr][nc]) {
                gCost[nr][nc] = newCost;
                parent[nr][nc] = {r, c};
                pq.push({newCost, {nr, nc}});
            }
        }
    }
}
```

---

### P7: Rescue Robot (Depth-Limited Search - DLS)
- **Concept**: Depth-First Search with a hard depth cutoff $L$.
- **Fixed Successor Order**: **Right $\to$ Down $\to$ Left $\to$ Up**.
- **Three Possible Returns**:
  1. `SUCCESS`: Goal $G$ found at depth $\le L$.
  2. `CUTOFF`: Reached depth $L$ without reaching $G$ (solution lies deeper).
  3. `FAILURE`: Search space completely exhausted without finding $G$.
- **Two Lab Test Runs**:
  - $L = 18 \implies$ **CUTOFF** (True shortest route needs 22 steps).
  - $L = 25 \implies$ **SUCCESS** (Found path in 22 steps).

#### C++ DLS Recursive Function:
```cpp
const int SUCCESS = 1, CUTOFF = 0, FAILURE = -1;

int dls(const vector<vector<char>>& grid, Point curr, Point goal, int depth, int limit, vector<Point>& path) {
    expansionOrder.push_back(curr);
    maxDepthReached = max(maxDepthReached, depth);

    if (curr == goal) { solutionPath = path; return SUCCESS; }
    if (depth == limit) return CUTOFF;

    bool cutoffOccurred = false;
    for (int i = 0; i < 4; i++) {
        int nr = curr.first + dr[i], nc = curr.second + dc[i];
        if (isValid(nr, nc) && !onPath[nr][nc]) {
            onPath[nr][nc] = true;
            path.push_back({nr, nc});

            int res = dls(grid, {nr, nc}, goal, depth + 1, limit, path);

            path.pop_back();
            onPath[nr][nc] = false;

            if (res == SUCCESS) return SUCCESS;
            if (res == CUTOFF) cutoffOccurred = true;
        }
    }
    return cutoffOccurred ? CUTOFF : FAILURE;
}
```

---

## 7. Comparison of Search Algorithms (Exam Summary Table)

| Criterion | BFS | DFS | UCS (Dijkstra) | DLS |
| :--- | :--- | :--- | :--- | :--- |
| **Frontier Structure** | Queue (FIFO) | Stack (LIFO) / Recursion | Min-Heap Priority Queue | Recursion Stack |
| **Expansion Criterion** | Shallowest node first | Deepest node first | Lowest path cost $g(n)$ first | Deepest up to limit $L$ |
| **Completeness?** | Yes (if $b < \infty$) | No (can loop in cycles) | Yes (if step cost $\ge \epsilon > 0$) | Yes if $L \ge d$, No if $L < d$ |
| **Cost Optimal?** | Yes (only if step costs equal) | No | **Yes (always optimal for $c \ge 0$)** | No (returns 1st path found) |
| **Time Complexity** | $O(b^d)$ | $O(b^m)$ | $O(b^{1 + \lfloor C^* / \epsilon \rfloor})$ | $O(b^L)$ |
| **Space Complexity** | $O(b^d)$ (Exponential - High) | $O(b \cdot m)$ (Linear - Low) | $O(b^{1 + \lfloor C^* / \epsilon \rfloor})$ | $O(b \cdot L)$ (Linear - Low) |

*Notations: $b$ = branching factor, $d$ = goal depth, $m$ = max depth, $L$ = depth limit, $C^*$ = optimal path cost.*

---

## 8. Expected Viva / Discussion Questions & Exact Answers

### Q1. Why can DLS fail even when a valid path to the goal exists?
> **Answer**: DLS cuts off exploration at depth $L$. If all paths from $S$ to $G$ require more than $L$ moves (e.g., $L = 18$ while the true path is 22 moves), every branch hits `CUTOFF` before reaching $G$.

### Q2. How does increasing limit $L$ affect time and memory in DLS?
> **Answer**:
> - **Time**: Increases exponentially in worst case ($O(b^L)$) because deeper tree levels are searched.
> - **Memory**: Grows only **linearly** ($O(b \cdot L)$) because only the active recursion path and its siblings are stored.

### Q3. Is DLS guaranteed to return the shortest path?
> **Answer**: **No.** DLS is depth-first and stops at the very **first** goal path it encounters under its fixed successor order (`Right -> Down -> Left -> Up`). It does not compare alternative paths to check if a shorter one exists.

### Q4. Why does a Simple Reflex Agent oscillate between cells?
> **Answer**: Because it has **no memory (state)**. Once it cleans a dirty cell, the cell becomes clean. At the next step, based on its fixed rule priority (e.g., Up, Right, Down, Left), it moves back into the cell it just came from, creating an infinite loop between adjacent clean cells.

### Q5. Why does UCS use a Priority Queue instead of a simple FIFO Queue?
> **Answer**: A FIFO queue only guarantees shortest paths when all step costs are equal (1 unit). When terrain costs vary (e.g., Normal road = 1, Congested road = 5), a priority queue ensures the node with the **lowest accumulated cost $g(n)$** is always expanded first.

---

## 9. Common Exam Pitfalls & Tips

1. **Matrix Indexing**:
   - `grid[row][col]` where `row` is vertical (`y`) and `col` is horizontal (`x`).
   - Move Up: `r - 1, c`
   - Move Down: `r + 1, c`
   - Move Left: `r, c - 1`
   - Move Right: `r, c + 1`
2. **Cycle Prevention in DLS**:
   - Always add `onPath[r][c] = true` before recursion, and **remove** it (`onPath[r][c] = false`) after returning (Backtracking).
3. **Array vs. Coordinate Display**:
   - Output coordinates as `(row, col)`.
   - Remember: `Movements = Number of nodes in path - 1`.

---
*Good luck with your Lab Test tomorrow! You have everything you need.* 🚀
