#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int ROWS = 10;
const int COLS = 10;

// Directions: Up, Down, Left, Right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

using Point = pair<int, int>;

// Terrain entry costs
int getCost(char ch) {
    if (ch == 'S') return 0;
    if (ch == 'G' || ch == 'R') return 1;
    if (ch == 'M') return 3;
    if (ch == 'T') return 5;
    return 1;
}

int main() {
    vector<vector<char>> grid = {
        {'S', 'R', 'R', 'T', 'T', 'R', 'R', 'R', 'R', 'R'},
        {'X', 'X', 'R', 'T', 'X', 'R', 'X', 'X', 'X', 'R'},
        {'R', 'R', 'R', 'T', 'R', 'R', 'R', 'R', 'X', 'R'},
        {'R', 'X', 'X', 'T', 'R', 'X', 'X', 'R', 'X', 'R'},
        {'R', 'R', 'R', 'R', 'R', 'R', 'X', 'R', 'R', 'R'},
        {'R', 'X', 'X', 'X', 'X', 'R', 'X', 'X', 'X', 'R'},
        {'R', 'R', 'R', 'R', 'X', 'R', 'R', 'R', 'R', 'R'},
        {'X', 'X', 'X', 'R', 'X', 'X', 'X', 'X', 'X', 'R'},
        {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'},
        {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'G'}
    };

    Point start = {0, 0};
    Point goal = {9, 9};

    cout << "--- Uniform Cost Search ---\n";
    cout << "Start: (" << start.first << ", " << start.second << ")\n";
    cout << "Goal: (" << goal.first << ", " << goal.second << ")\n\n";

    // Min-heap priority queue: pair<cost, (r, c)>
    using Node = pair<int, Point>;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    vector<vector<int>> gCost(ROWS, vector<int>(COLS, 1e9));
    vector<vector<Point>> parent(ROWS, vector<Point>(COLS, {-1, -1}));
    vector<vector<bool>> expanded(ROWS, vector<bool>(COLS, false));
    vector<Point> expansionOrder;

    pq.push({0, start});
    gCost[start.first][start.second] = 0;

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        int cost = top.first;
        Point curr = top.second;

        int r = curr.first, c = curr.second;
        if (expanded[r][c]) continue; // Skip stale entries

        expanded[r][c] = true;
        expansionOrder.push_back(curr);

        if (curr == goal) break;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && grid[nr][nc] != 'X') {
                int newCost = cost + getCost(grid[nr][nc]);
                if (newCost < gCost[nr][nc]) {
                    gCost[nr][nc] = newCost;
                    parent[nr][nc] = {r, c};
                    pq.push({newCost, {nr, nc}});
                }
            }
        }
    }

    cout << "Nodes expanded (in order):\n";
    for (int i = 0; i < (int)expansionOrder.size(); i++) {
        cout << "(" << expansionOrder[i].first << ", " << expansionOrder[i].second << ")";
        if (i + 1 < (int)expansionOrder.size()) cout << " -> ";
    }
    cout << "\n";

    if (!expanded[goal.first][goal.second]) {
        cout << "\nNo path exists\n";
        return 0;
    }

    // Reconstruct minimum-cost path
    vector<Point> path;
    Point curr = goal;
    while (curr != make_pair(-1, -1)) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());

    cout << "\nMinimum-cost route:\n";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << "(" << path[i].first << ", " << path[i].second << ")";
        if (i + 1 < (int)path.size()) cout << " -> ";
    }
    cout << "\n\nTotal movements: " << (path.size() - 1) << "\n";
    cout << "Total path cost: " << gCost[goal.first][goal.second] << "\n";

    return 0;
}
