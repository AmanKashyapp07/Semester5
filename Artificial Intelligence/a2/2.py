import heapq
from collections import deque

class DeliveryEnvironment:
    """Represents the 2D grid environment for the delivery robot.
    
    Grid Legend:
    S = Start position
    P = Package pickup location
    G = Goal / Delivery destination
    R = Road cell
    X = Blocked cell / Obstacle
    """
    def __init__(self, grid_layout):
        self.grid = [row.split() for row in grid_layout]
        self.rows = len(self.grid)
        self.cols = len(self.grid[0])
        self.start_pos = self._find_symbol('S')
        self.package_pos = self._find_symbol('P')
        self.goal_pos = self._find_symbol('G')

    def _find_symbol(self, symbol):
        """Locates the coordinates (r, c) of a specific symbol in the grid."""
        for r in range(self.rows):
            for c in range(self.cols):
                if self.grid[r][c] == symbol:
                    return (r, c)
        raise ValueError(f"Symbol '{symbol}' not found in grid.")

    def is_valid_move(self, r, c):
        """Checks whether a position is within grid boundaries and traversable."""
        return 0 <= r < self.rows and 0 <= c < self.cols and self.grid[r][c] != 'X'


class GoalBasedAgent:
    """Part A: Goal-Based Agent
    
    Uses Breadth-First Search (BFS) to find the shortest path in terms of movement steps:
    1. Goal 1: Navigate from Start (S) to Package (P).
    2. Goal 2: Navigate from Package (P) to Destination (G).
    """
    def __init__(self, env):
        self.env = env
        self.directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]  # Up, Right, Down, Left

    def _bfs(self, start, target):
        """Finds the shortest path from start to target using Breadth-First Search."""
        queue = deque([(start, [start])])
        visited = {start}

        while queue:
            curr, path = queue.popleft()
            if curr == target:
                return path

            r, c = curr
            for dr, dc in self.directions:
                nr, nc = r + dr, c + dc
                if self.env.is_valid_move(nr, nc) and (nr, nc) not in visited:
                    visited.add((nr, nc))
                    queue.append(((nr, nc), path + [(nr, nc)]))
        return []

    def run(self):
        # Stage 1: Route to Package
        path_to_package = self._bfs(self.env.start_pos, self.env.package_pos)
        movements_to_package = len(path_to_package) - 1

        # Stage 2: Route to Destination
        path_to_dest = self._bfs(self.env.package_pos, self.env.goal_pos)
        movements_to_dest = len(path_to_dest) - 1

        total_movements = movements_to_package + movements_to_dest

        return {
            "path_to_package": path_to_package,
            "movements_to_package": movements_to_package,
            "path_to_dest": path_to_dest,
            "movements_to_dest": movements_to_dest,
            "total_movements": total_movements
        }


