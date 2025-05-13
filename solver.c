#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>


int** copyBoard(int** board, int n) {
    int** newBoard = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        newBoard[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            newBoard[i][j] = board[i][j];
    }
    return newBoard;
}
void freeBoard(int** board, int n) {
    for (int i = 0; i < n; i++)
        free(board[i]);
    free(board);
}
int** generateFixedMask(int** board, int n) {
    int** mask = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        mask[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            mask[i][j] = (board[i][j] != 0);
    }
    return mask;
}


void fillSubgrid(int** board, int n, int startRow, int startCol) {
    int subgridSize = (n == 4) ? 2 : (n == 9) ? 3 : 4;
    bool used[n + 1];
    for (int i = 1; i <= n; i++) used[i] = false;

    // Zaznacz już istniejące liczby
    for (int i = startRow; i < startRow + subgridSize; i++)
        for (int j = startCol; j < startCol + subgridSize; j++)
            if (board[i][j] != 0)
                used[board[i][j]] = true;

    // Wstaw brakujące liczby w puste miejsca
    int idx = 1;
    for (int i = startRow; i < startRow + subgridSize; i++) {
        for (int j = startCol; j < startCol + subgridSize; j++) {
            if (board[i][j] == 0) {
                while (idx <= n && used[idx]) idx++;
                if (idx <= n) {
                    board[i][j] = idx;
                    used[idx] = true;
                }
            }
        }
    }
}
int calculateEnergy(int** board, int n) {
    int conflicts = 0;

    // Wiersze
    for (int i = 0; i < n; i++) {
        int* count = calloc(n + 1, sizeof(int));
        for (int j = 0; j < n; j++)
            if (board[i][j] != 0) count[board[i][j]]++;
        for (int k = 1; k <= n; k++)
            if (count[k] > 1) conflicts += (count[k] - 1);
        free(count);
    }

    // Kolumny
    for (int j = 0; j < n; j++) {
        int* count = calloc(n + 1, sizeof(int));
        for (int i = 0; i < n; i++)
            if (board[i][j] != 0) count[board[i][j]]++;
        for (int k = 1; k <= n; k++)
            if (count[k] > 1) conflicts += (count[k] - 1);
        free(count);
    }

    return conflicts;
}


void generateNeighbor(int** board, int** fixedMask, int n) {
    int subgridSize = (n == 4) ? 2 : (n == 9) ? 3 : 4;

    // Losowa podplansza
    int subRow = rand() % subgridSize;
    int subCol = rand() % subgridSize;
    int startRow = subRow * subgridSize;
    int startCol = subCol * subgridSize;

    // Zbierz nieustalone pola
    int candidates[n * 2];
    int count = 0;
    for (int i = startRow; i < startRow + subgridSize; i++) {
        for (int j = startCol; j < startCol + subgridSize; j++) {
            if (fixedMask[i][j] == 0) {
                candidates[count++] = i;
                candidates[count++] = j;
            }
        }
    }

    if (count >= 4) { // Co najmniej dwa pola do zamiany
        int idx1 = (rand() % (count / 2)) * 2;
        int idx2;
        do {
            idx2 = (rand() % (count / 2)) * 2;
        } while (idx1 == idx2);

        int row1 = candidates[idx1], col1 = candidates[idx1 + 1];
        int row2 = candidates[idx2], col2 = candidates[idx2 + 1];

        // Zamiana wartości
        int temp = board[row1][col1];
        board[row1][col1] = board[row2][col2];
        board[row2][col2] = temp;
    }
}
int** solveSimulatedAnnealing(int** board, int n, int initialTemp, double coolingRate, int maxIterations) {
    int** currentBoard = copyBoard(board, n);
    int** fixedMask = generateFixedMask(board, n);

    // Inicjalizacja planszy poprawnymi podplanszami
    int subgridSize = (n == 4) ? 2 : (n == 9) ? 3 : 4;
    for (int i = 0; i < n; i += subgridSize) {
        for (int j = 0; j < n; j += subgridSize) {
            fillSubgrid(currentBoard, n, i, j);
        }
    }

    int currentEnergy = calculateEnergy(currentBoard, n);
    double temp = initialTemp;

    for (int iter = 0; iter < maxIterations && currentEnergy > 0; iter++) {
        int** neighborBoard = copyBoard(currentBoard, n);
        generateNeighbor(neighborBoard, fixedMask, n);
        int neighborEnergy = calculateEnergy(neighborBoard, n);

        int delta = neighborEnergy - currentEnergy;
        if (delta < 0 || (rand() / (double)RAND_MAX) < exp(-delta / temp)) {
            freeBoard(currentBoard, n);
            currentBoard = neighborBoard;
            currentEnergy = neighborEnergy;
        } else {
            freeBoard(neighborBoard, n);
        }

        temp *= coolingRate;
        
        printf("Iter: %d | Energy: %d | Temp: %.2f\n", iter, currentEnergy, temp);
        
    }

    freeBoard(fixedMask, n);
    return currentBoard;
}


