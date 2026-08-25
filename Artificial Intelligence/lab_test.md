# 🎓 AI Lab Test Complete Preparation Guide (Semester 5)

> **Don't Panic!** Everything covered in your 3 labs is organized below into **crystal-clear explanations, short cheat sheets, viva Q&As, and your updated code implementations**.

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
8. [Expected Viva / Discussion Questions & Exact Answers](#8-expected-viva--discussion-questions--exact-answers)
9. [Common Exam Pitfalls & Tips](#9-common-exam-pitfalls--tips)

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
  - **P**erformance measure (Success criteria, e.g. dirty cells cleaned, total cost minimized)
  - **E**nvironment (Workspace / 2D Grid)
  - **A**ctuators (Motors / Wheels / Cleaning mechanism)
  - **S**ensors (Grid percepts / Coordinate detectors)

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
4. **Goal Test**: Check if current state satisfies target condition (e.g., `x == c || y == c` or `grid[r][c] == 'G'`).
5. **Path Cost**: Sum of step costs along path $g(n)$.

---

## 3. Universal 2D Grid Template

All grid programs in Lab 1, Lab 2, and Lab 3 share the **exact same core template**:

### Direction Vectors & Boundary Check:
```cpp
const int N = 10;
using Point = pair<int, int>;

// Direction Moves: Up, Right, Down, Left
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

// Check if cell is valid and not an obstacle
bool isValid(int nr, int nc, const vector<vector<char>>& grid) {
    return (nr >= 0 && nr < N && nc >= 0 && nc < N && grid[nr][nc] != 'X');
}
```

### Path Reconstruction (From Goal back to Start):
```cpp
vector<Point> path;
Point curr = goal;
while (curr != start) {
    path.push_back(curr);
    curr = parent[curr.first][curr.second];
}
path.push_back(start);
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

void printBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < 3; i++) {
        cout << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << "\n";
        cout << "---------\n";
    }
} // just printing all values in the 2D vector board

string checkWinner(const vector<vector<char>>& board) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != '_' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return string(1, board[i][0]);
        if (board[0][i] != '_' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return string(1, board[0][i]);
    }
    // Check diagonals
    if (board[0][0] != '_' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return string(1, board[0][0]);
    if (board[0][2] != '_' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return string(1, board[0][2]);

    // Check for empty cells (game still running)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') return "None";
        }
    }

    return "Draw";
} // checking if there is a winner or if the game is still running or if it is a draw

int main() {
    cout << "Enter the 3x3 board row by row.\n";
    cout << "Use 'X', 'O', or '_' for empty cells, separated by spaces:\n";

    vector<vector<char>> board(3, vector<char>(3));
    for (int i = 0; i < 3; i++) {
        cout << "Row " << (i + 1) << ": ";
        for (int j = 0; j < 3; j++) {
            cin >> board[i][j];
        }
    }

    cout << "\nBoard:\n";
    printBoard(board);

    string result = checkWinner(board);
    if (result == "Draw") {
        cout << "Result: The game is a Draw.\n";
    } else if (result == "None") {
        cout << "Result: No winner yet, game still in progress.\n";
    } else {
        cout << "Result: Player '" << result << "' wins!\n";
    }

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
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

vector<pair<int,int>> solve(int a, int b, int c) {
    queue<pair<int,int>> q;                   // queue to perform BFS
    map<pair<int,int>, pair<int,int>> parent; // stores parent to reconstruct path
    map<pair<int,int>, bool> vis;             // keeps track of visited states

    pair<int,int> start = {0, 0};             // starting state (both empty)
    q.push(start); 
    vis[start] = true;

    pair<int,int> goal = {-1, -1};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        if (x == c || y == c) {
            goal = {x, y};
            break;
        }

        vector<pair<int,int>> next = {
            {a, y},                             // Fill A
            {x, b},                             // Fill B
            {0, y},                             // Empty A
            {x, 0},                             // Empty B
            {x - min(x, b-y), y + min(x, b-y)}, // Pour A -> B
            {x + min(y, a-x), y - min(y, a-x)}  // Pour B -> A
        };

        for (auto s : next) {
            if (!vis[s]) {
                vis[s] = true;
                parent[s] = {x, y};
                q.push(s);
            }
        }
    }

    vector<pair<int,int>> path;
    if (goal.first == -1)
        return path;

    for (pair<int,int> cur = goal; cur != start; cur = parent[cur])
        path.push_back(cur);

    path.push_back(start);
    reverse(path.begin(), path.end());

    return path;
}

int main() {
    int a = 4, b = 3, c = 2;

    auto path = solve(a, b, c);

    if (path.empty()) {
        cout << "No solution\n";
        return 0;
    }

    for (auto [x, y] : path)
        cout << "(" << x << ", " << y << ")\n";

    cout << "Steps: " << path.size() - 1 << '\n';
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

// Direction moves: Up, Down, Left, Right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int main() {
    cout << "Smart Parking System (BFS)\n";
    cout << "1. Use demo 10x10 grid\n";
    cout << "2. Enter custom grid\n";
    cout << "Choose an option (1/2): ";
    
    int choice;
    if (!(cin >> choice)) choice = 1;

    int rows = 10, cols = 10;
    vector<vector<char>> grid;

    if (choice == 1) {
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
        }; // R=Road, X=Obstacle, A=Available, O=Occupied, E=Entrance
        for (auto& line : demo) {
            vector<char> row;
            for (char ch : line) {
                if (ch != ' ') row.push_back(ch);
            }
            grid.push_back(row);
        }
    } else {
        cout << "Enter Rows and Columns: ";
        cin >> rows >> cols;
        grid.assign(rows, vector<char>(cols));
        cout << "Enter grid elements:\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cin >> grid[i][j];
            }
        }
    }

    // Display Grid
    cout << "Parking Grid:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << grid[i][j] << (j + 1 < cols ? " " : "");
        }
        cout << "\n";
    }
    cout << "\n";

    // Locate entrance 'E'
    int startR = -1, startC = -1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 'E') {
                startR = i;
                startC = j;
            }
        }
    }

    if (startR == -1) {
        cout << "Error: No entrance 'E' found!\n";
        return 0;
    }

    // BFS to find nearest available parking spot 'A'
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));

    q.push({startR, startC});
    visited[startR][startC] = true;

    pair<int, int> goal = {-1, -1};

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        int r = curr.first;
        int c = curr.second;

        // If available parking spot reached
        if (grid[r][c] == 'A') {
            goal = {r, c};
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Valid bounds & traversable cell ('R', 'A', 'E')
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
        cout << "No reachable available parking space found.\n";
        return 0;
    }

    // Reconstruct path
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
#include <queue>
#include <algorithm>
using namespace std;

const int N = 10;

// Movement priority:
// Up -> Right -> Down -> Left
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

// Count how many dirty cells are present
int countDirty(vector<vector<char>>& grid) {
    int count = 0;

    for (auto &row : grid)
        for (char cell : row)
            if (cell == 'D')
                count++;

    return count;
}

/*
    1. SIMPLE REFLEX AGENT

    Idea:
    - Agent only looks at the CURRENT cell.
    - If current cell is dirty -> clean it.
    - Otherwise -> move using fixed priority:
      Up, Right, Down, Left.
    - It does NOT remember visited cells.
    - It does NOT plan a path.

    This can cause unnecessary/repeated movements.
*/
void simpleReflex(vector<vector<char>> grid, int r, int c) {

    int totalDirty = countDirty(grid);
    int cleaned = 0;
    int moves = 0;

    while (cleaned < totalDirty) {

        // Rule 1: If current cell is dirty, clean it
        if (grid[r][c] == 'D') {
            grid[r][c] = 'C';
            cleaned++;
            continue;
        }

        // Rule 2: If cell is clean, move in fixed priority
        bool moved = false;

        for (int i = 0; i < 4; i++) {

            int nr = r + dr[i];
            int nc = c + dc[i];

            // Move only if:
            // 1. Inside the grid
            // 2. Not an obstacle ('X')
            if (nr >= 0 && nr < N &&
                nc >= 0 && nc < N &&
                grid[nr][nc] != 'X') {

                r = nr;
                c = nc;
                moves++;

                moved = true;
                break;
            }
        }

        // Stop if no valid movement is possible
        if (!moved)
            break;
    }

    cout << "--- Simple Reflex Agent ---\n";
    cout << "Dirty cells cleaned: " << cleaned << '\n';
    cout << "Movements: " << moves << "\n\n";
}


/*
    BFS HELPER

    Goal:
    Find the SHORTEST PATH from the current position
    to the NEAREST dirty cell.

    BFS explores cells level by level:

        Start
          |
       distance 1
          |
       distance 2
          |
       distance 3 ...

    Therefore, the first dirty cell found is the
    nearest reachable dirty cell.
*/
vector<pair<int,int>> bfs(vector<vector<char>>& grid,
                          int sr, int sc) {

    queue<pair<int,int>> q;

    // visited prevents visiting the same cell again
    bool visited[N][N] = {};

    // parent[x][y] stores the previous cell
    // from which we reached (x,y)
    pair<int,int> parent[N][N];

    q.push({sr, sc});
    visited[sr][sc] = true;

    pair<int,int> target = {-1, -1};

    while (!q.empty()) {

        auto [r, c] = q.front();
        q.pop();

        // If this cell is dirty, we found our target
        if (grid[r][c] == 'D') {
            target = {r, c};
            break;
        }

        // Try all 4 directions
        for (int i = 0; i < 4; i++) {

            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < N &&
                nc >= 0 && nc < N &&
                grid[nr][nc] != 'X' &&
                !visited[nr][nc]) {

                visited[nr][nc] = true;

                // Remember where we came from
                parent[nr][nc] = {r, c};

                q.push({nr, nc});
            }
        }
    }

    // No reachable dirty cell
    if (target.first == -1)
        return {};

    /*
        Reconstruct path.

        Example:

        S -> A -> B -> D

        parent[D] = B
        parent[B] = A
        parent[A] = S

        So we go backwards and then reverse the path.
    */
    vector<pair<int,int>> path;

    while (target != make_pair(sr, sc)) {
        path.push_back(target);
        target = parent[target.first][target.second];
    }

    reverse(path.begin(), path.end());

    return path;
} // returns path from current position to nearest dirty cell, empty if no dirty cell is reachable


/*
    2. MODEL-BASED REFLEX AGENT

    Idea:
    - Agent maintains information about the environment.
    - Instead of blindly moving, it searches for
      the nearest dirty cell.
    - BFS is used to find the shortest path.
    - Agent then moves one step along that path.
    - Repeat until all reachable dirty cells are cleaned.
*/
void modelBased(vector<vector<char>> grid, int r, int c) {

    int totalDirty = countDirty(grid);
    int cleaned = 0;
    int moves = 0;

    while (cleaned < totalDirty) {

        // If current cell is dirty -> clean it
        if (grid[r][c] == 'D') {
            grid[r][c] = 'C';
            cleaned++;
            continue;
        }

        // Find shortest path to nearest dirty cell
        vector<pair<int,int>> path = bfs(grid, r, c);

        // No dirty cell is reachable
        if (path.empty())
            break;

        // Move only ONE step along the planned path
        r = path[0].first;
        c = path[0].second;

        moves++;
    }

    cout << "--- Model-Based Reflex Agent ---\n";
    cout << "Dirty cells cleaned: " << cleaned << '\n';
    cout << "Movements: " << moves << "\n\n";

    // Display final grid
    cout << "--- Final Grid ---\n";

    for (auto &row : grid) {
        for (char cell : row)
            cout << cell << ' ';

        cout << '\n';
    }
}


int main() {

    /*
        GRID REPRESENTATION

        S = Starting position
        C = Clean cell
        D = Dirty cell
        X = Obstacle

        Agent must clean all reachable D cells
        while avoiding X cells.
    */
    vector<vector<char>> grid = {

        {'S','C','D','C','X','C','C','D','C','C'},
        {'C','X','C','C','C','C','X','C','D','C'},
        {'D','C','C','X','D','C','C','C','C','C'},
        {'C','C','X','C','C','C','D','X','C','C'},
        {'C','D','C','C','X','C','C','C','C','D'},
        {'C','C','C','D','C','C','X','C','C','C'},
        {'X','C','C','C','C','D','C','C','X','C'},
        {'C','C','D','X','C','C','C','D','C','C'},
        {'C','X','C','C','D','C','C','C','C','C'},
        {'D','C','C','C','C','X','C','C','D','C'}
    };

    // Starting position of the agent
    int startRow = 0;
    int startCol = 0;

    // Run both AI agents on the same environment
    simpleReflex(grid, startRow, startCol);

    modelBased(grid, startRow, startCol);

    return 0;
}
```

---

### P5: Delivery Robot (Goal-Based BFS vs. Utility-Based Cost Search)
- **Problem**: Robot starts at $S$, picks up package at $P$, delivers to destination $G$.
- **Goal-Based Agent**: Uses BFS ($S \to P$ then $P \to G$) to minimize **movement steps**.
- **Utility-Based Agent**: Uses UCS/Dijkstra to minimize **total utility cost**:
  $$\text{Cost} = 0.4 \times \text{Distance} + 0.3 \times \text{Energy} + 0.2 \times \text{Risk} + 0.1 \times \text{Traffic}$$
  - Special shortcut cells $(2,6)$ and $(2,7)$ have higher energy (5), risk (4), traffic (3).
  - **Key Observation**: Utility-based agent may take more physical steps if it results in a cheaper, safer route.

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

const int N = 10;

using Point = pair<int, int>;

// Movement: Up, Right, Down, Left
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};


/*
    Convert a path into readable form.

    Example:
    (0,0) -> (0,1) -> (1,1) -> ...
*/
void printPath(const vector<Point>& path) {

    if (path.empty()) {
        cout << "No Path Found";
        return;
    }

    for (int i = 0; i < path.size(); i++) {

        cout << "(" << path[i].first
             << "," << path[i].second << ")";

        if (i + 1 < path.size())
            cout << " -> ";
    }
}


/*
    GOAL-BASED AGENT
    ----------------

    Goal:
    Reach a specific destination.

    Here:
        S = Start
        P = Package
        G = Destination

    The agent first goes:
        S -> P

    Then:
        P -> G

    BFS is used because every movement has the same cost.

    BFS gives the SHORTEST PATH
    in terms of number of movements.
*/
vector<Point> bfs(const vector<vector<char>>& grid,
                  Point start,
                  Point goal) {

    queue<Point> q;

    // Stores whether a cell has already been visited
    bool visited[N][N] = {};

    // Stores the previous cell in the path
    Point parent[N][N];

    q.push(start);
    visited[start.first][start.second] = true;

    while (!q.empty()) {

        Point curr = q.front();
        q.pop();

        int r = curr.first;
        int c = curr.second;

        // Goal reached
        if (curr == goal)
            break;

        // Try all 4 directions
        for (int i = 0; i < 4; i++) {

            int nr = r + dr[i];
            int nc = c + dc[i];

            // Valid cell and not an obstacle
            if (nr >= 0 && nr < N &&
                nc >= 0 && nc < N &&
                grid[nr][nc] != 'X' &&
                !visited[nr][nc]) {

                visited[nr][nc] = true;

                // Remember where we came from
                parent[nr][nc] = {r, c};

                q.push({nr, nc});
            }
        }
    }

    // Goal cannot be reached
    if (!visited[goal.first][goal.second])
        return {};

    /*
        Reconstruct path.

        Start from goal and keep going to parent
        until we reach the starting cell.
    */
    vector<Point> path;

    Point curr = goal;

    while (curr != start) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }

    // Add starting point
    path.push_back(start);

    // Currently path is G -> ... -> S
    // Reverse it to get S -> ... -> G
    reverse(path.begin(), path.end());

    return path;
}


