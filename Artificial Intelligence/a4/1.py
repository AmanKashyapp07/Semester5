import heapq

graph = {
    'a': {'b': 5, 'c': 2, 'd': 3},
    'b': {'a': 5, 'c': 2, 'f': 3},
    'c': {'a': 2, 'b': 2, 'd': 1, 'e': 2, 'f': 6},
    'd': {'a': 3, 'c': 1, 'e': 4},
    'e': {'c': 2, 'd': 4, 'f': 4},
    'f': {'b': 3, 'c': 6, 'e': 4},
}

heuristic = {'a': 6, 'b': 2, 'c': 5, 'd': 6, 'e': 4, 'f': 0}


def dijkstra(graph, start, goal):
    dist = {node: float('inf') for node in graph}
    dist[start] = 0
    prev = {node: None for node in graph}
    visited = set()

    pq = [(0, start)]
    while pq:
        d, node = heapq.heappop(pq)
        if node in visited:
            continue
        visited.add(node)

        if node == goal:
            break

        for neighbor, weight in graph[node].items():
            new_dist = d + weight
            if new_dist < dist[neighbor]:
                dist[neighbor] = new_dist
                prev[neighbor] = node
                heapq.heappush(pq, (new_dist, neighbor))

    path = []
    node = goal
    while node is not None:
        path.append(node)
        node = prev[node]
    path.reverse()

    return path, dist[goal]


def a_star(graph, start, goal, heuristic):
    g_score = {node: float('inf') for node in graph}
    g_score[start] = 0
    prev = {node: None for node in graph}

    open_set = [(heuristic[start], start)]
    closed = set()

    while open_set:
        f, node = heapq.heappop(open_set)

        if node == goal:
            break
        if node in closed:
            continue
        closed.add(node)

        for neighbor, weight in graph[node].items():
            tentative_g = g_score[node] + weight
            if tentative_g < g_score[neighbor]:
                g_score[neighbor] = tentative_g
                prev[neighbor] = node
                f_score = tentative_g + heuristic[neighbor]
                heapq.heappush(open_set, (f_score, neighbor))

    path = []
    node = goal
    while node is not None:
        path.append(node)
        node = prev[node]
    path.reverse()

    return path, g_score[goal]


def run_question1():
    print("=" * 60)
    print("QUESTION 1 : Shortest path from 'a' to 'f'")
    print("=" * 60)

    path, cost = dijkstra(graph, 'a', 'f')
    print("\n(a) Dijkstra's algorithm")
    print(f"    Path : {' -> '.join(path)}")
    print(f"    Cost : {cost}")

    path, cost = a_star(graph, 'a', 'f', heuristic)
    print("\n(b) A* algorithm")
    print(f"    Path : {' -> '.join(path)}")
    print(f"    Cost : {cost}")


if __name__ == "__main__":
    run_question1()