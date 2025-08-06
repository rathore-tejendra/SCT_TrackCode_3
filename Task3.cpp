#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class SudokuSolver {
private:
    static const int SIZE = 9;
    static const int BOX_SIZE = 3;
    vector<vector<int>> grid;
    vector<vector<bool>> originalNumbers;

public:
    SudokuSolver() : grid(SIZE, vector<int>(SIZE, 0)),
                     originalNumbers(SIZE, vector<bool>(SIZE, false)) {}

    bool isValid(int row, int col, int num) const {
        for (int x = 0; x < SIZE; x++) {
            if (grid[row][x] == num) return false;
        }
        for (int x = 0; x < SIZE; x++) {
            if (grid[x][col] == num) return false;
        }

        int startRow = row - row % BOX_SIZE;
        int startCol = col - col % BOX_SIZE;

        for (int i = 0; i < BOX_SIZE; i++) {
            for (int j = 0; j < BOX_SIZE; j++) {
                if (grid[i + startRow][j + startCol] == num) return false;
            }
        }
        return true;
    }

    bool solve() {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (grid[row][col] == 0) {
                    for (int num = 1; num <= 9; num++) {
                        if (isValid(row, col, num)) {
                            grid[row][col] = num;
                            if (solve()) return true;
                            grid[row][col] = 0; // backtrack
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    bool isValidPuzzle() const {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (grid[row][col] != 0) {
                    int temp = grid[row][col];
                    const_cast<SudokuSolver*>(this)->grid[row][col] = 0;
                    bool valid = isValid(row, col, temp);
                    const_cast<SudokuSolver*>(this)->grid[row][col] = temp;
                    if (!valid) return false;
                }
            }
        }
        return true;
    }

    void loadPuzzleFromInput() {
        cout << "Enter the Sudoku puzzle (9x9 grid):\n";
        cout << "Use 0 for empty cells, numbers 1-9 for filled cells:\n";
        cout << "Enter row by row, separated by spaces:\n\n";

        for (int i = 0; i < SIZE; i++) {
            cout << "Row " << (i + 1) << ": ";
            for (int j = 0; j < SIZE; j++) {
                int value;
                while (!(cin >> value) || value < 0 || value > 9) {
                    cout << "Invalid input! Please enter numbers 0-9: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
                grid[i][j] = value;
                originalNumbers[i][j] = (value != 0);
            }
        }
    }

    void loadExamplePuzzle() {
        vector<vector<int>> example = {
            {5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        };

        grid = example;;;

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                originalNumbers[i][j] = (grid[i][j] != 0);
            }
        }
    }

    bool loadPuzzleFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return false;
        }

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (!(file >> grid[i][j]) || grid[i][j] < 0 || grid[i][j] > 9) {
                    cout << "Error: Invalid data in file" << endl;
                    file.close();
                    return false;
                }
                originalNumbers[i][j] = (grid[i][j] != 0);
            }
        }

        file.close();
        return true;
    }

    void displayGrid() const {
        cout << "\n";
        for (int i = 0; i < SIZE; i++) {
            if (i % 3 == 0 && i != 0) {
                cout << "------+-------+------\n";
            }

            for (int j = 0; j < SIZE; j++) {
                if (j % 3 == 0 && j != 0) {
                    cout << "| ";
                }

                if (grid[i][j] == 0) {
                    cout << ". ";
                } else {
                    if (originalNumbers[i][j]) {
                        cout << grid[i][j] << " ";
                    } else {
                        cout << "\033[32m" << grid[i][j] << "\033[0m ";
                    }
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }

    void saveSolutionToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                file << grid[i][j];
                if (j < SIZE - 1) file << " ";
            }
            file << "\n";
        }

        file.close();
        cout << "Solution saved to " << filename << endl;
    }

    void displayStatistics() const {
        int filledCells = 0;
        int emptyCells = 0;

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (originalNumbers[i][j]) {
                    filledCells++;
                } else if (grid[i][j] != 0) {
                    emptyCells++;
                }
            }
        }

        cout << "Puzzle Statistics:\n";
        cout << "Original filled cells: " << filledCells << "/81\n";
        cout << "Solved cells: " << emptyCells << "/81\n";
        cout << "Total filled: " << (filledCells + emptyCells) << "/81\n\n";
    }
};

void displayMenu() {
    cout << "\n=== SUDOKU SOLVER ===" << endl;
    cout << "1. Enter puzzle manually" << endl;
    cout << "2. Load example puzzle" << endl;
    cout << "3. Load puzzle from file" << endl;
    cout << "4. Exit" << endl;
    cout << "Choose an option (1-4): ";
}

int main() {
    SudokuSolver solver;
    int choice;

    while (true) {
        displayMenu();

        if (!(cin >> choice) || choice < 1 || choice > 4) {
            cout << "Invalid choice! Please enter 1-4." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 4) {
            cout << "Thank you for using Sudoku Solver!" << endl;
            break;
        }

        bool puzzleLoaded = false;

        switch (choice) {
            case 1:
                solver.loadPuzzleFromInput();
                puzzleLoaded = true;
                break;

            case 2:
                solver.loadExamplePuzzle();
                cout << "Example puzzle loaded!" << endl;
                puzzleLoaded = true;
                break;

            case 3: {
                string filename;
                cout << "Enter filename: ";
                cin >> filename;
                puzzleLoaded = solver.loadPuzzleFromFile(filename);
                break;
            }
        }

        if (!puzzleLoaded) {
            continue;
        }

        cout << "\nOriginal Puzzle:" << endl;
        solver.displayGrid();

        if (!solver.isValidPuzzle()) {
            cout << "Error: The puzzle contains conflicts and cannot be solved!" << endl;
            continue;
        }

        cout << "Solving puzzle..." << endl;

        if (solver.solve()) {
            cout << "✓ Puzzle solved successfully!" << endl;
            cout << "\nSolution (solved numbers in green):" << endl;
            solver.displayGrid();
            solver.displayStatistics();

            char saveChoice;
            cout << "Save solution to file? (y/n): ";
            cin >> saveChoice;
            if (saveChoice == 'y' || saveChoice == 'Y') {
                string filename;
                cout << "Enter filename: ";
                cin >> filename;
                solver.saveSolutionToFile(filename);
            }
        } else {
            cout << "✗ No solution exists for this puzzle!" << endl;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}
