#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <random>
#include <numeric>

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

    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Calculate total route cost:
// W -> all locations in route -> W
double routeCost(const vector<string>& r) {
    // Distance from W to the first location
    double total = dist("W", r.front());

    // Add distances between consecutive locations
    for (int i = 0; i + 1 < (int)r.size(); i++)
        total += dist(r[i], r[i + 1]);

    // Return from the last location back to W
    return total + dist(r.back(), "W");
}

// Steepest-descent Hill Climbing
// Returns: {final minimum cost found, number of iterations}
pair<double, int> hillClimb(vector<string> route) {
    // Calculate cost of the starting route
    double cost = routeCost(route);
    int iter = 0;

    while (true) {
        // Assume the current route is the best initially
        vector<string> best = route;
        double bestCost = cost;

        // Try every possible pairwise swap
        for (int i = 0; i < (int)route.size(); i++) {
            for (int j = i + 1; j < (int)route.size(); j++) {

                // Create a neighboring route by swapping two locations
                swap(route[i], route[j]);

                // Calculate cost of this neighboring route
                double c = routeCost(route);

                // Keep it if it is better than the best neighbor found so far
                if (c < bestCost) {
                    bestCost = c;
                    best = route;
                }

                // Undo the swap to restore the original route
                swap(route[i], route[j]);
            }
        }

        // Stop if no neighboring route is better
        // This means we reached a local optimum
        if (bestCost >= cost) break;

        // Move to the best neighboring solution
        route = best;
        cost = bestCost;
        iter++;
    }

    return {cost, iter};
}

// Simulated Annealing
// Returns: {best cost found, number of iterations}
pair<double, int> simAnneal(
    vector<string> route,
    mt19937& rng,
    double T = 100.0,      // Initial temperature
    double alpha = 0.95,   // Cooling rate
    double tMin = 0.1      // Minimum temperature
) {
    // Current route cost
    double cost = routeCost(route);

    // Best cost found during the entire search
    double bestCost = cost;

    // Randomly choose route positions
    uniform_int_distribution<int> idx(0, route.size() - 1);

    // Random value between 0 and 1 for probability-based acceptance
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
        // deltaE < 0 means the new route is better
        double deltaE = newCost - cost;

        // Accept if:
        // 1. The new solution is better, OR
        // 2. It is worse but passes the probability test
        if (deltaE < 0 || unif(rng) < exp(-deltaE / T)) {
            // Keep the new route
            cost = newCost;

            // Update the best cost found so far
            bestCost = min(bestCost, cost);
        } else {
            // Reject the route and undo the swap
            swap(route[i], route[j]);
        }

        // Gradually reduce the temperature
        T *= alpha;
    }

    return {bestCost, iter};
}

int main() {
    // Random number generator with fixed seed
    mt19937 rng(42);

    // Run both algorithms multiple times for comparison
    const int runs = 5;

    // Store costs and iterations of Hill Climbing
    vector<double> hcCosts;
    vector<int> hcIters;

    // Store costs and iterations of Simulated Annealing
    vector<double> saCosts;
    vector<int> saIters;

    cout << "Run  HC_Cost   SA_Cost   HC_Iters  SA_Iters\n";

    // Perform multiple independent runs
    for (int run = 1; run <= runs; run++) {

        // Create all locations
        vector<string> route = {"A", "B", "C", "D", "E", "F", "G"};

        // Generate a random starting route
        shuffle(route.begin(), route.end(), rng);

        // Run Hill Climbing from the same initial route
        auto [hc, hi] = hillClimb(route);

        // Run Simulated Annealing from the same initial route
        auto [sa, si] = simAnneal(route, rng);

        // Store results for later analysis
        hcCosts.push_back(hc);
        saCosts.push_back(sa);

        hcIters.push_back(hi);
        saIters.push_back(si);

        // Print results of this run
        cout << run << "    "
             << hc << "   "
             << sa << "   "
             << hi << "   "
             << si << "\n";
    }

    // Lambda function to calculate average of double values
    auto avg = [](const vector<double>& v) {
        return accumulate(v.begin(), v.end(), 0.0) / v.size();
    };

    // Lambda function to calculate average of integer values
    auto avgI = [](const vector<int>& v) {
        return accumulate(v.begin(), v.end(), 0.0) / v.size();
    };

    // Print Hill Climbing statistics
    cout << "\nHC: best="
         << *min_element(hcCosts.begin(), hcCosts.end())
         << " worst="
         << *max_element(hcCosts.begin(), hcCosts.end())
         << " avg="
         << avg(hcCosts)
         << " avgIters="
         << avgI(hcIters)
         << "\n";

    // Print Simulated Annealing statistics
    cout << "SA: best="
         << *min_element(saCosts.begin(), saCosts.end())
         << " worst="
         << *max_element(saCosts.begin(), saCosts.end())
         << " avg="
         << avg(saCosts)
         << " avgIters="
         << avgI(saIters)
         << "\n";

    return 0;
}