/*
    CELL COST
    ---------

    Utility-Based Agent does not only care about
    number of movements.

    Every cell has a cost:

        Cost =
        0.4 * Distance
        + 0.3 * Energy
        + 0.2 * Risk
        + 0.1 * Traffic

    Normal cell:
        Distance = 1
        Energy   = 1
        Risk     = 1
        Traffic  = 1

    Special cells (2,6) and (2,7):
        Higher energy, risk and traffic
        -> Therefore they are expensive.
*/
double cellCost(int r, int c) {

    double distance = 1;
    double energy = 1;
    double risk = 1;
    double traffic = 1;

    // High-cost / risky cells
    if ((r == 2 && c == 6) ||
        (r == 2 && c == 7)) {

        energy = 5;
        risk = 4;
        traffic = 3;
    }

    return 0.4 * distance +
           0.3 * energy +
           0.2 * risk +
           0.1 * traffic;
}


/*
    UTILITY-BASED AGENT
    -------------------

    Goal:
    Find the path with the LOWEST TOTAL COST.

    We use Uniform Cost Search (UCS).

    UCS is basically Dijkstra's algorithm when
    all edge costs are non-negative.

    Important:

    BFS:
        Minimizes number of movements.

    UCS:
        Minimizes total cost.

    Therefore UCS may take MORE steps if that
    results in a cheaper/safer route.
*/
pair<vector<Point>, double> ucs(
    const vector<vector<char>>& grid,
    Point start,
    Point goal) {

    // (cost, position)
    using Node = pair<double, Point>;

    // Always process the lowest-cost node first
    priority_queue<Node,
                   vector<Node>,
                   greater<Node>> pq;

    // Minimum cost required to reach each cell
    double dist[N][N];

    // Parent array for path reconstruction
    Point parent[N][N];

    // Initialize distances
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            dist[i][j] = 1e9;

    // Starting cell has cost 0
    dist[start.first][start.second] = 0;

    pq.push({0, start});

    while (!pq.empty()) {

        auto [cost, curr] = pq.top();
        pq.pop();

        int r = curr.first;
        int c = curr.second;

        // Ignore outdated priority queue entries
        if (cost > dist[r][c])
            continue;

        // Goal reached
        if (curr == goal)
            break;

        // Try all 4 directions
        for (int i = 0; i < 4; i++) {

            int nr = r + dr[i];
            int nc = c + dc[i];

            // Valid and not obstacle
            if (nr >= 0 && nr < N &&
                nc >= 0 && nc < N &&
                grid[nr][nc] != 'X') {

                // Cost of reaching new cell
                double newCost =
                    cost + cellCost(nr, nc);

                // Found a cheaper route
                if (newCost < dist[nr][nc]) {

                    dist[nr][nc] = newCost;

                    // Store parent for path reconstruction
                    parent[nr][nc] = {r, c};

                    pq.push({newCost, {nr, nc}});
                }
            }
        }
    }

    // Goal cannot be reached
    if (dist[goal.first][goal.second] == 1e9)
        return {{}, 1e9};


    // Reconstruct path
    vector<Point> path;

    Point curr = goal;

    while (curr != start) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }

    path.push_back(start);

    // Reverse: S -> ... -> G
    reverse(path.begin(), path.end());

    return {path, dist[goal.first][goal.second]};
}


