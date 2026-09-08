#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

map<string, pair<double,double>> coords = {
    {"W", {0,0}}, {"A", {2,6}}, {"B", {5,2}}, {"C", {6,7}},
    {"D", {8,3}}, {"E", {1,4}}, {"F", {7,6}}, {"G", {3,1}}
};

double dist(const string& a, const string& b) {
    auto [x1,y1] = coords[a];
    auto [x2,y2] = coords[b];
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
} // returns distance between two points

double routeCost(const vector<string>& r) {
    double total = dist("W", r.front());
    for (int i = 0; i + 1 < (int)r.size(); i++) total += dist(r[i], r[i+1]);
    return total + dist(r.back(), "W");
} // returns total cost of the route W->...->W

string routeStr(const vector<string>& r) {
    string s = "W";
    for (auto& loc : r) s += " -> " + loc;
    return s + " -> W";
} // just for printing the route

int main() {
    vector<string> route = {"A","B","C","D","E","F","G"};
    shuffle(route.begin(), route.end(), mt19937(42));

    double cost = routeCost(route);
    cout << "Initial Route: " << routeStr(route) << "  Cost: " << cost << "\n\n";

    int iter = 0;
    while (true) {
        vector<string> best = route;
        double bestCost = cost;
        int n = (int)route.size();
        for(int i = 0; i < n; i++) {
            for(int j = i + 1; j < n; j++) {
                swap(route[i], route[j]);
               // double newCost = routeCost(route); // calculate cost after swapping, we can optimise by only calculating the difference in cost due to the swap, but for simplicity we recalculate the whole route cost
                double oldCost = dist(route[i], route[(i+1)%n]) + dist(route[(i-1+n)%n], route[i]) 
                            + dist(route[j], route[(j+1)%n]) + dist(route[(j-1+n)%n], route[j]);
                double newCost = dist(route[j], route[(i+1)%n]) + dist(route[(i-1+n)%n], route[j]) 
                            + dist(route[i], route[(j+1)%n]) + dist(route[(j-1+n)%n], route[i]);
                double newTotalCost = cost + -oldCost + newCost;
                // we are only calculating the cost difference due to the swap, which is more efficient than recalculating the entire route cost, first we will subtract the cost of the edges that are being removed due to the swap, and then we will add the cost of the new edges that are being created due to the swap
                if (newTotalCost < bestCost) {
                    bestCost = newTotalCost;
                    best = route;
                }
                swap(route[i], route[j]); // swap back
            }
        }
        if (bestCost >= cost) break; // no improvement found

        route = best;
        cost = bestCost;
        cout << "Iter " << ++iter << ": " << routeStr(route) << "  Cost: " << cost << "\n";
    } // this loop is for finding the best route by swapping pairs of locations, it will run until no improvement can be found, and it will print the best route found in each iteration, in each loop iteration, we will try to swap every pair of locations in the route, and if the new route has a lower cost than the current best route, we will update the best route and best cost, if no improvement can be found, we will break out of the loop

    cout << "\nFinal Route: " << routeStr(route) << "  Cost: " << cost;
    cout << "\nIterations: " << iter << "\n";
    return 0;
}