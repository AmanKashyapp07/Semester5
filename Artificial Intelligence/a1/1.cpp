#include <iostream>
#include <vector>
#include <string>
using namespace std;

void printBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < 3; i++) {
        cout << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << "\n";
        cout << "---------\n";
    }
} // just printing all values in the 2D vector board

string checkWinner(const vector<vector<char>>& board) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != '_' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return string(1, board[i][0]);
        if (board[0][i] != '_' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return string(1, board[0][i]);
    }
    // Check diagonals
    if (board[0][0] != '_' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return string(1, board[0][0]);
    if (board[0][2] != '_' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return string(1, board[0][2]);

    // Check for empty cells (game still running)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') return "None";
        }
    }

    return "Draw";
} // checking if there is a winner or if the game is still running or if it is a draw

int main() {
    cout << "Enter the 3x3 board row by row.\n";
    cout << "Use 'X', 'O', or '_' for empty cells, separated by spaces:\n";

    vector<vector<char>> board(3, vector<char>(3));
    for (int i = 0; i < 3; i++) {
        cout << "Row " << (i + 1) << ": ";
        for (int j = 0; j < 3; j++) {
            cin >> board[i][j];
        }
    }

    cout << "\nBoard:\n";
    printBoard(board);

    string result = checkWinner(board);
    if (result == "Draw") {
        cout << "Result: The game is a Draw.\n";
    } else if (result == "None") {
        cout << "Result: No winner yet, game still in progress.\n";
    } else {
        cout << "Result: Player '" << result << "' wins!\n";
    }

    return 0;
}
