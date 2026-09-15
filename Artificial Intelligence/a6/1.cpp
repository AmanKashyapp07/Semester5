/*
    Question 1 : Map Coloring CSP
    Regions   : A, B, C, D, E
    Colors    : Red, Green, Blue
    Solved two ways:
       1) Plain Backtracking Search   (static order A,B,C,D,E)
       2) MRV + Forward Checking      (dynamic order, degree heuristic for ties)
    Both versions print the full trace: every assignment tried,
    every failure, and every backtrack.
*/

#include <bits/stdc++.h>
using namespace std;

vector<string> VARS = {"A","B","C","D","E"};
vector<string> COLORS = {"Red","Green","Blue"};

map<string, vector<string>> NEIGH = {
    {"A", {"B","C"}},
    {"B", {"A","C","D","E"}},
    {"C", {"A","B","E"}},
    {"D", {"B","E"}},
    {"E", {"B","C","D"}}
};

int assignCount = 0, backtrackCount = 0;

bool consistent(const string &var, const string &val, map<string,string> &assign) {
    for (auto &nb : NEIGH[var])
        if (assign.count(nb) && assign[nb] == val) return false;
    return true;
}

/* ---------- 1) PLAIN BACKTRACKING (static order, no heuristics) ---------- */
bool plainBT(int idx, map<string,string> &assign) {
    if (idx == (int)VARS.size()) return true;
    string var = VARS[idx];
    cout << "\nAssigning variable: " << var << "\n";
    for (auto &val : COLORS) {
        assignCount++;
        cout << "  Try " << var << " = " << val;
        if (consistent(var, val, assign)) {
            cout << "  -> OK\n";
            assign[var] = val;
            if (plainBT(idx+1, assign)) return true;
            cout << "  Backtracking from " << var << " = " << val << "\n";
            assign.erase(var);
            backtrackCount++;
        } else {
            cout << "  -> conflict, reject\n";
        }
    }
    cout << "  No value works for " << var << " -> backtrack to previous variable\n";
    return false;
}

/* ---------- 2) MRV + FORWARD CHECKING ---------- */
bool fcMRV(map<string, vector<string>> &domains, map<string,string> &assign) {
    if (assign.size() == VARS.size()) return true;

    // pick unassigned variable with Minimum Remaining Values,
    // tie-break with degree heuristic (more neighbours first)
    string chosen;
    int best = INT_MAX, bestDeg = -1;
    for (auto &v : VARS) {
        if (assign.count(v)) continue;
        int sz = domains[v].size();
        int deg = NEIGH[v].size();
        if (sz < best || (sz == best && deg > bestDeg)) {
            best = sz; bestDeg = deg; chosen = v;
        }
    }

    cout << "\nMRV selects variable: " << chosen
         << "  (domain size = " << best << ", degree = " << bestDeg << ")\n";
    cout << "  Current domain of " << chosen << " = { ";
    for (auto &c : domains[chosen]) cout << c << " ";
    cout << "}\n";

    vector<string> myDomain = domains[chosen];
    for (auto &val : myDomain) {
        assignCount++;
        cout << "  Try " << chosen << " = " << val;
        if (!consistent(chosen, val, assign)) { cout << " -> conflict, reject\n"; continue; }
        cout << " -> OK, assign and forward-check\n";
        assign[chosen] = val;

        // save domains for backtracking, then prune neighbours
        map<string, vector<string>> saved = domains;
        bool wipeout = false;
        for (auto &nb : NEIGH[chosen]) {
            if (assign.count(nb)) continue;
            auto &d = domains[nb];
            auto it = remove(d.begin(), d.end(), val);
            if (it != d.end()) {
                d.erase(it, d.end());
                cout << "    Forward check: removed " << val << " from domain of "
                     << nb << " -> { ";
                for (auto &c : d) cout << c << " ";
                cout << "}\n";
            }
            if (d.empty()) { wipeout = true; }
        }

        if (!wipeout && fcMRV(domains, assign)) return true;

        if (wipeout) cout << "    Domain wipeout detected -> backtrack\n";
        cout << "  Backtracking from " << chosen << " = " << val << "\n";
        domains = saved;
        assign.erase(chosen);
        backtrackCount++;
    }
    return false;
}

void printSolution(map<string,string> &assign) {
    for (auto &v : VARS) cout << v << " = " << assign[v] << "\n";
}

int main() {
    cout << "=====================================================\n";
    cout << "PART 1: PLAIN BACKTRACKING SEARCH (order A,B,C,D,E)\n";
    cout << "=====================================================\n";
    assignCount = 0; backtrackCount = 0;
    map<string,string> assign1;
    plainBT(0, assign1);
    cout << "\n--- Plain Backtracking Result ---\n";
    printSolution(assign1);
    cout << "Total value-assignments tried : " << assignCount << "\n";
    cout << "Total backtracks              : " << backtrackCount << "\n";

    cout << "\n\n=====================================================\n";
    cout << "PART 2: MRV + FORWARD CHECKING\n";
    cout << "=====================================================\n";
    assignCount = 0; backtrackCount = 0;
    map<string, vector<string>> domains;
    for (auto &v : VARS) domains[v] = COLORS;
    map<string,string> assign2;
    fcMRV(domains, assign2);
    cout << "\n--- MRV + Forward Checking Result ---\n";
    printSolution(assign2);
    cout << "Total value-assignments tried : " << assignCount << "\n";
    cout << "Total backtracks              : " << backtrackCount << "\n";

    return 0;
}