from collections import deque


def water_jug_bfs(a, b, c):
    start = (0, 0)
    seen = set()
    q = deque()
    q.append((start, [start]))
    seen.add(start)

    while q:
        (x, y), path = q.popleft()

        if x == c or y == c:
            return path

        next_states = [
            (a, y),
            (x, b),
            (0, y),
            (x, 0),
            (x - min(x, b - y), y + min(x, b - y)),
            (x + min(y, a - x), y - min(y, a - x)),
        ]

        for state in next_states:
            if state not in seen:
                seen.add(state)
                q.append((state, path + [state]))

    return None


def print_solution(path, a, b):
    print(f"Water Jug Problem: {a}-li and {b}-li jugs\n")
    print(f"{'Step':<6}{'4-li jug':<12}{'3-li jug':<12}")
    for i, (x, y) in enumerate(path):
        print(f"{i:<6}{x:<12}{y:<12}")


def main():
    a = 4
    b = 3
    c = 2

    solution = water_jug_bfs(a, b, c)

    if solution:
        print_solution(solution, a, b)
        print(f"\nGoal reached: {c} liters obtained in {len(solution) - 1} steps.")
    else:
        print("No solution exists.")


if __name__ == "__main__":
    main()