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
    int maxSteps = 1000; // Limit steps to prevent infinite oscillation

    while (cleaned < totalDirty && moves < maxSteps) {

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