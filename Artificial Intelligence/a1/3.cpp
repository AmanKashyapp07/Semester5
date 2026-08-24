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
        };
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
