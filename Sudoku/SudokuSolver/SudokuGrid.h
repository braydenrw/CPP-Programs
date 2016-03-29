#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <array>
#include <bitset>
#include <string>

class Cell {
public:
    int value;
    std::bitset<9> pencils;
    bool fixed;
    bool solved;
};

class SudokuGrid {
private:
    std::array<std::array<Cell,9>,9> grid;
public:
    // implement the following

    SudokuGrid(std::string s) {
        int k = 0;
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 9; j++) {
                if(s[k] == '.') {
                    grid[i][j].value = 0;
                    grid[i][j].fixed = false;
                    grid[i][j].solved = false;
                } else {
                    grid[i][j].value = (s[k] - '0');
                    grid[i][j].fixed = false;
                    grid[i][j].solved = true;
                }
                clearAllPencils(i, j);
                k++;
            }
        }
    } // constructor

    int number(int row, int col) const {
        return grid[row][col].value;
    }
    void setNumber(int row, int col, int number) {
        grid[row][col].value = number;
    }
    bool isFixed(int row, int col) const {
        return grid[row][col].fixed;
    }
    bool isSolved(int row, int col) const {
        return grid[row][col].solved;
    }
    void setSolved(int row, int col) {
        grid[row][col].solved = true;
    }

    bool isPencilSet(int row, int col, int n) const {
        return grid[row][col].pencils[n-1];
    }
    bool anyPencilsSet(int row, int col) const {
        for(int i = 0; i < 9; i++) {
            if(grid[row][col].pencils[i]) {
                return true;
            }
        }
        return false;
    }
    void setPencil(int row, int col, int n) {
        grid[row][col].pencils[n-1] = true;
    }
    void setAllPencils(int row, int col) {
        for(int i = 0; i < 9; i++) {
            setPencil(row, col, i);
        }
    }
    void clearPencil(int row, int col, int n) {
        grid[row][col].pencils[n-1] = false;
    }
    void clearAllPencils(int row, int col) {
        for(int i = 0; i < 9; i++) {
            grid[row][col].pencils[i] = false;
        }
    }
};

#endif // SUDOKUGRID_H
