#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

const vector<int> START = {1, 2, 3, 0, 4, 6, 7, 5, 8};
const vector<int> GOAL = {1, 2, 3, 4, 5, 6, 7, 8, 0};

vector<vector<int>> get_neighbors(const vector<int>& state) {
    vector<vector<int>> neighbors;
    int blank = find(state.begin(), state.end(), 0) - state.begin();
    int row = blank / 3;
    int col = blank % 3;

    vector<int> moves;
    if (row > 0) moves.push_back(-3);
    if (row < 2) moves.push_back(3);
    if (col > 0) moves.push_back(-1);
    if (col < 2) moves.push_back(1);

    for (int move : moves) {
        int new_blank = blank + move;
        vector<int> next = state;
        swap(next[blank], next[new_blank]);
        neighbors.push_back(next);
    }

    return neighbors;
}

void print_state(const vector<int>& state) {
    for (int i = 0; i < 9; i += 3) {
        cout << "   ";
        for (int j = 0; j < 3; ++j) {
            int value = state[i + j];
            cout << (value == 0 ? "#" : to_string(value)) << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}

vector<vector<int>> reconstruct_path(const map<vector<int>, vector<int>>& came_from, const vector<int>& state) {
    vector<vector<int>> path;
    vector<int> current = state;
    path.push_back(current);

    while (came_from.count(current)) {
        current = came_from.at(current);
        path.push_back(current);
    }
    reverse(path.begin(), path.end());
    return path;
}

pair<vector<vector<int>>, int> bfs_solve(const vector<int>& start, const vector<int>& goal) {
    queue<vector<int>> frontier;
    frontier.push(start);

    set<vector<int>> visited;
    visited.insert(start);

    map<vector<int>, vector<int>> came_from;
    int nodes_expanded = 0;

    while (!frontier.empty()) {
        vector<int> state = frontier.front();
        frontier.pop();
        nodes_expanded++;

        if (state == goal) {
            return {reconstruct_path(came_from, state), nodes_expanded};
        }

        for (const auto& nxt : get_neighbors(state)) {
            if (!visited.count(nxt)) {
                visited.insert(nxt);
                came_from[nxt] = state;
                frontier.push(nxt);
            }
        }
    }

    return {{}, nodes_expanded};
}

pair<vector<vector<int>>, int> dfs_solve(const vector<int>& start, const vector<int>& goal, int depth_limit = 30) {
    stack<pair<vector<int>, int>> stack;
    stack.push({start, 0});

    set<vector<int>> visited;
    visited.insert(start);

    map<vector<int>, vector<int>> came_from;
    int nodes_expanded = 0;

    while (!stack.empty()) {
        auto [state, depth] = stack.top();
        stack.pop();
        nodes_expanded++;

        if (state == goal) {
            return {reconstruct_path(came_from, state), nodes_expanded};
        }

        if (depth >= depth_limit) {
            continue;
        }

        auto neighbors = get_neighbors(state);
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            const auto& nxt = *it;
            if (!visited.count(nxt)) {
                visited.insert(nxt);
                came_from[nxt] = state;
                stack.push({nxt, depth + 1});
            }
        }
    }

    return {{}, nodes_expanded};
}

int misplaced_tiles(const vector<int>& state, const vector<int>& goal) {
    int count = 0;
    for (size_t i = 0; i < state.size(); ++i) {
        if (state[i] != 0 && state[i] != goal[i]) {
            count++;
        }
    }
    return count;
}

pair<vector<vector<int>>, int> a_star_puzzle(const vector<int>& start, const vector<int>& goal) {
    struct NodeRecord {
        int f_score;
        int counter;
        vector<int> state;
        bool operator<(const NodeRecord& other) const {
            if (f_score != other.f_score) return f_score > other.f_score;
            return counter > other.counter;
        }
    };

    int counter = 0;
    map<vector<int>, int> g_score;
    map<vector<int>, vector<int>> came_from;
    g_score[start] = 0;

    priority_queue<NodeRecord> open_set;
    open_set.push({misplaced_tiles(start, goal), counter++, start});

    set<vector<int>> closed;
    int nodes_expanded = 0;

    while (!open_set.empty()) {
        NodeRecord current = open_set.top();
        open_set.pop();
        nodes_expanded++;

        if (current.state == goal) {
            return {reconstruct_path(came_from, current.state), nodes_expanded};
        }

        if (closed.count(current.state)) {
            continue;
        }
        closed.insert(current.state);

        for (const auto& nxt : get_neighbors(current.state)) {
            int tentative_g = g_score[current.state] + 1;
            if (closed.count(nxt) && tentative_g >= g_score[nxt]) {
                continue;
            }
            if (tentative_g < g_score[nxt]) {
                g_score[nxt] = tentative_g;
                came_from[nxt] = current.state;
                open_set.push({tentative_g + misplaced_tiles(nxt, goal), counter++, nxt});
            }
        }
    }

    return {{}, nodes_expanded};
}

void run_question2() {
    cout << string(60, '=') << '\n';
    cout << "QUESTION 2 : 8-puzzle" << '\n';
    cout << string(60, '=') << '\n';
    cout << "\nStart state:" << '\n';
    print_state(START);
    cout << "Goal state:" << '\n';
    print_state(GOAL);

    vector<pair<string, function<pair<vector<vector<int>>, int>(const vector<int>&, const vector<int>&)>>> solvers = {
        {"(a) BFS", bfs_solve},
        {"(b) DFS", [](const vector<int>& s, const vector<int>& g) { return dfs_solve(s, g, 30); }},
        {"(c) A*", a_star_puzzle}
    };

    for (const auto& [label, solver] : solvers) {
        cout << string(60, '-') << '\n';
        cout << label << '\n';
        auto [path, nodes_expanded] = solver(START, GOAL);
        if (path.empty()) {
            cout << "    No solution found within limits." << '\n';
            continue;
        }
        cout << "    Solution length (moves): " << path.size() - 1 << '\n';
        cout << "    Nodes expanded         : " << nodes_expanded << '\n';
        cout << "    Path of states:" << '\n';
        for (size_t step = 0; step < path.size(); ++step) {
            cout << "    Step " << step << ":" << '\n';
            print_state(path[step]);
        }
    }
}

int main() {
    run_question2();
    return 0;
}
