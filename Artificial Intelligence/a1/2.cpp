#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;



vector<pair<int,int>> solve(int a, int b, int c) {
    queue<pair<int,int>> q; // queue to perform BFS
    map<pair<int,int>, pair<int,int>> parent; // stores the parent of each state to reconstruct the path
    map<pair<int,int>, bool> vis; // keeps track of visited states

    pair<int,int> start = {0, 0}; // starting state where both jugs are empty
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
            {a, y},                         // Fill A
            {x, b},                         // Fill B
            {0, y},                         // Empty A
            {x, 0},                         // Empty B
            {x - min(x, b-y), y + min(x, b-y)}, // A -> B
            {x + min(y, a-x), y - min(y, a-x)}  // B -> A
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