#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

// Store coordinates of warehouse W and all locations
map<string, pair<double, double>> coords = {
    {"W", {0, 0}}, {"A", {2, 6}}, {"B", {5, 2}}, {"C", {6, 7}},
    {"D", {8, 3}}, {"E", {1, 4}}, {"F", {7, 6}}, {"G", {3, 1}}
};

// Calculate Euclidean distance between two locations
double dist(const string& a, const string& b) {
    auto [x1, y1] = coords[a];
    auto [x2, y2] = coords[b];

    return sqrt((x1 - x2) * (x1 - x2) +
                (y1 - y2) * (y1 - y2));
}

// Calculate total cost of a route:
// W -> all locations in route -> W
double routeCost(const vector<string>& r) {
    // Distance from W to first location
    double total = dist("W", r.front());

    // Distance between consecutive locations
    for (int i = 0; i + 1 < (int)r.size(); i++)
        total += dist(r[i], r[i + 1]);

    // Distance from last location back to W
    return total + dist(r.back(), "W");
}

// Convert route into a readable string for printing
// Example: W -> A -> B -> C -> W
string routeStr(const vector<string>& r) {
    string s = "W";

    for (auto& loc : r)
        s += " -> " + loc;

    return s + " -> W";
}

int main() {
    // Simulated Annealing parameters
    double T = 100.0;      // Initial temperature
    double alpha = 0.95;   // Cooling rate
    double tMin = 0.1;     // Stop when temperature becomes smaller than this

    // Random number generator with fixed seed
    mt19937 rng(42);

    // Create initial route containing all locations
    vector<string> route = {"A", "B", "C", "D", "E", "F", "G"};

    // Randomly shuffle to get the starting solution
    shuffle(route.begin(), route.end(), rng);

    // Calculate cost of the initial route
    double cost = routeCost(route);

    // Store the best route found so far
    vector<string> best = route;
    double bestCost = cost;

    cout << "Initial Route: " << routeStr(route)
         << "  Cost: " << cost << "\n\n";

    // Random distribution for choosing two route positions
    uniform_int_distribution<int> idx(0, route.size() - 1);

    // Random number between 0 and 1 for probability-based acceptance
    uniform_real_distribution<double> unif(0.0, 1.0);

    int iter = 0;

    // Continue until temperature becomes too low
    while (T >= tMin) {
        iter++;

        // Choose two different random positions
        int i = idx(rng), j = idx(rng);
        while (i == j)
            j = idx(rng);

        // Create a neighboring solution by swapping two locations
        swap(route[i], route[j]);

        // Calculate cost of the new route
        double newCost = routeCost(route);

        // Change in cost
        // Negative deltaE means the new route is better
        double deltaE = newCost - cost;

        // Accept if:
        // 1. New route is better, OR
        // 2. New route is worse but passes the probability test
        bool accept = deltaE < 0 || unif(rng) < exp(-deltaE / T);

        if (accept) {
            // Keep the new route
            cost = newCost;

            // Update the best solution if this is the best route so far
            if (cost < bestCost) {
                bestCost = cost;
                best = route;
            }
        } else {
            // Reject the new route and restore the previous route
            swap(route[i], route[j]);
        }

        // Print progress of each iteration
        cout << "Iter " << iter
             << "  T=" << T
             << "  " << (accept ? "ACCEPTED" : "REJECTED")
             << "  Cost=" << cost
             << "  Best=" << bestCost << "\n";

        // Reduce temperature (cooling process)
        T *= alpha;
    }

    // Print the best route found
    cout << "\nBest Route: " << routeStr(best)
         << "  Cost: " << bestCost;

    cout << "\nIterations: " << iter << "\n";

    return 0;
}