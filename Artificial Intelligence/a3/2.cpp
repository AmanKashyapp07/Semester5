#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int ROWS = 10;
const int COLS = 10;

// Fixed move priority: Right, Down, Left, Up
int dr[] = {0, 1, 0, -1};
int dc[] = {1, 0, -1, 0};

using Point = pair<int, int>;

// Status constants
const int SUCCESS = 1;
const int CUTOFF = 0;
const int FAILURE = -1;

vector<Point> expansionOrder;
int maxDepthReached = 0;
bool onPath[ROWS][COLS];
vector<Point> solutionPath;

// Recursive Depth-Limited Search (DLS)
int dls(const vector<vector<char>>& grid, Point curr, Point goal, int depth, int limit, vector<Point>& path) {
    expansionOrder.push_back(curr);
    maxDepthReached = max(maxDepthReached, depth);

    if (curr == goal) {
        solutionPath = path;
        return SUCCESS;
    }

    if (depth == limit) return CUTOFF;

    bool cutoffOccurred = false;
    int r = curr.first, c = curr.second;

    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && grid[nr][nc] != 'X' && !onPath[nr][nc]) {
            onPath[nr][nc] = true;
            path.push_back({nr, nc});

            int result = dls(grid, {nr, nc}, goal, depth + 1, limit, path);

            path.pop_back();
            onPath[nr][nc] = false;

            if (result == SUCCESS) return SUCCESS;
            if (result == CUTOFF) cutoffOccurred = true;
        }
    }

    return cutoffOccurred ? CUTOFF : FAILURE;
}

void runDLS(const vector<vector<char>>& grid, int limit) {
    cout << "--- Depth-Limited Search: Limit = " << limit << " ---\n";

    expansionOrder.clear();
    maxDepthReached = 0;
    solutionPath.clear();
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            onPath[i][j] = false;

    Point start = {0, 0};
    Point goal = {9, 9};

    onPath[start.first][start.second] = true;
    vector<Point> path = {start};

    int result = dls(grid, start, goal, 0, limit, path);

    cout << "Nodes expanded (in order):\n";
    for (int i = 0; i < (int)expansionOrder.size(); i++) {
        cout << "(" << expansionOrder[i].first << ", " << expansionOrder[i].second << ")";
        if (i + 1 < (int)expansionOrder.size()) cout << " -> ";
    }
    cout << "\n";
    cout << "Maximum depth reached: " << maxDepthReached << "\n";

    if (result == SUCCESS) {
        cout << "Result: SUCCESS\nRoute:\n";
        for (int i = 0; i < (int)solutionPath.size(); i++) {
            cout << "(" << solutionPath[i].first << ", " << solutionPath[i].second << ")";
            if (i + 1 < (int)solutionPath.size()) cout << " -> ";
        }
        cout << "\nSolution depth / movements: " << (solutionPath.size() - 1) << "\n";
    } else if (result == CUTOFF) {
        cout << "Result: CUTOFF / Goal not found within depth limit\n";
        cout << "Maximum allowed depth: " << limit << "\n";
    } else {
        cout << "Result: FAILURE / Goal not reachable\n";
    }
    cout << "\n";
}

int main() {
    vector<vector<char>> grid = {
        {'S', 'R', 'R', 'X', 'R', 'R', 'R', 'R', 'R', 'R'},
        {'X', 'X', 'R', 'X', 'R', 'X', 'X', 'X', 'X', 'R'},
        {'R', 'R', 'R', 'X', 'R', 'R', 'R', 'R', 'X', 'R'},
        {'R', 'X', 'X', 'X', 'X', 'X', 'X', 'R', 'X', 'R'},
        {'R', 'R', 'R', 'R', 'R', 'R', 'X', 'R', 'R', 'R'},
        {'X', 'X', 'X', 'X', 'X', 'R', 'X', 'X', 'X', 'R'},
        {'R', 'R', 'R', 'R', 'X', 'R', 'R', 'R', 'R', 'R'},
        {'R', 'X', 'X', 'R', 'X', 'X', 'X', 'X', 'X', 'R'},
        {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'},
        {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'G'}
    };

    runDLS(grid, 18); // Run 1
    runDLS(grid, 25); // Run 2

    return 0;
}

/*
---------------------------------------------------------------
Comparison / Discussion Questions & Analysis
---------------------------------------------------------------
1. Why can DLS fail to find a goal even when a valid path exists?
   DLS stops expanding when depth == limit L. If the shortest valid path
   needs more than L moves (e.g., L = 18 when 22 are needed), DLS hits
   a CUTOFF on all candidate paths before reaching the goal.

2. How does increasing L affect time and memory usage?
   Time: Increases exponentially in the worst case (O(b^L)), as deeper
   branches are explored.
   Memory: Grows linearly (O(L)), because only the current recursion path
   is kept in memory.

3. How is DLS different from ordinary DFS?
   DFS can wander infinitely deep into loops/cycles or deep branches.
   DLS imposes a hard depth cutoff L, guaranteeing termination.

4. Is DLS guaranteed to return the shortest path?
   No. DLS returns the FIRST path found under its fixed successor
   order (Right -> Down -> Left -> Up). It does not search for or
   compare alternative shorter paths.
*/