int main() {

    /*
        GRID

        S = Start
        P = Package
        G = Destination
        R = Normal road
        X = Obstacle

        Task:

        1. Start at S
        2. Pick up package at P
        3. Deliver package to G

        Two agents solve this problem differently:

        Goal-Based:
            Find shortest route.

        Utility-Based:
            Find cheapest route.
    */

    vector<vector<char>> grid = {

        {'S','R','R','X','R','R','R','R','R','R'},
        {'X','X','R','R','R','X','R','X','R','R'},
        {'R','R','R','X','R','R','R','P','R','R'},
        {'R','X','R','R','R','X','R','R','R','X'},
        {'R','R','R','X','R','R','X','R','R','R'},
        {'X','R','R','R','R','R','R','R','X','R'},
        {'R','R','X','R','X','R','R','R','R','R'},
        {'R','R','R','R','R','X','R','X','R','R'},
        {'R','X','R','R','R','R','R','R','R','R'},
        {'R','R','R','X','R','R','R','R','X','G'}
    };

    Point S = {0, 0};
    Point P = {2, 7};
    Point G = {9, 9};


    /*
        1. GOAL-BASED AGENT

        Find:
            S -> P
            P -> G

        BFS gives shortest number of movements.
    */

    vector<Point> path1 = bfs(grid, S, P);
    vector<Point> path2 = bfs(grid, P, G);

    int moves1 = path1.empty() ? 0 : path1.size() - 1;
    int moves2 = path2.empty() ? 0 : path2.size() - 1;

    cout << "--- Goal-Based Agent ---\n";

    cout << "Route to Package: ";
    printPath(path1);
    cout << "\n";

    cout << "Movements to Package: "
         << moves1 << "\n";

    cout << "Route to Destination: ";
    printPath(path2);
    cout << "\n";

    cout << "Movements to Destination: "
         << moves2 << "\n";

    cout << "Total Movements: "
         << moves1 + moves2 << "\n\n";


    /*
        2. UTILITY-BASED AGENT

        UCS finds the path with minimum TOTAL COST.

        It may use more movements than BFS,
        but it can avoid expensive/risky cells.
    */

    auto result1 = ucs(grid, S, P);
    auto result2 = ucs(grid, P, G);

    vector<Point> ucsPath1 = result1.first;
    vector<Point> ucsPath2 = result2.first;

    double cost1 = result1.second;
    double cost2 = result2.second;

    int ucsMoves =
        (ucsPath1.size() - 1) +
        (ucsPath2.size() - 1);

    double totalCost = cost1 + cost2;


    cout << "--- Utility-Based Agent ---\n";

    cout << "Route to Package: ";
    printPath(ucsPath1);
    cout << "\n";

    cout << "Route to Destination: ";
    printPath(ucsPath2);
    cout << "\n";

    cout << "Total Movements: "
         << ucsMoves << "\n";

    cout << fixed << setprecision(1);

    cout << "Total Cost: "
         << totalCost << "\n";

    return 0;
}
```

---

## 6. Lab 3: Uninformed Search Methods

---

### P6: Cost-Aware Delivery Robot (Uniform Cost Search - UCS)
- **Concept**: Explores nodes in increasing order of accumulated path cost $g(n)$ using a **Min-Heap Priority Queue** (`std::priority_queue<Node, vector<Node>, greater<Node>>`).
- **Terrain Entry Costs**:
  - `S` = 0 (Start)
  - `G` = 1 (Goal)
  - `R` = 1 (Normal road)
  - `M` = 3 (Mud / rough surface)
  - `T` = 5 (Traffic congested)
  - `X` = Blocked
- **Optimality**: UCS guarantees finding the **minimum-cost route**.

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int N = 10;

using Point = pair<int, int>;

// Movement: Up, Down, Left, Right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};


/*
    COST OF ENTERING A CELL

    R = Road     -> cost 1
    M = Mud      -> cost 3
    T = Terrain  -> cost 5
    G = Goal     -> cost 1
    X = Obstacle -> cannot enter

    UCS chooses the path with the
    minimum TOTAL cost.
*/
int cost(char cell) {

    switch (cell) {
        case 'R': return 1;
        case 'M': return 3;
        case 'T': return 5;
        case 'G': return 1;
        default:  return 1e9; // X or any other invalid cell
    }
}


int main() {

    /*
        GRID

        S = Start
        G = Goal
        R = Normal road
        M = Mud
        T = Difficult terrain
        X = Obstacle

        Problem:
        Find the minimum-cost path from S to G.
    */
    vector<vector<char>> grid = {

        {'S','R','R','T','T','R','R','R','R','R'},
        {'X','X','R','T','X','R','X','X','X','R'},
        {'R','R','R','T','R','R','R','R','X','R'},
        {'R','X','X','T','R','X','X','R','X','R'},
        {'R','R','R','R','R','R','X','R','R','R'},
        {'R','X','X','X','X','R','X','X','X','R'},
        {'R','R','R','R','X','R','R','R','R','R'},
        {'X','X','X','R','X','X','X','X','X','R'},
        {'R','R','R','R','R','R','R','R','R','R'},
        {'X','X','X','X','X','X','X','X','X','G'}
    };


    Point start = {0, 0};
    Point goal = {9, 9};


    /*
        UNIFORM COST SEARCH (UCS)
        -------------------------

        UCS always expands the node having
        the SMALLEST PATH COST.

        Priority Queue stores:

            {cost, position}

        Example:

            {3, (1,2)}
            {5, (2,3)}
            {8, (4,5)}

        Node with cost 3 is processed first.

        This is essentially Dijkstra's algorithm
        for non-negative edge costs.
    */

    using Node = pair<int, Point>;

    // Min-heap: smallest cost comes first
    priority_queue<Node,
                   vector<Node>,
                   greater<Node>> pq;


    /*
        gCost[r][c]

        Minimum known cost to reach cell (r,c).

        Initially:
            infinity for every cell.

        Start:
            cost = 0
    */
    vector<vector<int>> gCost(
        N, vector<int>(N, 1e9)
    );


    /*
        parent[r][c]

        Stores the previous cell from which
        we reached (r,c).

        Used later to reconstruct the path.
    */
    vector<vector<Point>> parent(
        N, vector<Point>(N, {-1, -1})
    );


    // Keeps track of cells already expanded
    vector<vector<bool>> visited(
        N, vector<bool>(N, false)
    );


    // Stores order in which UCS expands nodes
    vector<Point> order;


    // Start UCS from the starting cell
    pq.push({0, start});
    gCost[start.first][start.second] = 0;


    while (!pq.empty()) {

        // Get cell with minimum cost
        auto [currCost, curr] = pq.top();
        pq.pop();

        int r = curr.first;
        int c = curr.second;


        // Ignore if already processed
        if (visited[r][c])
            continue;

        visited[r][c] = true;

        // Save expansion order
        order.push_back(curr);


        // Goal reached
        if (curr == goal)
            break;


        /*
            Check all 4 neighboring cells.
        */
        for (int i = 0; i < 4; i++) {

            int nr = r + dr[i];
            int nc = c + dc[i];


            // Check:
            // 1. Inside grid
            // 2. Not an obstacle
            if (nr >= 0 && nr < N &&
                nc >= 0 && nc < N &&
                grid[nr][nc] != 'X') {


                /*
                    New cost =
                    cost so far + cost of entering
                    the neighboring cell.
                */
                int newCost =
                    currCost + cost(grid[nr][nc]);


                /*
                    If this is cheaper than the
                    previously known cost, update it.
                */
                if (newCost < gCost[nr][nc]) {

                    gCost[nr][nc] = newCost;

                    // Remember previous cell
                    parent[nr][nc] = {r, c};

                    // Add new state to priority queue
                    pq.push({
                        newCost,
                        {nr, nc}
                    });
                }
            }
        }
    }


    /*
        PRINT EXPANSION ORDER

        This shows the order in which UCS
        explored the cells.
    */
    cout << "--- Uniform Cost Search ---\n\n";

    cout << "Nodes expanded:\n";

    for (int i = 0; i < order.size(); i++) {

        cout << "("
             << order[i].first << ", "
             << order[i].second << ")";

        if (i + 1 < order.size())
            cout << " -> ";
    }

    cout << "\n";


    // Goal was never reached
    if (!visited[goal.first][goal.second]) {

        cout << "\nNo path exists.\n";
        return 0;
    }


    /*
        RECONSTRUCT PATH

        Start from G and repeatedly follow
        parent[] until we reach S.

        Example:

            G <- C <- B <- A <- S

        Reverse it:

            S -> A -> B -> C -> G
    */
    vector<Point> path;

    Point curr = goal;

    while (curr != start) {

        path.push_back(curr);

        curr = parent[
            curr.first
        ][curr.second];
    }

    // Add starting point
    path.push_back(start);

    // Convert G -> S into S -> G
    reverse(path.begin(), path.end());


    /*
        PRINT FINAL ANSWER
    */

    cout << "\nMinimum-cost path:\n";

    for (int i = 0; i < path.size(); i++) {

        cout << "("
             << path[i].first << ", "
             << path[i].second << ")";

        if (i + 1 < path.size())
            cout << " -> ";
    }

    cout << "\n";

    cout << "\nTotal movements: "
         << path.size() - 1 << "\n";

    cout << "Total path cost: "
         << gCost[goal.first][goal.second]
         << "\n";


    return 0;
}
```

