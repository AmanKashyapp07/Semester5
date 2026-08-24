#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
using namespace std;

const int ROWS = 10;
const int COLS = 10;

// Direction priority: Up, Right, Down, Left
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

// Helper to count total dirty cells in grid
int countDirty(const vector<vector<char>>& grid) {
    int count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == 'D') count++;
        }
    }
    return count;
}

// 1. Simple Reflex Agent (No internal memory, moves by fixed priority rules)
void runSimpleReflex(vector<vector<char>> grid, int startR, int startC) {
    int r = startR, c = startC;
    int totalDirty = countDirty(grid);
    int cleaned = 0, movements = 0, totalActions = 0;
    map<pair<int, int>, int> visitCount;
    visitCount[{r, c}] = 1;

    int maxSteps = 1000; // Cap to handle reflex oscillation
    for (int step = 0; step < maxSteps && cleaned < totalDirty; step++) {
        // Condition-Action: If current cell is dirty -> Clean it
        if (grid[r][c] == 'D') {
            grid[r][c] = 'C';
            cleaned++;
            totalActions++;
            continue;
        }

        // Otherwise move by priority: Up, Right, Down, Left
        bool moved = false;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && grid[nr][nc] != 'X') {
                r = nr;
                c = nc;
                movements++;
                totalActions++;
                visitCount[{r, c}]++;
                moved = true;
                break;
            }
        }
        if (!moved) break;
    }

    int repeatedVisits = 0;
    for (auto& entry : visitCount) {
        if (entry.second > 1) repeatedVisits += (entry.second - 1);
    }

    cout << "--- Simple Reflex Agent ---\n";
    cout << "Dirty cells cleaned: " << cleaned << "\n";
    cout << "Movements: " << movements << "\n";
    cout << "Total actions: " << totalActions << "\n";
    cout << "Repeated visits: " << repeatedVisits << "\n\n";
}

// BFS helper for Model-Based Agent to find shortest path to nearest dirty cell
vector<pair<int, int>> findNearestDirty(const vector<vector<char>>& grid, int startR, int startC) {
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    vector<vector<pair<int, int>>> parent(ROWS, vector<pair<int, int>>(COLS, {-1, -1}));

    q.push({startR, startC});
    visited[startR][startC] = true;
    pair<int, int> target = {-1, -1};

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        int r = curr.first;
        int c = curr.second;

        if (grid[r][c] == 'D') {
            target = {r, c};
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && grid[nr][nc] != 'X') {
                if (!visited[nr][nc]) {
                    visited[nr][nc] = true;
                    parent[nr][nc] = {r, c};
                    q.push({nr, nc});
                }
            }
        }
    }

    if (target.first == -1) return {};

    vector<pair<int, int>> path;
    pair<int, int> curr = target;
    while (curr != make_pair(startR, startC)) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());
    return path;
}

// 2. Model-Based Reflex Agent (Maintains memory state & plans path to nearest dirty cell)
void runModelBased(vector<vector<char>> grid, int startR, int startC) {
    int r = startR, c = startC;
    int totalDirty = countDirty(grid);
    int cleaned = 0, movements = 0, totalActions = 0;
    map<pair<int, int>, int> visitCount;
    visitCount[{r, c}] = 1;

    while (cleaned < totalDirty) {
        if (grid[r][c] == 'D') {
            grid[r][c] = 'C';
            cleaned++;
            totalActions++;
            continue;
        }

        vector<pair<int, int>> path = findNearestDirty(grid, r, c);
        if (path.empty()) break; // All reachable dirty cells cleaned

        // Take next step along planned path
        r = path[0].first;
        c = path[0].second;
        movements++;
        totalActions++;
        visitCount[{r, c}]++;
    }

    int repeatedVisits = 0;
    for (auto& entry : visitCount) {
        if (entry.second > 1) repeatedVisits += (entry.second - 1);
    }

    cout << "--- Model-Based Reflex Agent ---\n";
    cout << "Dirty cells cleaned: " << cleaned << "\n";
    cout << "Movements: " << movements << "\n";
    cout << "Total actions: " << totalActions << "\n";
    cout << "Repeated visits: " << repeatedVisits << "\n\n";

    cout << "--- Final State of Grid (After Model-Based Cleaning) ---\n";
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << grid[i][j] << (j + 1 < COLS ? " " : "");
        }
        cout << "\n";
    }
}

int main() {
    vector<string> rawGrid = {
        "S C D C X C C D C C",
        "C X C C C C X C D C",
        "D C C X D C C C C C",
        "C C X C C C D X C C",
        "C D C C X C C C C D",
        "C C C D C C X C C C",
        "X C C C C D C C X C",
        "C C D X C C C D C C",
        "C X C C D C C C C C",
        "D C C C C X C C D C"
    };

    vector<vector<char>> grid(ROWS, vector<char>(COLS));
    int startR = 0, startC = 0;

    for (int i = 0; i < ROWS; i++) {
        int colIdx = 0;
        for (char ch : rawGrid[i]) {
            if (ch != ' ') {
                grid[i][colIdx] = ch;
                if (ch == 'S') { startR = i; startC = colIdx; }
                colIdx++;
            }
        }
    }

    runSimpleReflex(grid, startR, startC);
    runModelBased(grid, startR, startC);

    return 0;
}
