#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int N = 10;

// Movement priority: Right -> Down -> Left -> Up
int dr[] = {0, 1, 0, -1};
int dc[] = {1, 0, -1, 0};

using Point = pair<int, int>;


/*
    DLS RESULT

    SUCCESS = Goal found
    CUTOFF  = Search stopped because depth limit was reached
    FAILURE = No path exists
*/
const int SUCCESS = 1;
const int CUTOFF = 0;
const int FAILURE = -1;


// Stores the cells explored by DLS
vector<Point> expanded;

// Stores the current path
vector<Point> solution;

// Prevents the algorithm from visiting
// the same cell again in the CURRENT path
bool onPath[N][N];


/*
    DEPTH-LIMITED SEARCH (DLS)
    --------------------------

    DLS is basically DFS + a depth limit.

    DFS:
        Keep going deeper.

    DLS:
        Keep going deeper ONLY until
        depth == limit.

    Example:

        S
        |
        A       depth = 1
        |
        B       depth = 2
        |
        G       depth = 3

    If limit = 2:
        G will NOT be searched.

    If limit = 3:
        G can be found.
*/
int dls(const vector<vector<char>>& grid,
        Point curr,
        Point goal,
        int depth,
        int limit,
        vector<Point>& path) {

    // Record that this node was expanded
    expanded.push_back(curr);


    // Goal found
    if (curr == goal) {

        solution = path;

        return SUCCESS;
    }


    /*
        If depth limit has been reached,
        don't go any deeper.

        This is the main difference
        between DFS and DLS.
    */
    if (depth == limit)
        return CUTOFF;


    bool cutoffFound = false;

    int r = curr.first;
    int c = curr.second;


    /*
        Try all four possible movements.

        Order:
            Right
            Down
            Left
            Up
    */
    for (int i = 0; i < 4; i++) {

        int nr = r + dr[i];
        int nc = c + dc[i];


        /*
            Move only if:
            1. Inside grid
            2. Not an obstacle
            3. Not already present in current path
        */
        if (nr >= 0 && nr < N &&
            nc >= 0 && nc < N &&
            grid[nr][nc] != 'X' &&
            !onPath[nr][nc]) {


            // Mark cell as part of current path
            onPath[nr][nc] = true;

            path.push_back({nr, nc});


            // Recursively search next cell
            int result = dls(
                grid,
                {nr, nc},
                goal,
                depth + 1,
                limit,
                path
            );


            // Backtrack
            path.pop_back();
            onPath[nr][nc] = false;


            // Goal found
            if (result == SUCCESS)
                return SUCCESS;


            // At least one branch hit depth limit
            if (result == CUTOFF)
                cutoffFound = true;
        }
    }


    /*
        If any branch was stopped because of
        depth limit -> return CUTOFF.

        Otherwise -> no path exists from here.
    */
    return cutoffFound ? CUTOFF : FAILURE;
}


/*
    RUN DLS WITH A GIVEN DEPTH LIMIT
*/
void runDLS(const vector<vector<char>>& grid, int limit) {

    Point start = {0, 0};
    Point goal = {9, 9};


    cout << "--- Depth-Limited Search ---\n";
    cout << "Depth Limit: " << limit << "\n";


    // Reset previous run
    expanded.clear();
    solution.clear();

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            onPath[i][j] = false;


    // Starting cell belongs to current path
    onPath[start.first][start.second] = true;

    vector<Point> path = {start};


    // Start DLS
    int result = dls(
        grid,
        start,
        goal,
        0,          // Starting depth
        limit,
        path
    );


    /*
        PRINT EXPANSION ORDER
    */
    cout << "Nodes expanded:\n";

    for (int i = 0; i < expanded.size(); i++) {

        cout << "("
             << expanded[i].first << ", "
             << expanded[i].second << ")";

        if (i + 1 < expanded.size())
            cout << " -> ";
    }

    cout << "\n";


    /*
        PRINT RESULT
    */

    if (result == SUCCESS) {

        cout << "Result: SUCCESS\n";

        cout << "Route:\n";

        for (int i = 0; i < solution.size(); i++) {

            cout << "("
                 << solution[i].first << ", "
                 << solution[i].second << ")";

            if (i + 1 < solution.size())
                cout << " -> ";
        }

        cout << "\n";

        cout << "Movements: "
             << solution.size() - 1 << "\n";
    }

    else if (result == CUTOFF) {

        cout << "Result: CUTOFF\n";
        cout << "Goal was not found within depth "
             << limit << "\n";
    }

    else {

        cout << "Result: FAILURE\n";
        cout << "Goal is not reachable.\n";
    }

    cout << "\n";
}


int main() {

    /*
        GRID

        S = Start
        G = Goal
        R = Normal road
        X = Obstacle

        Task:
        Find a path from S -> G.

        DLS will search using DFS,
        but it cannot go deeper than
        the specified depth limit.
    */

    vector<vector<char>> grid = {

        {'S','R','R','X','R','R','R','R','R','R'},
        {'X','X','R','X','R','X','X','X','X','R'},
        {'R','R','R','X','R','R','R','R','X','R'},
        {'R','X','X','X','X','X','X','R','X','R'},
        {'R','R','R','R','R','R','X','R','R','R'},
        {'X','X','X','X','X','R','X','X','X','R'},
        {'R','R','R','R','X','R','R','R','R','R'},
        {'R','X','X','R','X','X','X','X','X','R'},
        {'R','R','R','R','R','R','R','R','R','R'},
        {'X','X','X','X','X','X','X','X','X','G'}
    };


    /*
        Run DLS twice.

        First:
            limit = 18

        Second:
            limit = 25

        If the solution needs more than 18
        movements, the first run will return
        CUTOFF.

        The second run may find the goal.
    */

    runDLS(grid, 18);

    runDLS(grid, 25);


    return 0;
}