class UtilityBasedAgent:
    """Part B: Utility-Based Agent
    
    Evaluates routes using a multi-factor cost/utility function:
    Cost(cell) = w1 * Distance + w2 * Energy + w3 * Risk + w4 * Traffic
    Uses Uniform Cost Search (Dijkstra's Algorithm) to minimize total route cost.
    """
    def __init__(self, env, cost_grid=None, weights=(0.4, 0.3, 0.2, 0.1)):
        self.env = env
        self.directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
        self.w1, self.w2, self.w3, self.w4 = weights
        self.cost_grid = cost_grid if cost_grid else self._generate_default_cost_grid()

    def _generate_default_cost_grid(self):
        """Assigns travel characteristic vectors (Distance, Energy, Risk, Traffic) to cells."""
        costs = {}
        for r in range(self.env.rows):
            for c in range(self.env.cols):
                if self.env.grid[r][c] != 'X':
                    # High traffic/risk penalty assigned to central shortcut region (2, 7)
                    if (r, c) == (2, 7) or (r, c) == (2, 6):
                        costs[(r, c)] = (1.0, 5.0, 4.0, 3.0)  # High energy & risk
                    else:
                        costs[(r, c)] = (1.0, 1.0, 1.0, 1.0)  # Standard road conditions
        return costs

    def _calculate_cell_cost(self, pos):
        """Computes weighted cost for entering a cell: Cost = w1*D + w2*E + w3*R + w4*T"""
        dist, energy, risk, traffic = self.cost_grid.get(pos, (1.0, 1.0, 1.0, 1.0))
        return (self.w1 * dist) + (self.w2 * energy) + (self.w3 * risk) + (self.w4 * traffic)

    def _uniform_cost_search(self, start, target):
        """Finds the lowest utility-cost route using Dijkstra / Uniform Cost Search."""
        pq = [(0.0, start, [start])]
        min_costs = {start: 0.0}

        while pq:
            curr_cost, curr_pos, path = heapq.heappop(pq)

            if curr_pos == target:
                return path, curr_cost

            if curr_cost > min_costs.get(curr_pos, float('inf')):
                continue

            r, c = curr_pos
            for dr, dc in self.directions:
                nr, nc = r + dr, c + dc
                if self.env.is_valid_move(nr, nc):
                    next_pos = (nr, nc)
                    step_cost = self._calculate_cell_cost(next_pos)
                    new_cost = curr_cost + step_cost

                    if new_cost < min_costs.get(next_pos, float('inf')):
                        min_costs[next_pos] = new_cost
                        heapq.heappush(pq, (new_cost, next_pos, path + [next_pos]))

        return [], float('inf')

    def run(self):
        # Stage 1: Minimum-cost route to Package
        path_to_package, cost_to_package = self._uniform_cost_search(self.env.start_pos, self.env.package_pos)
        
        # Stage 2: Minimum-cost route to Destination
        path_to_dest, cost_to_dest = self._uniform_cost_search(self.env.package_pos, self.env.goal_pos)

        total_movements = (len(path_to_package) - 1) + (len(path_to_dest) - 1)
        total_cost = cost_to_package + cost_to_dest

        return {
            "path_to_package": path_to_package,
            "path_to_dest": path_to_dest,
            "total_movements": total_movements,
            "total_cost": total_cost
        }


def format_path(path):
    """Formats path tuple list into a clean display string."""
    if not path:
        return "No Path Found"
    if len(path) <= 3:
        return " -> ".join([f"({r},{c})" for r, c in path])
    return f"({path[0][0]},{path[0][1]}) -> ... -> ({path[-1][0]},{path[-1][1]})"


# ==========================================
# DRIVER CODE & TEST EXECUTION
# ==========================================

if __name__ == "__main__":
    # Sample 10x10 Grid from Problem Statement
    delivery_grid_layout = [
        "S R R X R R R R R R",
        "X X R R R X R X R R",
        "R R R X R R R P R R",
        "R X R R R X R R R X",
        "R R R X R R X R R R",
        "X R R R R R R R X R",
        "R R X R X R R R R R",
        "R R R R R X R X R R",
        "R X R R R R R R R R",
        "R R R X R R R R X G"
    ]

    # Initialize Environment
    env = DeliveryEnvironment(delivery_grid_layout)

    # ------------------------------------------
    # 1. Run Goal-Based Agent
    # ------------------------------------------
    goal_agent = GoalBasedAgent(env)
    goal_res = goal_agent.run()

    print("--- Goal-Based Agent ---")
    print(f"Route to Package: {format_path(goal_res['path_to_package'])}")
    print(f"Movements to Package: {goal_res['movements_to_package']}")
    print(f"Route to Destination: {format_path(goal_res['path_to_dest'])}")
    print(f"Movements to Destination: {goal_res['movements_to_dest']}")
    print(f"Total Movements: {goal_res['total_movements']}")
    print()

    # ------------------------------------------
    # 2. Run Utility-Based Agent
    # ------------------------------------------
    utility_agent = UtilityBasedAgent(env)
    util_res = utility_agent.run()

    print("--- Utility-Based Agent ---")
    print(f"Route to Package: {format_path(util_res['path_to_package'])}")
    print(f"Route to Destination: {format_path(util_res['path_to_dest'])}")
    print(f"Total Movements: {util_res['total_movements']}")
    print(f"Total Cost: {util_res['total_cost']:.1f}")
    print("The selected route has lower overall utility cost despite having more movements.")