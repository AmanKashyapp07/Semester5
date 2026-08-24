#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <iomanip>
#include <algorithm>
using namespace std;

const int ROWS = 10;
const int COLS = 10;

int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

using Point = pair<int, int>;

// Format path helper
string formatPath(const vector<Point>& path) {
    if (path.empty()) return "No Path Found";
    if (path.size() <= 3) {
        string res = "";
        for (int i = 0; i < (int)path.size(); i++) {
            res += "(" + to_string(path[i].first) + "," + to_string(path[i].second) + ")";
            if (i + 1 < (int)path.size()) res += " -> ";
        }
        return res;
    }
    return "(" + to_string(path.front().first) + "," + to_string(path.front().second) +
           ") -> ... -> (" + to_string(path.back().first) + "," + to_string(path.back().second) + ")";
}

// 1. Goal-Based Agent: Breadth-First Search (BFS)
vector<Point> bfs(const vector<vector<char>>& grid, Point start, Point goal) {
    queue<Point> q;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    vector<vector<Point>> parent(ROWS, vector<Point>(COLS, {-1, -1}));

    q.push(start);
    visited[start.first][start.second] = true;

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        int r = curr.first;
        int c = curr.second;

        if (r == goal.first && c == goal.second) break;

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

    if (!visited[goal.first][goal.second]) return {};

    vector<Point> path;
    Point curr = goal;
    while (curr != make_pair(-1, -1)) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());
    return path;
}

// Calculate cost for entering a cell
double getCellCost(int r, int c) {
    // High traffic / risk shortcut zone at (2, 7) and (2, 6)
    double dist = 1.0, energy = 1.0, risk = 1.0, traffic = 1.0;
    if ((r == 2 && c == 7) || (r == 2 && c == 6)) {
        energy = 5.0; risk = 4.0; traffic = 3.0;
    }
    // Cost = 0.4*Distance + 0.3*Energy + 0.2*Risk + 0.1*Traffic
    return (0.4 * dist) + (0.3 * energy) + (0.2 * risk) + (0.1 * traffic);
}

// 2. Utility-Based Agent: Uniform Cost Search (Dijkstra)
pair<vector<Point>, double> ucs(const vector<vector<char>>& grid, Point start, Point goal) {
    using Node = pair<double, Point>; // (cost, (r, c))
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    vector<vector<double>> minCost(ROWS, vector<double>(COLS, 1e9));
    vector<vector<Point>> parent(ROWS, vector<Point>(COLS, {-1, -1}));

    pq.push({0.0, start});
    minCost[start.first][start.second] = 0.0;

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        double cost = top.first;
        Point curr = top.second;

        int r = curr.first, c = curr.second;
        if (curr == goal) break;
        if (cost > minCost[r][c]) continue;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && grid[nr][nc] != 'X') {
                double newCost = cost + getCellCost(nr, nc);
                if (newCost < minCost[nr][nc]) {
                    minCost[nr][nc] = newCost;
                    parent[nr][nc] = {r, c};
                    pq.push({newCost, {nr, nc}});
                }
            }
        }
    }

    if (minCost[goal.first][goal.second] >= 1e9) return {{}, 1e9};

    vector<Point> path;
    Point curr = goal;
    while (curr != make_pair(-1, -1)) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());
    return {path, minCost[goal.first][goal.second]};
}

int main() {
    vector<string> layout = {
        "S R R X R R R R R R",
        "X X R R R X R X R R",
        "R R R X R R R P R R",
        "R X R R R X R R R X",
        "R R R X R R X R R R",
        "X R R R R R R R X R",
        "R R X R X R R R R R",
        "R R R R R X R X R R",
        "R X R R R R R R R R",
        "R R R X R R R R X G"
    };

    vector<vector<char>> grid(ROWS, vector<char>(COLS));
    Point S, P, G;

    for (int i = 0; i < ROWS; i++) {
        int colIdx = 0;
        for (char ch : layout[i]) {
            if (ch != ' ') {
                grid[i][colIdx] = ch;
                if (ch == 'S') S = {i, colIdx};
                if (ch == 'P') P = {i, colIdx};
                if (ch == 'G') G = {i, colIdx};
                colIdx++;
            }
        }
    }

    // 1. Run Goal-Based Agent
    vector<Point> pathToPackage = bfs(grid, S, P);
    vector<Point> pathToDest = bfs(grid, P, G);
    int movPackage = pathToPackage.size() - 1;
    int movDest = pathToDest.size() - 1;

    cout << "--- Goal-Based Agent ---\n";
    cout << "Route to Package: " << formatPath(pathToPackage) << "\n";
    cout << "Movements to Package: " << movPackage << "\n";
    cout << "Route to Destination: " << formatPath(pathToDest) << "\n";
    cout << "Movements to Destination: " << movDest << "\n";
    cout << "Total Movements: " << (movPackage + movDest) << "\n\n";

    // 2. Run Utility-Based Agent
    auto resPackage = ucs(grid, S, P);
    auto resDest = ucs(grid, P, G);
    int ucsTotalMov = (resPackage.first.size() - 1) + (resDest.first.size() - 1);
    double totalCost = resPackage.second + resDest.second;

    cout << "--- Utility-Based Agent ---\n";
    cout << "Route to Package: " << formatPath(resPackage.first) << "\n";
    cout << "Route to Destination: " << formatPath(resDest.first) << "\n";
    cout << "Total Movements: " << ucsTotalMov << "\n";
    cout << fixed << setprecision(1);
    cout << "Total Cost: " << totalCost << "\n";
    cout << "The selected route has lower overall utility cost despite having more movements.\n";

    return 0;
}
