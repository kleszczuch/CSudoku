#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>

#define N 9
#define BLOCK 3

// Declare the SudokuState struct to match the one in solver.c
typedef struct {
    int grid[N][N];
    int cost;
} SudokuState;

// Function declarations
void solve_sudoku_sa(int initial_grid[N][N], double T_start, double T_end, double alpha, int max_iter);
bool solveSudokuSolver(int size, char*** grid);

#endif // SOLVER_H
