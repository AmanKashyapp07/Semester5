#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int N = 10;

using Point = pair<int, int>;

// Movement: Up, Down, Left, Right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};


/*
    COST OF ENTERING A CELL

    R = Road     -> cost 1
    M = Mud      -> cost 3
    T = Terrain  -> cost 5
    G = Goal     -> cost 1
    X = Obstacle -> cannot enter

    UCS chooses the path with the
    minimum TOTAL cost.
*/
int cost(char cell) {

    switch (cell) {
        case 'R': return 1;
        case 'M': return 3;
        case 'T': return 5;
        case 'G': return 1;
        default:  return 1e9; // X or any other invalid cell
    }
}


int main() {

    /*
        GRID

        S = Start
        G = Goal
        R = Normal road
        M = Mud
        T = Difficult terrain
        X = Obstacle

        Problem:
        Find the minimum-cost path from S to G.
    */
    vector<vector<char>> grid = {

        {'S','R','R','T','T','R','R','R','R','R'},
        {'X','X','R','T','X','R','X','X','X','R'},
        {'R','R','R','T','R','R','R','R','X','R'},
        {'R','X','X','T','R','X','X','R','X','R'},
        {'R','R','R','R','R','R','X','R','R','R'},
        {'R','X','X','X','X','R','X','X','X','R'},
        {'R','R','R','R','X','R','R','R','R','R'},
        {'X','X','X','R','X','X','X','X','X','R'},
        {'R','R','R','R','R','R','R','R','R','R'},
        {'X','X','X','X','X','X','X','X','X','G'}
    };


    Point start = {0, 0};
    Point goal = {9, 9};


    /*
        UNIFORM COST SEARCH (UCS)
        -------------------------

        UCS always expands the node having
        the SMALLEST PATH COST.

        Priority Queue stores:

            {cost, position}

        Example:

            {3, (1,2)}
            {5, (2,3)}
            {8, (4,5)}

        Node with cost 3 is processed first.

        This is essentially Dijkstra's algorithm
        for non-negative edge costs.
    */

    using Node = pair<int, Point>;

    // Min-heap: smallest cost comes first
    priority_queue<Node,
                   vector<Node>,
                   greater<Node>> pq;


    /*
        gCost[r][c]

        Minimum known cost to reach cell (r,c).

        Initially:
            infinity for every cell.

        Start:
            cost = 0
    */
    vector<vector<int>> gCost(
        N, vector<int>(N, 1e9)
    );


    /*
        parent[r][c]

        Stores the previous cell from which
        we reached (r,c).

        Used later to reconstruct the path.
    */
    vector<vector<Point>> parent(
        N, vector<Point>(N, {-1, -1})
    );


    // Keeps track of cells already expanded
    vector<vector<bool>> visited(
        N, vector<bool>(N, false)
    );


    // Stores order in which UCS expands nodes
    vector<Point> order;


    // Start UCS from the starting cell
    pq.push({0, start});
    gCost[start.first][start.second] = 0;


    while (!pq.empty()) {

        // Get cell with minimum cost
        auto [currCost, curr] = pq.top();
        pq.pop();

        int r = curr.first;
        int c = curr.second;


        // Ignore if already processed
        if (visited[r][c])
            continue;

        visited[r][c] = true;

        // Save expansion order
        order.push_back(curr);


        // Goal reached
        if (curr == goal)
            break;


        /*
            Check all 4 neighboring cells.
        */
        for (int i = 0; i < 4; i++) {

            int nr = r + dr[i];
            int nc = c + dc[i];


            // Check:
            // 1. Inside grid
            // 2. Not an obstacle
            if (nr >= 0 && nr < N &&
                nc >= 0 && nc < N &&
                grid[nr][nc] != 'X') {


                /*
                    New cost =
                    cost so far + cost of entering
                    the neighboring cell.
                */
                int newCost =
                    currCost + cost(grid[nr][nc]);


                /*
                    If this is cheaper than the
                    previously known cost, update it.
                */
                if (newCost < gCost[nr][nc]) {

                    gCost[nr][nc] = newCost;

                    // Remember previous cell
                    parent[nr][nc] = {r, c};

                    // Add new state to priority queue
                    pq.push({
                        newCost,
                        {nr, nc}
                    });
                }
            }
        }
    }


    /*
        PRINT EXPANSION ORDER

        This shows the order in which UCS
        explored the cells.
    */
    cout << "--- Uniform Cost Search ---\n\n";

    cout << "Nodes expanded:\n";

    for (int i = 0; i < order.size(); i++) {

        cout << "("
             << order[i].first << ", "
             << order[i].second << ")";

        if (i + 1 < order.size())
            cout << " -> ";
    }

    cout << "\n";


    // Goal was never reached
    if (!visited[goal.first][goal.second]) {

        cout << "\nNo path exists.\n";
        return 0;
    }


    /*
        RECONSTRUCT PATH

        Start from G and repeatedly follow
        parent[] until we reach S.

        Example:

            G <- C <- B <- A <- S

        Reverse it:

            S -> A -> B -> C -> G
    */
    vector<Point> path;

    Point curr = goal;

    while (curr != start) {

        path.push_back(curr);

        curr = parent[
            curr.first
        ][curr.second];
    }

    // Add starting point
    path.push_back(start);

    // Convert G -> S into S -> G
    reverse(path.begin(), path.end());


    /*
        PRINT FINAL ANSWER
    */

    cout << "\nMinimum-cost path:\n";

    for (int i = 0; i < path.size(); i++) {

        cout << "("
             << path[i].first << ", "
             << path[i].second << ")";

        if (i + 1 < path.size())
            cout << " -> ";
    }

    cout << "\n";

    cout << "\nTotal movements: "
         << path.size() - 1 << "\n";

    cout << "Total path cost: "
         << gCost[goal.first][goal.second]
         << "\n";


    return 0;
}