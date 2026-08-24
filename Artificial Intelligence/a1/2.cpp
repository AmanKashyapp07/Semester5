#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;

using State = pair<int, int>; // (jugA, jugB)

// Solve Water Jug using Breadth-First Search (BFS)
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

        int x = curr.first;
        int y = curr.second;

        // Check if either jug has reached the target 'c' liters
        if (x == c || y == c) {
            goal = curr;
            break;
        }

        // 6 possible operations:
        vector<State> nextStates = {
            {a, y},                                      // 1. Fill Jug A
            {x, b},                                      // 2. Fill Jug B
            {0, y},                                      // 3. Empty Jug A
            {x, 0},                                      // 4. Empty Jug B
            {x - min(x, b - y), y + min(x, b - y)},       // 5. Pour A -> B
            {x + min(y, a - x), y - min(y, a - x)}        // 6. Pour B -> A
        };

        for (auto next : nextStates) {
            if (!visited[next]) {
                visited[next] = true;
                parent[next] = curr;
                q.push(next);
            }
        }
    }

    // Reconstruct solution path from goal to start
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
    cout << "Water Jug Problem: " << a << "-li and " << b << "-li jugs\n\n";

    vector<State> path = solveWaterJug(a, b, c);

    if (path.empty()) {
        cout << "No solution exists.\n";
        return 0;
    }

    cout << left << setw(6) << "Step" 
         << setw(12) << (to_string(a) + "-li jug") 
         << setw(12) << (to_string(b) + "-li jug") << "\n";

    for (int i = 0; i < (int)path.size(); i++) {
        cout << left << setw(6) << i 
             << setw(12) << path[i].first 
             << setw(12) << path[i].second << "\n";
    }

    cout << "\nGoal reached: " << c << " liters obtained in " << (path.size() - 1) << " steps.\n";
    return 0;
}
