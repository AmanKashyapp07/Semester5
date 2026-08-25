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