---

### P7: Rescue Robot (Depth-Limited Search - DLS)
- **Concept**: Depth-First Search with a hard depth cutoff $L$.
- **Fixed Successor Order**: **Right $\to$ Down $\to$ Left $\to$ Up**.
- **Three Possible Returns**:
  1. `SUCCESS` ($1$): Goal $G$ found at depth $\le L$.
  2. `CUTOFF` ($0$): Reached depth $L$ without reaching $G$ (solution lies deeper).
  3. `FAILURE` ($-1$): Search space exhausted without finding $G$.
- **Two Lab Test Runs**:
  - $L = 18 \implies$ **CUTOFF** (True shortest route needs 22 steps).
  - $L = 25 \implies$ **SUCCESS** (Found path in 22 steps).

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int N = 10;

// Movement priority: Right -> Down -> Left -> Up
int dr[] = {0, 1, 0, -1};
int dc[] = {1, 0, -1, 0};

using Point = pair<int, int>;


/*
    DLS RESULT

    SUCCESS = Goal found
    CUTOFF  = Search stopped because depth limit was reached
    FAILURE = No path exists
*/
const int SUCCESS = 1;
const int CUTOFF = 0;
const int FAILURE = -1;


// Stores the cells explored by DLS
vector<Point> expanded;

// Stores the current path
vector<Point> solution;

