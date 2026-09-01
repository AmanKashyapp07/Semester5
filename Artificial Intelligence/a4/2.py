import heapq
from collections import deque

START = (1, 2, 3, '#', 4, 6, 7, 5, 8)
GOAL = (1, 2, 3, 4, 5, 6, 7, 8, '#')


def get_neighbors(state):
    neighbors = []
    blank = state.index('#')
    row, col = divmod(blank, 3)

    moves = []
    if row > 0:
        moves.append(-3)
    if row < 2:
        moves.append(3)
    if col > 0:
        moves.append(-1)
    if col < 2:
        moves.append(1)

    for m in moves:
        new_blank = blank + m
        state_list = list(state)
        state_list[blank], state_list[new_blank] = state_list[new_blank], state_list[blank]
        neighbors.append(tuple(state_list))

    return neighbors


def print_state(state):
    for i in range(0, 9, 3):
        print("   ", state[i:i + 3])
    print()


def reconstruct_path(came_from, state):
    path = [state]
    while state in came_from:
        state = came_from[state]
        path.append(state)
    path.reverse()
    return path


def bfs_solve(start, goal):
    frontier = deque([start])
    came_from = {}
    visited = {start}
    nodes_expanded = 0

    while frontier:
        state = frontier.popleft()
        nodes_expanded += 1

        if state == goal:
            return reconstruct_path(came_from, state), nodes_expanded

        for nxt in get_neighbors(state):
            if nxt not in visited:
                visited.add(nxt)
                came_from[nxt] = state
                frontier.append(nxt)

    return None, nodes_expanded


def dfs_solve(start, goal, depth_limit=30):
    stack = [(start, 0)]
    came_from = {}
    visited = {start}
    nodes_expanded = 0

    while stack:
        state, depth = stack.pop()
        nodes_expanded += 1

        if state == goal:
            return reconstruct_path(came_from, state), nodes_expanded

        if depth >= depth_limit:
            continue

        for nxt in get_neighbors(state):
            if nxt not in visited:
                visited.add(nxt)
                came_from[nxt] = state
                stack.append((nxt, depth + 1))

    return None, nodes_expanded


def misplaced_tiles(state, goal):
    return sum(1 for s, g in zip(state, goal) if s != g and s != '#')


def a_star_puzzle(start, goal):
    counter = 0
    g_score = {start: 0}
    came_from = {}
    h0 = misplaced_tiles(start, goal)
    open_set = [(h0, counter, start)]
    open_set_tracker = {start}
    closed = set()
    nodes_expanded = 0

    while open_set:
        f, _, state = heapq.heappop(open_set)
        open_set_tracker.discard(state)
        nodes_expanded += 1

        if state == goal:
            return reconstruct_path(came_from, state), nodes_expanded

        closed.add(state)

        for nxt in get_neighbors(state):
            tentative_g = g_score[state] + 1
            if nxt in closed and tentative_g >= g_score.get(nxt, float('inf')):
                continue
            if tentative_g < g_score.get(nxt, float('inf')):
                g_score[nxt] = tentative_g
                came_from[nxt] = state
                f_score = tentative_g + misplaced_tiles(nxt, goal)
                counter += 1
                heapq.heappush(open_set, (f_score, counter, nxt))
                open_set_tracker.add(nxt)

    return None, nodes_expanded


def run_question2():
    print("=" * 60)
    print("QUESTION 2 : 8-puzzle")
    print("=" * 60)
    print("\nStart state:")
    print_state(START)
    print("Goal state:")
    print_state(GOAL)

    for name, solver in [("(a) BFS", bfs_solve),
                          ("(b) DFS", dfs_solve),
                          ("(c) A*", a_star_puzzle)]:
        print("-" * 60)
        print(name)
        path, nodes_expanded = solver(START, GOAL)
        if path is None:
            print("    No solution found within limits.")
            continue
        print(f"    Solution length (moves): {len(path) - 1}")
        print(f"    Nodes expanded         : {nodes_expanded}")
        print("    Path of states:")
        for step, state in enumerate(path):
            print(f"    Step {step}:")
            print_state(state)


if __name__ == "__main__":
    run_question2()
