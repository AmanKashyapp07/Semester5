#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

const map<string, map<string, int>> graph = {
    {"a", {{"b", 5}, {"c", 2}, {"d", 3}}},
    {"b", {{"a", 5}, {"c", 2}, {"f", 3}}},
    {"c", {{"a", 2}, {"b", 2}, {"d", 1}, {"e", 2}, {"f", 6}}},
    {"d", {{"a", 3}, {"c", 1}, {"e", 4}}},
    {"e", {{"c", 2}, {"d", 4}, {"f", 4}}},
    {"f", {{"b", 3}, {"c", 6}, {"e", 4}}}
};

const map<string, int> heuristic = {
    {"a", 6}, {"b", 2}, {"c", 5}, {"d", 6}, {"e", 4}, {"f", 0}
};

pair<vector<string>, int> dijkstra(const map<string, map<string, int>>& graph, const string& start, const string& goal) {
    map<string, int> dist;
    map<string, string> prev;
    set<string> visited;

    for (const auto& entry : graph) {
        dist[entry.first] = numeric_limits<int>::max();
        prev[entry.first] = "";
    }
    dist[start] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, node] = pq.top();
        pq.pop();

        if (visited.count(node)) {
            continue;
        }
        visited.insert(node);

        if (node == goal) {
            break;
        }

        for (const auto& [neighbor, weight] : graph.at(node)) {
            int new_dist = d + weight;
            if (new_dist < dist[neighbor]) {
                dist[neighbor] = new_dist;
                prev[neighbor] = node;
                pq.push({new_dist, neighbor});
            }
        }
    }

    vector<string> path;
    string node = goal;
    while (!node.empty()) {
        path.push_back(node);
        if (prev.count(node) == 0) {
            break;
        }
        node = prev[node];
    }
    reverse(path.begin(), path.end());

    return {path, dist[goal]};
}

pair<vector<string>, int> a_star(const map<string, map<string, int>>& graph, const string& start, const string& goal, const map<string, int>& heuristic) {
    map<string, int> g_score;
    map<string, string> prev;
    set<string> closed;

    for (const auto& entry : graph) {
        g_score[entry.first] = numeric_limits<int>::max();
        prev[entry.first] = "";
    }
    g_score[start] = 0;

    struct NodeRecord {
        int f_score;
        string node;
        bool operator<(const NodeRecord& other) const {
            return f_score > other.f_score;
        }
    };

    priority_queue<NodeRecord> open_set;
    open_set.push({heuristic.at(start), start});

    while (!open_set.empty()) {
        auto current = open_set.top();
        open_set.pop();

        if (current.node == goal) {
            break;
        }
        if (closed.count(current.node)) {
            continue;
        }
        closed.insert(current.node);

        for (const auto& [neighbor, weight] : graph.at(current.node)) {
            int tentative_g = g_score[current.node] + weight;
            if (tentative_g < g_score[neighbor]) {
                g_score[neighbor] = tentative_g;
                prev[neighbor] = current.node;
                int f_score = tentative_g + heuristic.at(neighbor);
                open_set.push({f_score, neighbor});
            }
        }
    }

    vector<string> path;
    string node = goal;
    while (!node.empty()) {
        path.push_back(node);
        if (prev.count(node) == 0) {
            break;
        }
        node = prev[node];
    }
    reverse(path.begin(), path.end());

    return {path, g_score[goal]};
}

void run_question1() {
    cout << string(60, '=') << '\n';
    cout << "QUESTION 1 : Shortest path from 'a' to 'f'" << '\n';
    cout << string(60, '=') << '\n';

    auto [path, cost] = dijkstra(graph, "a", "f");
    cout << "\n(a) Dijkstra's algorithm" << '\n';
    cout << "    Path : ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << path[i];
    }
    cout << '\n';
    cout << "    Cost : " << cost << '\n';

    auto [astar_path, astar_cost] = a_star(graph, "a", "f", heuristic);
    cout << "\n(b) A* algorithm" << '\n';
    cout << "    Path : ";
    for (size_t i = 0; i < astar_path.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << astar_path[i];
    }
    cout << '\n';
    cout << "    Cost : " << astar_cost << '\n';
}

int main() {
    run_question1();
    return 0;
}
