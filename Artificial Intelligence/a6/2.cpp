/*
    Question 2 : University Course Scheduling CSP
    Courses   : AI, DBMS, OS, CN
    Slots     : 1, 2, 3, 4
    Constraints (not-equal, "conflict" graph):
        AI  -- DBMS
        AI  -- OS
        DBMS-- CN
        OS  -- CN
    Domains:
        AI   : {1,2,3}
        DBMS : {1,2,4}
        OS   : {2,3,4}
        CN   : {1,3,4}

    Solved two ways:
       1) Plain Backtracking Search (static order AI,DBMS,OS,CN)
       2) MRV + Forward Checking (dynamic order)
*/

#include <bits/stdc++.h>
using namespace std;

vector<string> VARS = {"AI","DBMS","OS","CN"};

map<string, vector<int>> BASE_DOMAIN = {
    {"AI",   {1,2,3}},
    {"DBMS", {1,2,4}},
    {"OS",   {2,3,4}},
    {"CN",   {1,3,4}}
};

map<string, vector<string>> NEIGH = {
    {"AI",   {"DBMS","OS"}},
    {"DBMS", {"AI","CN"}},
    {"OS",   {"AI","CN"}},
    {"CN",   {"DBMS","OS"}}
};

int assignCount = 0, backtrackCount = 0;

bool consistent(const string &var, int val, map<string,int> &assign) {
    for (auto &nb : NEIGH[var])
        if (assign.count(nb) && assign[nb] == val) return false;
    return true;
}

/* ---------- 1) PLAIN BACKTRACKING (static order, no forward checking) ---------- */
bool plainBT(int idx, map<string,int> &assign) {
    if (idx == (int)VARS.size()) return true;
    string var = VARS[idx];
    cout << "\nAssigning variable: " << var << "\n";
    for (int val : BASE_DOMAIN[var]) {
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
void printDomains(map<string, vector<int>> &domains) {
    for (auto &v : VARS) {
        cout << "      " << v << " : { ";
        for (int c : domains[v]) cout << c << " ";
        cout << "}\n";
    }
}

bool fcMRV(map<string, vector<int>> &domains, map<string,int> &assign, int step) {
    if (assign.size() == VARS.size()) return true;

    string chosen;
    int best = INT_MAX;
    for (auto &v : VARS) {
        if (assign.count(v)) continue;
        int sz = domains[v].size();
        if (sz < best) { best = sz; chosen = v; }
    }

    cout << "\n--- Step " << step << " ---\n";
    cout << "1. Variable selected using MRV : " << chosen
         << "  (remaining domain size = " << best << ")\n";
    cout << "   Domain of " << chosen << " before assignment = { ";
    for (int c : domains[chosen]) cout << c << " ";
    cout << "}\n";

    vector<int> myDomain = domains[chosen];
    for (int val : myDomain) {
        assignCount++;
        cout << "2. Value assigned (trying)     : " << chosen << " = " << val;
        if (!consistent(chosen, val, assign)) { cout << "  -> conflict, reject\n"; continue; }
        cout << "  -> OK\n";
        assign[chosen] = val;

        map<string, vector<int>> saved = domains;
        bool wipeout = false;
        for (auto &nb : NEIGH[chosen]) {
            if (assign.count(nb)) continue;
            auto &d = domains[nb];
            auto it = remove(d.begin(), d.end(), val);
            if (it != d.end()) d.erase(it, d.end());
            if (d.empty()) wipeout = true;
        }
        cout << "3. Updated domains after Forward Checking:\n";
        printDomains(domains);

        if (wipeout) {
            cout << "4. Domain wipeout -> Backtracking required : YES\n";
            domains = saved;
            assign.erase(chosen);
            backtrackCount++;
            continue;
        }

        if (fcMRV(domains, assign, step+1)) {
            cout << "4. Backtracking required : NO\n";
            return true;
        }

        cout << "   Backtracking from " << chosen << " = " << val << "\n";
        domains = saved;
        assign.erase(chosen);
        backtrackCount++;
    }
    return false;
}

void printSolution(map<string,int> &assign) {
    for (auto &v : VARS) cout << v << " = slot " << assign[v] << "\n";
}

int main() {
    cout << "=====================================================\n";
    cout << "PART 1: PLAIN BACKTRACKING SEARCH (order AI,DBMS,OS,CN)\n";
    cout << "=====================================================\n";
    assignCount = 0; backtrackCount = 0;
    map<string,int> assign1;
    plainBT(0, assign1);
    cout << "\n--- Plain Backtracking Result ---\n";
    printSolution(assign1);
    cout << "Total value-assignments tried : " << assignCount << "\n";
    cout << "Total backtracks              : " << backtrackCount << "\n";

    cout << "\n\n=====================================================\n";
    cout << "PART 2: MRV + FORWARD CHECKING\n";
    cout << "=====================================================\n";
    assignCount = 0; backtrackCount = 0;
    map<string, vector<int>> domains = BASE_DOMAIN;
    map<string,int> assign2;
    fcMRV(domains, assign2, 1);
    cout << "\n--- MRV + Forward Checking Result ---\n";
    printSolution(assign2);
    cout << "Total value-assignments tried : " << assignCount << "\n";
    cout << "Total backtracks              : " << backtrackCount << "\n";

    return 0;
}