// Prevents the algorithm from visiting
// the same cell again in the CURRENT path
bool onPath[N][N];


/*
    DEPTH-LIMITED SEARCH (DLS)
    --------------------------

    DLS is basically DFS + a depth limit.

    DFS:
        Keep going deeper.

    DLS:
        Keep going deeper ONLY until
        depth == limit.

    Example:

        S
        |
        A       depth = 1
        |
        B       depth = 2
        |
        G       depth = 3

    If limit = 2:
        G will NOT be searched.

    If limit = 3:
        G can be found.
*/
int dls(const vector<vector<char>>& grid,
        Point curr,
        Point goal,
        int depth,
        int limit,
        vector<Point>& path) {

    // Record that this node was expanded
    expanded.push_back(curr);


    // Goal found
    if (curr == goal) {

        solution = path;

        return SUCCESS;
    }


    /*
        If depth limit has been reached,
        don't go any deeper.

        This is the main difference
        between DFS and DLS.
    */
    if (depth == limit)
        return CUTOFF;


    bool cutoffFound = false;

    int r = curr.first;
    int c = curr.second;


    /*
        Try all four possible movements.

        Order:
            Right
            Down
            Left
            Up
    */
    for (int i = 0; i < 4; i++) {

        int nr = r + dr[i];
        int nc = c + dc[i];


        /*
            Move only if:
            1. Inside grid
            2. Not an obstacle
            3. Not already present in current path
        */
        if (nr >= 0 && nr < N &&
            nc >= 0 && nc < N &&
            grid[nr][nc] != 'X' &&
            !onPath[nr][nc]) {


            // Mark cell as part of current path
            onPath[nr][nc] = true;

            path.push_back({nr, nc});


            // Recursively search next cell
            int result = dls(
                grid,
                {nr, nc},
                goal,
                depth + 1,
                limit,
                path
            );


            // Backtrack
            path.pop_back();
            onPath[nr][nc] = false;


            // Goal found
            if (result == SUCCESS)
                return SUCCESS;


            // At least one branch hit depth limit
            if (result == CUTOFF)
                cutoffFound = true;
        }
    }


    /*
        If any branch was stopped because of
        depth limit -> return CUTOFF.

        Otherwise -> no path exists from here.
    */
    return cutoffFound ? CUTOFF : FAILURE;
}


