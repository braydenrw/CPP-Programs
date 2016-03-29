#include <string>
#include <iostream>
#include <algorithm>
#include "SudokuGrid.h"

using namespace std;

/*  Finds if there is a conflicting number in either the row column
    or block if it finds one, return true else false */
bool conflictingNumber(SudokuGrid &grid, int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (grid.number(i, col) == num) return true;
    }

    for (int i = 0; i < 9; i++) {
        if (grid.number(row, i) == num) return true;
    }

    if (0 <= row && row <= 2) {
        /* .##
           ###
           ### */
        if (0 <= col && col <= 2) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
            /* #.#
               ###
               ### */
        } else if (3 <= col && col <= 5) {
            for (int i = 0; i < 3; i++) {
                for (int j = 3; j < 6; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
            /* ##.
               ###
               ### */
        } else {
            for (int i = 0; i < 3; i++) {
                for (int j = 6; j < 9; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
        }
        /* ###
           .##
           ### */
    } else if (3 <= row && row <= 5) {
        if (0 <= col && col <= 2) {
            for (int i = 3; i < 6; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
            /* ###
               #.#
               ### */
        } else if (3 <= col && col <= 5) {
            for (int i = 3; i < 6; i++) {
                for (int j = 3; j < 6; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
            /* ###
               ##.
               ### */
        } else {
            for (int i = 3; i < 6; i++) {
                for (int j = 6; j < 9; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
        }
        /* ###
           ###
           .## */
    } else {
        if (0 <= col && col <= 2) {
            for (int i = 6; i < 9; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
            /* ###
               ###
               #.# */
        } else if (3 <= col && col <= 5) {
            for (int i = 6; i < 9; i++) {
                for (int j = 3; j < 6; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
            /* ###
               ###
               ##. */
        } else {
            for (int i = 6; i < 9; i++) {
                for (int j = 6; j < 9; j++) {
                    if (grid.number(i, j) == num) return true;
                }
            }
        }
    }
    return false;
}

/*  Pencils in all possibilities for each cell in the grid 
    by set all 9 pencils and then removing conflicting pencils */
void autoPencil(SudokuGrid &grid) {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (grid.number(r, c) == 0) {
                grid.setAllPencils(r, c);
                for (int n = 1; n <= 9; n++) {
                    if (conflictingNumber(grid, r, c, n)) {
                        grid.clearPencil(r, c, n);
                    }
                }
            }
        }
    }
}

/*  Counts the number of times the pencil n appears in the given row */
int numPencilsInRow(SudokuGrid &grid, int row, int n) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (grid.isPencilSet(row, i, n)) count++;
    }
    return count;
}
/*  Counts the number of times pencil n appears in the given column */
int numPencilsInColumn(SudokuGrid &grid, int col, int n) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (grid.isPencilSet(i, col, n)) count++;
    }
    return count;
}
/*  Counts the number of times pencil n appears in the given block */
int numPencilsInBlock(SudokuGrid &grid, int row, int col, int n) {
    int count = 0;
    if (0 <= row && row <= 2) {
        /* .##
           ###
           ### */
        if (0 <= col && col <= 2) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
            /* #.#
               ###
               ### */
        } else if (3 <= col && col <= 5) {
            for (int i = 0; i < 3; i++) {
                for (int j = 3; j < 6; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
            /* ##.
               ###
               ### */
        } else {
            for (int i = 0; i < 3; i++) {
                for (int j = 6; j < 9; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
        }
        /* ###
           .##
           ### */
    } else if (3 <= row && row <= 5) {
        if (0 <= col && col <= 2) {
            for (int i = 3; i < 6; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
            /* ###
               #.#
               ### */
        } else if (3 <= col && col <= 5) {
            for (int i = 3; i < 6; i++) {
                for (int j = 3; j < 6; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
            /* ###
               ##.
               ### */
        } else {
            for (int i = 3; i < 6; i++) {
                for (int j = 6; j < 9; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
        }
        /* ###
           ###
           .## */
    } else {
        if (0 <= col && col <= 2) {
            for (int i = 6; i < 9; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
            /* ###
               ###
               #.# */
        } else if (3 <= col && col <= 5) {
            for (int i = 6; i < 9; i++) {
                for (int j = 3; j < 6; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
            /* ###
               ###
               ##. */
        } else {
            for (int i = 6; i < 9; i++) {
                for (int j = 6; j < 9; j++) {
                    if (grid.isPencilSet(i, j, n)) count++;
                }
            }
        }
    }
    return count;
}

/*  Deductively solves a few places on the grid to lighten the load 
    for solve, it does this by finding pencils that have no conflicting
    pencil marks */
void deduce(SudokuGrid &grid) {
    bool changed;
    do { // repeat until no changes made
        autoPencil(grid);
        changed = false;
        for (int row = 0; row < 9; row++)
            for (int col = 0; col < 9; col++)
                for (int n = 1; n <= 9; n++)
                    if (grid.isPencilSet(row, col, n) &&
                        (numPencilsInRow(grid, row, n) == 1 ||
                         numPencilsInColumn(grid, col, n) == 1 ||
                         numPencilsInBlock(grid, row, col, n) == 1)) {
                        grid.clearAllPencils(row, col);
                        grid.setNumber(row, col, n);
                        grid.setSolved(row, col);
                        autoPencil(grid);
                        changed = true;
                        break;
                    }
    } while (changed);
}

/*  Finds cells that do not have a proper value and gives the saves the row
    col */
bool findUnassignedLocation(SudokuGrid &grid, int &row, int &col) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid.number(i, j) == 0) {
                row = i;
                col = j;
                return true;
            }
        }
    }
    return false;
}

/*  Recursively solves the rest of the sudoku grid through a back tracking
    algorithm */
bool solveSudoku(SudokuGrid &grid) {
    int row, col;
    if (!findUnassignedLocation(grid, row, col))
        return true; // puzzle filled, solution found!
    for (int num = 1; num <= 9; num++) {
        if (!conflictingNumber(grid, row, col, num)) {
            grid.setNumber(row, col, num); // try next number
            if (solveSudoku(grid))
                return true;                 // solved!
            grid.setNumber(row, col, 0);   // not solved, clear number
        }
    }
    return false; // not solved, back track
}

/*  Prints out the grid */
void printGrid(SudokuGrid &grid) {
    int k = 0;
    for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 9; j++) {
            if(k == 0) std::cout << "\n";
            else if(k % 27 == 0) std::cout << "\n------+-------+------\n";
            else if(k % 9 == 0) std::cout << "\n";
            else if(k % 3 == 0) std::cout << "| ";
            if(grid.number(i,j) == 0) std::cout << ". ";
            else std::cout << ((char)(grid.number(i, j) + '0')) << " ";
            k++;
        }
    }
    std::cout << "\n";
}

int main(int argc, char *argv[]) {
    std::string puzzle;
    std::cin >> puzzle;
    if (puzzle.length() != 9 * 9 || !all_of(puzzle.begin(), puzzle.end(), [](char ch) {
        return ch == '.' || ('1' <= ch && ch <= '9');
    })) {
        cerr << "bogus puzzle!" << endl;
        exit(1);
    }
    SudokuGrid grid(puzzle);

    printGrid(grid);
    deduce(grid);
    printGrid(grid);
    solveSudoku(grid);
    printGrid(grid);

    return 0;
}
