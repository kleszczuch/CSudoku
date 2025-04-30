#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>  
#define STR_SIZE 4
#define MAX_SIZE 16

bool unUsedInBox(int size, char*** grid, int rowStart, int colStart, const char* num);
bool unUsedInRow(int size, char*** grid, int row, const char* num);
bool unUsedInCol(int size, char*** grid, int col, const char* num);
bool checkIfSafe(int size, char*** grid, int row, int col, const char* num);
void fillBox(int size, char*** grid, int rowStart, int colStart);
void fillDiagonal(int size, char*** grid);
bool fillRemaining(int size, char*** grid, int i, int j);
void removeKDigits(int size, char*** grid, int k);
void sudokuGenerator(int size, char*** grid, char*** solution, int k);
void printGrid(int size, char*** grid, int* moves);
bool isComplete(int size, char*** grid, char*** solution);
void userInsert(int size, char*** grid, char*** solution, int *moves);
void saveGameToFile(int size, char*** grid, char*** solution, int *moves);
bool loadGameFromFile(char*** grid, char*** solution, int *size, int *moves);
char ***createDynamicGrid(int size);
void freeDynamicGrid(char ***grid, int size);

#endif
