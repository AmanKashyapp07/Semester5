"""
Q2. Rescue Robot using Depth-Limited Search (DLS)
--------------------------------------------------------------
DLS behaves like DFS but refuses to expand any node beyond a
user-specified depth limit L. It is implemented explicitly with
recursion (no ready-made graph-search library routine is used).
Successor order is fixed: Right, Down, Left, Up.
"""

BLOCKED = 'X'

# Fixed successor order: Right, Down, Left, Up
MOVES = [(0, 1), (1, 0), (0, -1), (-1, 0)]
MOVE_NAMES = ['Right', 'Down', 'Left', 'Up']

CUTOFF = "CUTOFF"
FAILURE = "FAILURE"
SUCCESS = "SUCCESS"


def find_symbol(grid, symbol):
    for r, row in enumerate(grid):
        for c, val in enumerate(row):
            if val == symbol:
                return (r, c)
    return None


def in_bounds(grid, r, c):
    return 0 <= r < len(grid) and 0 <= c < len(grid[0])


def depth_limited_search(grid, limit):
    """
    Returns (result, path, expansion_order, max_depth_reached)
    result is one of SUCCESS, CUTOFF, FAILURE
    """
    start = find_symbol(grid, 'S')
    goal = find_symbol(grid, 'G')
    if start is None or goal is None:
        raise ValueError("Grid must contain exactly one 'S' and one 'G'.")

    expansion_order = []
    max_depth_reached = [0]
    on_path = {start}          # states currently on the DFS path (cycle guard)

    def recurse(state, depth, path):
        expansion_order.append(state)
        max_depth_reached[0] = max(max_depth_reached[0], depth)

        if state == goal:
            return SUCCESS, list(path)

        if depth == limit:
            return CUTOFF, None

        cutoff_occurred = False
        r, c = state
        for (dr, dc), name in zip(MOVES, MOVE_NAMES):
            nr, nc = r + dr, c + dc
            if not in_bounds(grid, nr, nc):
                continue
            if grid[nr][nc] == BLOCKED:
                continue
            neighbour = (nr, nc)
            if neighbour in on_path:      # prevent cycles on current path
                continue

            on_path.add(neighbour)
            path.append(neighbour)

            result, found_path = recurse(neighbour, depth + 1, path)

            path.pop()
            on_path.discard(neighbour)

            if result == SUCCESS:
                return SUCCESS, found_path
            if result == CUTOFF:
                cutoff_occurred = True

        return (CUTOFF if cutoff_occurred else FAILURE), None

    result, path = recurse(start, 0, [start])
    return result, path, expansion_order, max_depth_reached[0]


def print_result(grid, limit):
    print(f"--- Depth-Limited Search: Limit = {limit} ---")
    result, path, expansion_order, max_depth = depth_limited_search(grid, limit)

    print("Nodes expanded (in order):")
    print(" -> ".join(str(n) for n in expansion_order))
    print(f"Maximum depth reached: {max_depth}")

    if result == SUCCESS:
        print("Result: SUCCESS")
        print("Route:")
        print(" -> ".join(str(n) for n in path))
        print(f"Solution depth / movements: {len(path) - 1}")
    elif result == CUTOFF:
        print("Result: CUTOFF / Goal not found within depth limit")
        print(f"Maximum allowed depth: {limit}")
    else:
        print("Result: FAILURE / Goal not reachable (exhausted within limit)")
    print()


# ---------------------------------------------------------------
# Example grid (10 x 10) taken from the assignment
# ---------------------------------------------------------------
example_grid = [
    ['S', 'R', 'R', 'X', 'R', 'R', 'R', 'R', 'R', 'R'],
    ['X', 'X', 'R', 'X', 'R', 'X', 'X', 'X', 'X', 'R'],
    ['R', 'R', 'R', 'X', 'R', 'R', 'R', 'R', 'X', 'R'],
    ['R', 'X', 'X', 'X', 'X', 'X', 'X', 'R', 'X', 'R'],
    ['R', 'R', 'R', 'R', 'R', 'R', 'X', 'R', 'R', 'R'],
    ['X', 'X', 'X', 'X', 'X', 'R', 'X', 'X', 'X', 'R'],
    ['R', 'R', 'R', 'R', 'X', 'R', 'R', 'R', 'R', 'R'],
    ['R', 'X', 'X', 'R', 'X', 'X', 'X', 'X', 'X', 'R'],
    ['R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'],
    ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'G'],
]


if __name__ == "__main__":
    print_result(example_grid, 18)   # Run 1
    print_result(example_grid, 25)   # Run 2

# ---------------------------------------------------------------
# Comparison / Discussion Questions
# ---------------------------------------------------------------
# 1. Why can DLS fail to find a goal even when a valid path exists?
#    DLS refuses to expand any node once its depth equals the limit L,
#    even if that node is not the goal. If every path from S to G in
#    the grid requires MORE than L moves (as with L = 18 above, where
#    the true solution needs 22 moves), the search hits CUTOFF along
#    every branch before it can ever reach G. The goal is reachable
#    in principle, but it lies outside the "horizon" the search is
#    allowed to look past.
#
# 2. How does increasing L affect time and memory usage?
#    Time: increasing L allows deeper branches to be explored, so in
#    the worst case the number of nodes expanded grows exponentially
#    with L (roughly b^L, where b is the branching factor). A larger
#    L that still fails to find the goal means MORE wasted work
#    exploring deep, fruitless branches before cutting off.
#    Memory: DLS is depth-first, so at any moment it only needs to
#    store the nodes on the current path (plus siblings not yet
#    explored) - memory usage grows LINEARLY with L, i.e. O(L) in
#    this grid, not exponentially. This is DLS's main practical
#    advantage over breadth-first search.
#
# 3. How is DLS different from ordinary DFS?
#    Ordinary DFS explores as deep as possible along each branch with
#    no limit, stopping only at dead ends (blocked cells/visited
#    states) - so it can wander arbitrarily deep, or never terminate
#    on an infinite/very large state space. DLS adds a hard depth
#    limit L: any branch reaching depth L is cut off immediately,
#    regardless of whether it dead-ends. This bounds DLS's runtime
#    and guarantees termination, at the cost of possibly missing an
#    existing but deeper solution.
#
# 4. Is DLS guaranteed to return the shortest path? Explain using your
#    observations.
#    No. DLS is not a cost- or length-optimal search - it simply
#    returns the FIRST goal-reaching path discovered under the fixed
#    successor order (Right, Down, Left, Up), stopping there. In the
#    Run 2 example, the route found needs 22 moves; the search never
#    verifies whether a shorter route to G exists elsewhere in the
#    grid, since as soon as one path to G is found it returns SUCCESS
#    without comparing it to alternative branches. Only search
#    strategies that explicitly compare path costs/lengths across all
#    candidates (e.g. BFS for step-count, or UCS/A* for weighted cost)
#    can guarantee optimality.