/*
    RUN DLS WITH A GIVEN DEPTH LIMIT
*/
void runDLS(const vector<vector<char>>& grid, int limit) {

    Point start = {0, 0};
    Point goal = {9, 9};


    cout << "--- Depth-Limited Search ---\n";
    cout << "Depth Limit: " << limit << "\n";


    // Reset previous run
    expanded.clear();
    solution.clear();

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            onPath[i][j] = false;


    // Starting cell belongs to current path
    onPath[start.first][start.second] = true;

    vector<Point> path = {start};


    // Start DLS
    int result = dls(
        grid,
        start,
        goal,
        0,          // Starting depth
        limit,
        path
    );


    /*
        PRINT EXPANSION ORDER
    */
    cout << "Nodes expanded:\n";

    for (int i = 0; i < expanded.size(); i++) {

        cout << "("
             << expanded[i].first << ", "
             << expanded[i].second << ")";

        if (i + 1 < expanded.size())
            cout << " -> ";
    }

    cout << "\n";


    /*
        PRINT RESULT
    */

    if (result == SUCCESS) {

        cout << "Result: SUCCESS\n";

        cout << "Route:\n";

        for (int i = 0; i < solution.size(); i++) {

            cout << "("
                 << solution[i].first << ", "
                 << solution[i].second << ")";

            if (i + 1 < solution.size())
                cout << " -> ";
        }

        cout << "\n";

        cout << "Movements: "
             << solution.size() - 1 << "\n";
    }

    else if (result == CUTOFF) {

        cout << "Result: CUTOFF\n";
        cout << "Goal was not found within depth "
             << limit << "\n";
    }

    else {

        cout << "Result: FAILURE\n";
        cout << "Goal is not reachable.\n";
    }

    cout << "\n";
}


