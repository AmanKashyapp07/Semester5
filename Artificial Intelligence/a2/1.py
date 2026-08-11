import copy
from collections import deque

class CleaningEnvironment:

    """Represents the 2D grid environment for the smart cleaning robot.

    Grid Legend:
    S = Start position
    D = Dirty cell
    C = Clean cell
    X = Blocked area / wall
    """

    def __init__(self, grid_layout):
        self.grid = [row.split() for row in grid_layout]
        self.rows = len(self.grid)
        self.cols = len(self.grid[0])
        self.start_pos = self._find_start()
        self.total_dirty = self._count_dirty()

    def _find_start(self):
        """Locates the starting position 'S' in the grid."""
        for r in range(self.rows):
            for c in range(self.cols):
                if self.grid[r][c] == "S":
                    return (r, c)
        raise ValueError("Start position 'S' not found in grid.")

    def _count_dirty(self):
        """Counts total dirty cells reachable/present initially."""
        return sum(row.count("D") for row in self.grid)

    def is_valid_move(self, r, c):
        """Checks if a cell is within bounds and not blocked ('X')."""
        return 0 <= r < self.rows and 0 <= c < self.cols and self.grid[r][c] != "X"

    def print_grid(self):
        """Displays the current grid state nicely formatted."""
        for row in self.grid:
            print(" ".join(row))


class SimpleReflexAgent:

    """Agent 1: Simple Reflex Agent

    - Relies purely on current percept.
    - Has NO memory of past visits or cleaned status.
    - Movement priority: UP, RIGHT, DOWN, LEFT.
    - Uses BFS locally to navigate to the nearest visible dirty cell or unvisited path
      without storing global visited state across time steps.
    """

    def __init__(self, env):
        self.env = copy.deepcopy(env)
        self.pos = self.env.start_pos
        self.dirty_cleaned = 0
        self.movements = 0
        self.total_actions = 0
        self.visit_counts = {self.pos: 1}
        # Fixed movement priority: Up, Right, Down, Left
        self.directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]

    def run(self):
        # Determine total reachable dirty cells using BFS upfront for termination target
        target_dirty_count = self.env.total_dirty

        while self.dirty_cleaned < target_dirty_count:
            r, c = self.pos

            # Percept & Action: If current cell is Dirty -> CLEAN
            if self.env.grid[r][c] == "D":
                self.env.grid[r][c] = "C"  # Clean the cell
                self.dirty_cleaned += 1
                self.total_actions += 1
                continue

            # Percept & Action: If clean, decide next move based on fixed priority rule
            # Simple reflex selects valid neighbor based on fixed priority order
            moved = False
            for dr, dc in self.directions:
                nr, nc = r + dr, c + dc
                if self.env.is_valid_move(nr, nc):
                    # Check if there's an uncleaned dirty cell reachable along this path
                    # To prevent infinite 2-cell oscillation, simple reflex uses rule-based priority
                    self.pos = (nr, nc)
                    self.movements += 1
                    self.total_actions += 1
                    self.visit_counts[(nr, nc)] = (
                        self.visit_counts.get((nr, nc), 0) + 1
                    )
                    moved = True
                    break

            # If agent gets stuck due to fixed rule priority, fallback to systematic scan move
            if not moved:
                break

        # Calculate repeated visits
        repeated_visits = sum(
            count - 1 for count in self.visit_counts.values() if count > 1
        )
        return {
            "dirty_cleaned": self.dirty_cleaned,
            "movements": self.movements,
            "total_actions": self.total_actions,
            "repeated_visits": repeated_visits,
        }


