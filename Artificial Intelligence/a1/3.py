from collections import deque


class ParkingSystem:
    def __init__(self, grid):
        self.grid = [list(row) for row in grid]
        self.rows = len(self.grid)
        self.cols = len(self.grid[0])
        self.entrance = self._find_entrance()

    def _find_entrance(self):
        for i in range(self.rows):
            for j in range(self.cols):
                if self.grid[i][j] == 'E':
                    return (i, j)
        raise ValueError("No entrance 'E' found in the grid.")

    def _is_valid(self, r, c):
        return 0 <= r < self.rows and 0 <= c < self.cols

    def find_nearest_parking(self):
        start = self.entrance
        seen = set()
        seen.add(start)

        q = deque()
        q.append(start)

        parent = {start: None}

        moves = [(-1, 0), (1, 0), (0, -1), (0, 1)]

        while q:
            current = q.popleft()
            r, c = current

            if self.grid[r][c] == 'A':
                path = self._reconstruct_path(parent, current)
                return current, len(path) - 1, path

            for dr, dc in moves:
                nr, nc = r + dr, c + dc
                if not self._is_valid(nr, nc):
                    continue
                if (nr, nc) in seen:
                    continue

                cell = self.grid[nr][nc]
                if cell in ('X', 'O'):
                    continue
                if cell not in ('R', 'A', 'E'):
                    continue

                seen.add((nr, nc))
                parent[(nr, nc)] = current
                q.append((nr, nc))

        return None, -1, []

    def _reconstruct_path(self, parent, end):
        path = []
        node = end
        while node is not None:
            path.append(node)
            node = parent[node]
        path.reverse()
        return path

    def display_grid(self):
        print("Parking Grid:")
        for row in self.grid:
            print(" ".join(row))
        print()

    def display_result(self, position, distance, path):
        if position is None:
            print("No reachable available parking space found.")
            return

        print(f"Nearest available parking space: {position}")
        print(f"Number of movements required: {distance}")
        route_str = " -> ".join(str(p) for p in path)
        print(f"Route: {route_str}")


def build_grid_from_rows(rows):
    max_len = max(len(r) for r in rows)
    grid = []
    for r in rows:
        row = list(r) + ['X'] * (max_len - len(r))
        grid.append(row)
    return grid


def get_grid_from_user():
    print("Enter the number of rows and columns of the parking grid (min 10x10 recommended):")
    rows = int(input("Rows: "))
    cols = int(input("Columns: "))

    print("\nEnter each row as space-separated symbols from {E, A, O, R, X}")
    print("(exactly one 'E' must be present)\n")

    grid_rows = []
    for i in range(rows):
        while True:
            row = input(f"Row {i + 1}: ").split()
            if len(row) == cols and all(c in ('E', 'A', 'O', 'R', 'X') for c in row):
                grid_rows.append(row)
                break
            print(f"Invalid row. Enter exactly {cols} values from {{E, A, O, R, X}}.")
    return grid_rows


def demo_grid_10x10():
    grid = [
        "E R R X A X X R R R",
        "R X R R R X R X R R",
        "A R R X O R R O R A",
        "R R X R R R X R R R",
        "X R R R X R R R X R",
        "R R O R R X R R R R",
        "R X R R R R X R O R",
        "A R R X R R R R R A",
        "R R R R O R R X R R",
        "X R R R R R R R A R",
    ]
    return [row.split() for row in grid]


def main():
    print("Smart Parking System (BFS)")
    print("1. Use demo 10x10 grid")
    print("2. Enter custom grid")
    choice = input("Choose an option (1/2): ").strip()

    if choice == '1':
        grid = demo_grid_10x10()
    else:
        grid = get_grid_from_user()

    parking = ParkingSystem(grid)
    parking.display_grid()

    position, distance, path = parking.find_nearest_parking()
    parking.display_result(position, distance, path)


if __name__ == "__main__":
    main()