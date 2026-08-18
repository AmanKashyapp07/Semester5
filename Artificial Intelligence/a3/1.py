"""
Q1. Cost-Aware Delivery Robot using Uniform Cost Search (UCS)
--------------------------------------------------------------
UCS expands the frontier node with the smallest accumulated path
cost g(n), using a priority queue (min-heap). If a cheaper path to
an already-discovered cell is found, its cost/parent are updated.
"""

import heapq

# ---------------------------------------------------------------
# Terrain costs (cost to ENTER a cell of this type)
# ---------------------------------------------------------------
TERRAIN_COST = {
    'S': 0,   # starting cell itself costs 0 to "enter"
    'G': 1,   # goal is a normal traversable cell
    'R': 1,   # normal road
    'M': 3,   # moderate-cost / rough surface
    'T': 5,   # traffic-congested / high-cost cell
}
BLOCKED = 'X'

# Four-directional moves: (dr, dc)
MOVES = [(-1, 0), (1, 0), (0, -1), (0, 1)]   # up, down, left, right


def find_symbol(grid, symbol):
    for r, row in enumerate(grid):
        for c, val in enumerate(row):
            if val == symbol:
                return (r, c)
    return None


def in_bounds(grid, r, c):
    return 0 <= r < len(grid) and 0 <= c < len(grid[0])


def uniform_cost_search(grid):
    start = find_symbol(grid, 'S')
    goal = find_symbol(grid, 'G')

    if start is None or goal is None:
        raise ValueError("Grid must contain exactly one 'S' and one 'G'.")

    # g(n): best known accumulated cost to reach a state
    g_cost = {start: 0}
    parent = {start: None}

    # Priority queue entries: (g(n), state)
    frontier = [(0, start)]
    visited = set()          # states that have been EXPANDED (finalized)
    expansion_order = []

    while frontier:
        cost_so_far, current = heapq.heappop(frontier)

        # Skip stale/outdated entries left in the heap
        if current in visited:
            continue

        visited.add(current)
        expansion_order.append(current)

        if current == goal:
            break

        r, c = current
        for dr, dc in MOVES:
            nr, nc = r + dr, c + dc
            if not in_bounds(grid, nr, nc):
                continue
            cell_symbol = grid[nr][nc]
            if cell_symbol == BLOCKED:
                continue

            neighbour = (nr, nc)
            step_cost = TERRAIN_COST[cell_symbol]
            new_cost = cost_so_far + step_cost

            if neighbour not in g_cost or new_cost < g_cost[neighbour]:
                g_cost[neighbour] = new_cost
                parent[neighbour] = current
                heapq.heappush(frontier, (new_cost, neighbour))

    if goal not in visited:
        return None, None, expansion_order

    # Reconstruct path from goal back to start
    path = []
    node = goal
    while node is not None:
        path.append(node)
        node = parent[node]
    path.reverse()

    return path, g_cost[goal], expansion_order


def print_result(grid):
    start = find_symbol(grid, 'S')
    goal = find_symbol(grid, 'G')

    print("--- Uniform Cost Search ---")
    print(f"Start: {start}")
    print(f"Goal: {goal}")

    path, total_cost, expansion_order = uniform_cost_search(grid)

    print("\nNodes expanded (in order):")
    print(" -> ".join(str(n) for n in expansion_order))

    if path is None:
        print("\nNo path exists")
        return

    print("\nMinimum-cost route:")
    print(" -> ".join(str(n) for n in path))
    print(f"\nTotal movements: {len(path) - 1}")
    print(f"Total path cost: {total_cost}")


# ---------------------------------------------------------------
# Example grid (10 x 10) taken from the assignment
# ---------------------------------------------------------------
example_grid = [
    ['S', 'R', 'R', 'T', 'T', 'R', 'R', 'R', 'R', 'R'],
    ['X', 'X', 'R', 'T', 'X', 'R', 'X', 'X', 'X', 'R'],
    ['R', 'R', 'R', 'T', 'R', 'R', 'R', 'R', 'X', 'R'],
    ['R', 'X', 'X', 'T', 'R', 'X', 'X', 'R', 'X', 'R'],
    ['R', 'R', 'R', 'R', 'R', 'R', 'X', 'R', 'R', 'R'],
    ['R', 'X', 'X', 'X', 'X', 'R', 'X', 'X', 'X', 'R'],
    ['R', 'R', 'R', 'R', 'X', 'R', 'R', 'R', 'R', 'R'],
    ['X', 'X', 'X', 'R', 'X', 'X', 'X', 'X', 'X', 'R'],
    ['R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'],
    ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'G'],
]


if __name__ == "__main__":
    print_result(example_grid)