int main() {

    /*
        GRID

        S = Start
        G = Goal
        R = Normal road
        X = Obstacle

        Task:
        Find a path from S -> G.

        DLS will search using DFS,
        but it cannot go deeper than
        the specified depth limit.
    */

    vector<vector<char>> grid = {

        {'S','R','R','X','R','R','R','R','R','R'},
        {'X','X','R','X','R','X','X','X','X','R'},
        {'R','R','R','X','R','R','R','R','X','R'},
        {'R','X','X','X','X','X','X','R','X','R'},
        {'R','R','R','R','R','R','X','R','R','R'},
        {'X','X','X','X','X','R','X','X','X','R'},
        {'R','R','R','R','X','R','R','R','R','R'},
        {'R','X','X','R','X','X','X','X','X','R'},
        {'R','R','R','R','R','R','R','R','R','R'},
        {'X','X','X','X','X','X','X','X','X','G'}
    };


    /*
        Run DLS twice.

        First:
            limit = 18

        Second:
            limit = 25

        If the solution needs more than 18
        movements, the first run will return
        CUTOFF.

        The second run may find the goal.
    */

    runDLS(grid, 18);

    runDLS(grid, 25);


    return 0;
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
> **Answer**: DLS stops expanding any branch once its depth reaches the limit $L$. If the shortest path from $S$ to $G$ requires more than $L$ moves (e.g. $L = 18$ while the true path needs 22 moves), the search hits a `CUTOFF` along every branch before it can ever reach $G$.

### Q2. How does increasing limit $L$ affect time and memory in DLS?
> **Answer**:
> - **Time**: Increases exponentially in worst case ($O(b^L)$) because deeper tree levels are explored.
> - **Memory**: Grows only **linearly** ($O(b \cdot L)$) because only the active recursion path and its unexpanded siblings are stored in memory.

### Q3. Is DLS guaranteed to return the shortest path?
> **Answer**: **No.** DLS is depth-first and stops at the very **first** goal path it encounters under its fixed successor order (`Right -> Down -> Left -> Up`). It does not compare alternative paths to check if a shorter one exists.

### Q4. Why does a Simple Reflex Agent oscillate between cells?
> **Answer**: Because it has **no memory (state)**. Once it cleans a dirty cell, the cell becomes clean. At the next step, based on its fixed rule priority (e.g. Up, Right, Down, Left), it moves back into the cell it just came from, creating an infinite oscillation loop between adjacent clean cells.

### Q5. Why does UCS use a Priority Queue instead of a simple FIFO Queue?
> **Answer**: A FIFO queue only guarantees shortest paths when all step costs are equal (1 unit). When terrain costs vary (e.g. Normal road = 1, Mud = 3, Traffic = 5), a priority queue ensures the node with the **lowest accumulated cost $g(n)$** is always expanded first.

---

## 9. Common Exam Pitfalls & Tips

1. **Matrix Indexing**:
   - `grid[row][col]` where `row` is vertical and `col` is horizontal.
   - Move Up: `r - 1, c`
   - Move Down: `r + 1, c`
   - Move Left: `r, c - 1`
   - Move Right: `r, c + 1`
2. **Cycle Prevention in DLS**:
   - Always set `onPath[nr][nc] = true` before recursion, and backtrack with `onPath[nr][nc] = false` after returning.
3. **Calculating Movements**:
   - $\text{Movements} = \text{Path Size} - 1$ (since the starting node itself is not a movement).

---
*Good luck with your Lab Test tomorrow! You are fully prepared.* 🚀