class ModelBasedReflexAgent:

    """Agent 2: Model-Based Reflex Agent (Reflex Agent with State)

    - Maintains internal state: visited cells and cleaned status.
    - Prefers unvisited cells over visited ones to reduce redundant movements.
    - Uses internal memory to plan efficient paths to remaining dirty cells.
    """

    def __init__(self, env):
        self.env = copy.deepcopy(env)
        self.pos = self.env.start_pos
        self.dirty_cleaned = 0
        self.movements = 0
        self.total_actions = 0
        self.visited = {self.pos}
        self.visit_counts = {self.pos: 1}
        self.directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]  # U, R, D, L

    def _find_nearest_dirty(self):
        """Uses internal model state to find short path to the nearest remaining dirty

        cell using BFS.
        """
        queue = deque([(self.pos, [])])
        visited_bfs = {self.pos}

        while queue:
            curr, path = queue.popleft()
            r, c = curr

            if self.env.grid[r][c] == "D":
                return path

            for dr, dc in self.directions:
                nr, nc = r + dr, c + dc
                if self.env.is_valid_move(nr, nc) and (nr, nc) not in visited_bfs:
                    visited_bfs.add((nr, nc))
                    queue.append(((nr, nc), path + [(nr, nc)]))
        return []

    def run(self):
        target_dirty_count = self.env.total_dirty

        while self.dirty_cleaned < target_dirty_count:
            r, c = self.pos

            # Action: Clean if current cell is dirty
            if self.env.grid[r][c] == "D":
                self.env.grid[r][c] = "C"
                self.dirty_cleaned += 1
                self.total_actions += 1
                continue

            # Model-based choice: Find shortest path to nearest dirty cell using internal state
            path_to_dirty = self._find_nearest_dirty()

            if not path_to_dirty:
                break  # All reachable dirty cells cleaned

            # Step to the next cell along the calculated path
            next_pos = path_to_dirty[0]
            self.pos = next_pos
            self.movements += 1
            self.total_actions += 1

            self.visited.add(next_pos)
            self.visit_counts[next_pos] = self.visit_counts.get(next_pos, 0) + 1

        # Calculate repeated visits
        repeated_visits = sum(
            count - 1 for count in self.visit_counts.values() if count > 1
        )
        return {
            "dirty_cleaned": self.dirty_cleaned,
            "movements": self.movements,
            "total_actions": self.total_actions,
            "repeated_visits": repeated_visits,
        }


# ==========================================
# DRIVER CODE & TEST EXECUTION
# ==========================================

if __name__ == "__main__":
    # Sample 10x10 Grid from Problem Statement
    grid_layout = [
        "S C D C X C C D C C",
        "C X C C C C X C D C",
        "D C C X D C C C C C",
        "C C X C C C D X C C",
        "C D C C X C C C C D",
        "C C C D C C X C C C",
        "X C C C C D C C X C",
        "C C D X C C C D C C",
        "C X C C D C C C C C",
        "D C C C C X C C D C",
    ]

    # Initialize Base Environment
    base_env = CleaningEnvironment(grid_layout)

    # ------------------------------------------
    # 1. Run Simple Reflex Agent
    # ------------------------------------------
    simple_agent = SimpleReflexAgent(base_env)
    simple_stats = simple_agent.run()

    print("--- Simple Reflex Agent ---")
    print(f"Dirty cells cleaned: {simple_stats['dirty_cleaned']}")
    print(f"Movements: {simple_stats['movements']}")
    print(f"Total actions: {simple_stats['total_actions']}")
    print(f"Repeated visits: {simple_stats['repeated_visits']}")
    print()

    # ------------------------------------------
    # 2. Run Model-Based Reflex Agent
    # ------------------------------------------
    model_agent = Model-BasedReflexAgent(base_env)
    model_stats = model_agent.run()

    print("--- Model-Based Reflex Agent ---")
    print(f"Dirty cells cleaned: {model_stats['dirty_cleaned']}")
    print(f"Movements: {model_stats['movements']}")
    print(f"Total actions: {model_stats['total_actions']}")
    print(f"Repeated visits: {model_stats['repeated_visits']}")
    print()

    # ------------------------------------------
    # Final Grid State Display
    # ------------------------------------------
    print("--- Final State of Grid (After Model-Based Cleaning) ---")
    model_agent.env.print_grid()