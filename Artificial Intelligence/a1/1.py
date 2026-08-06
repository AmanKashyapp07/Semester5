def print_board(board):
    for row in board:
        print(" | ".join(row))
        print("-" * 9)


def check_winner(board):
    lines = []

    for i in range(3):
        lines.append([board[i][0], board[i][1], board[i][2]])

    for j in range(3):
        lines.append([board[0][j], board[1][j], board[2][j]])

    lines.append([board[0][0], board[1][1], board[2][2]])
    lines.append([board[0][2], board[1][1], board[2][0]])

    for line in lines:
        if line[0] != '_' and line[0] == line[1] == line[2]:
            return line[0]

    for row in board:
        if '_' in row:
            return None

    return 'Draw'


def input_board():
    print("Enter the 3x3 board row by row.")
    print("Use 'X', 'O', or '_' (underscore) for empty cells, separated by spaces.")
    board = []
    for i in range(3):
        while True:
            row = input(f"Row {i + 1}: ").split()
            if len(row) == 3 and all(c in ('X', 'O', '_') for c in row):
                board.append(row)
                break
            print("Invalid row. Enter exactly 3 values from {X, O, _}.")
    return board


def main():
    board = input_board()
    print("\nBoard:")
    print_board(board)

    result = check_winner(board)

    if result == 'Draw':
        print("Result: The game is a Draw.")
    elif result is None:
        print("Result: No winner yet, game still in progress.")
    else:
        print(f"Result: Player '{result}' wins!")


if __name__ == "__main__":
    main()