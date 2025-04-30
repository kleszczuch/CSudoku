#include "SudokuGame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#ifdef _WIN32
#define CLEAR "cls"
#include <direct.h>
#else
#include <sys/stat.h> 
#define CLEAR "clear"
#endif

#define STR_SIZE 4
char numStr[STR_SIZE];


bool unUsedInBox(int size, char*** grid, int rowStart, int colStart, const char* num) { // check if number is in box
    int box_size = (int)(sqrt(size)); // square size
    for (int i = 0; i < box_size; i++) {
        for (int j = 0; j < box_size; j++) {
            if (strcmp(grid[rowStart + i][colStart + j], num) == 0)
                return false;
        }
    }
    return true;
}

bool unUsedInRow(int size, char*** grid, int row, const char* num) { // check if number is in row   
    for (int j = 0; j < size; j++) {
        if (strcmp(grid[row][j], num) == 0)
            return false;
    }
    return true;
}

bool unUsedInCol(int size, char*** grid, int col, const char* num) { // check if number is in column
    for (int i = 0; i < size; i++) {
        if (strcmp(grid[i][col], num) == 0)
            return false;
    }
    return true;
}

bool checkIfSafe(int size, char*** grid, int row, int col, const char* num) { // check if number is safe to put in grid
    int box_size = (int)(sqrt(size));
    return unUsedInRow(size, grid, row, num) &&
           unUsedInCol(size, grid, col, num) &&
           unUsedInBox(size, grid, row - row % box_size, col - col % box_size, num);
}

char*** createDynamicGrid(int size) { // Create a dynamic 2D array of strings
    char*** grid = (char***)malloc(size * sizeof(char**));// Allocate memory for rows
    if(!grid) return NULL;
    
    for(int i = 0; i < size; i++) {
        grid[i] = (char**)malloc(size * sizeof(char*)); // Allocate memory for columns
        if(!grid[i]) {
            for(int j = 0; j < i; j++) free(grid[j]);
            free(grid);
            return NULL;
        }
        
        for(int j = 0; j < size; j++) {
            grid[i][j] = (char*)malloc(STR_SIZE * sizeof(char)); // Allocate memory for each cell
            if(!grid[i][j]) {
                for(int k = 0; k < j; k++) free(grid[i][k]);
                free(grid[i]);
                for(int k = 0; k < i; k++) {
                    for(int l = 0; l < size; l++) free(grid[k][l]);
                    free(grid[k]);
                }
                free(grid);
                return NULL;
            }
            strcpy(grid[i][j], "-"); // Initialize with a placeholder
        }
    }
    return grid;
}

void freeDynamicGrid(char*** grid, int size) { // Free the dynamic 2D array of strings
    if(!grid) return;
    for(int i = 0; i < size; i++) {
        if(grid[i]) {
            for(int j = 0; j < size; j++) free(grid[i][j]);
            free(grid[i]);
        }
    }
    free(grid);
}

void fillBox(int size, char*** grid, int rowStart, int colStart) { // Fill a box with random numbers 
    int box_size = (int)(sqrt(size));
    char numStr[STR_SIZE];
    for (int i = 0; i < box_size; i++) {
        for (int j = 0; j < box_size; j++) {
            int num;
            do {
                num = (rand() % size) + 1; 
                snprintf(numStr, sizeof(numStr), "%d", num); 
            } while (!unUsedInBox(size, grid, rowStart, colStart, numStr));
            strcpy(grid[rowStart + i][colStart + j], numStr);
        }
    }
}

void fillDiagonal(int size, char*** grid) { // Fill the diagonal boxes
    int box_size = (int)(sqrt(size));
    for (int i = 0; i < size; i += box_size) {
        fillBox(size, grid, i, i);
    }
}

bool fillRemaining(int size, char*** grid, int i, int j) { // Fill the remaining cells in the grid
    if (i == size) return true;
    if (j == size) return fillRemaining(size, grid, i + 1, 0); // Move to next row
    if (strcmp(grid[i][j], "-") != 0) return fillRemaining(size, grid, i, j + 1); // if empty fillRemaining

    int* numbers = malloc(size * sizeof(int));
    for (int v = 0; v < size; v++) numbers[v] = v + 1;

    for (int v = 0; v < size; v++) { // randomize number location
        int r = rand() % size;
        int temp = numbers[v];
        numbers[v] = numbers[r];
        numbers[r] = temp;
    }

    char numStr[3];
    for (int v = 0; v < size; v++) {
        snprintf(numStr, sizeof(numStr), "%d", numbers[v]);
        if (checkIfSafe(size, grid, i, j, numStr)) {
            strcpy(grid[i][j], numStr);
            if (fillRemaining(size, grid, i, j + 1)) {
                free(numbers);
                return true;
            }
            strcpy(grid[i][j], "-");
        }
    }
    free(numbers);
    return false;
}

void removeKDigits(int size, char*** grid, int k) { // Remove k digits from the grid
    int total = size * size;
    int* positions = malloc(total * sizeof(int));
    for (int i = 0; i < total; i++) positions[i] = i;

    for (int i = total - 1; i > 0; i--) { // Create a random permutation of positions
        int j = rand() % (i + 1);
        int temp = positions[i];
        positions[i] = positions[j];
        positions[j] = temp;
    }

    for (int x = 0; x < k; x++) { // Calculate specific positions to remove and remove them
        int cellId = positions[x];
        int row = cellId / size;
        int col = cellId % size;
        strcpy(grid[row][col], "-");
    }

    free(positions);
}


void sudokuGenerator(int size, char*** grid, char*** solution, int k) { // Generate Sudoku grid
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            strcpy(grid[i][j], "-"); // Initialize grid with placeholder

    fillDiagonal(size, grid);
    fillRemaining(size, grid, 0, 0);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            strcpy(solution[i][j], grid[i][j]);

    removeKDigits(size, grid, k);
}

void printGrid(int size, char*** grid, int* moves) { // Print the grid
    int box = (int)sqrt(size);
    printf("    ");
    for (int c = 0; c < size; c++) {    
        printf("%2d ", c + 1); // write top numbers
        if ((c + 1) % box == 0 && c != size - 1)
            printf("|"); // write vertical line for boxes
    }
    printf("\n   ");
    printf("-"); // to move line 1 place right
    for (int c = 0; c < size; c++) { // write top line
        printf("---");
        if ((c + 1) % box == 0 && c != size - 1)
            printf("+");
    }
    printf("-\n");

    for (int r = 0; r < size; r++) {
        printf("%2d |", r + 1);  // write left numbers and line
        for (int c = 0; c < size; c++) {
            printf("%2s ", grid[r][c]); // write grid numbers
            if ((c + 1) % box == 0 && c != size - 1)
                printf("|"); // write vertical line for boxes
        }
        printf("\n");
        if ((r + 1) % box == 0 && r != size - 1) {
            printf("   ");
            printf("-"); // to move line 1 place right
            for (int c = 0; c < size; c++) {
                printf("---");
                if ((c + 1) % box == 0 && c != size - 1)
                    printf("+");
            }
            printf("-\n");
        }
    }
    printf("\nNumber of moves: %d\n\n", *moves); // print number of moves
}

bool isComplete(int size, char*** grid, char*** solution) { // Check if the grid is complete
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (strcmp(grid[i][j], solution[i][j]) != 0)
                return false;
    return true;
}

void userInsert(int size, char*** grid, char*** solution, int* moves) { // User insert number in grid
    int x, y, num;
    char numStr[3];

    printf("Choose X (Column, 1-%d): ", size);
    scanf("%d", &x);
    printf("Choose Y (Row, 1-%d): ", size);
    scanf("%d", &y);
    printf("Choose number (1-%d): ", size);
    scanf("%d", &num);

    system(CLEAR);

    if (x < 1 || x > size || y < 1 || y > size || num < 1 || num > size) {
        printf("Wrong data!\n");
        return;
    }

    snprintf(numStr, sizeof(numStr), "%d", num);

    if (strcmp(grid[y-1][x-1], "-") != 0) {
        printf("This place is taken\n");
        return;
    }

    if (strcmp(solution[y-1][x-1], numStr) == 0) {
        strcpy(grid[y-1][x-1], numStr);
        printf("Very good!!\n");
        (*moves)++;
    } else {
        printf("Boooo, wrong number\n");
        (*moves)++;
    } 
    
}
int main() {
    srand((unsigned)time(NULL));
    char ***grid = NULL, ***solutionGrid = NULL;
    int size, k;
    char choice;
    int moves = 0;
    while(1){   
        system(CLEAR);
        printf("Welcome in C-Sudoku!\n");
        printf("1. New Game\n2. Continue\n3. Exit\nDecision: ");
        scanf(" %c", &choice);
        int moves = 0;
        if (choice == '1'){
            system(CLEAR);

            printf("Choose size:\n1. 4x4\n2. 9x9\n3. 16x16\n");
            scanf(" %c", &choice);
            switch (choice) {
                case '1': size = 4; break;
                case '2': size = 9; break;
                case '3': size = 16; break;
                default: printf("Wrong choice, setting 9x9\n"); size = 9;
            }
            grid = createDynamicGrid(size);
            solutionGrid = createDynamicGrid(size);
            if(!grid || !solutionGrid) {
                printf("memory allocation error!\n");
                if(grid) freeDynamicGrid(grid, size);
                if(solutionGrid) freeDynamicGrid(solutionGrid, size);
                return 1;
            }
            printf("Set difficulty:\n1. Easy\n2. Medium\n3. Hard\n");
            scanf(" %c", &choice);
            switch (choice) {
                case '1': k = (size * size) / 4; break;
                case '2': k = (size * size) / 3; break;
                case '3': k = (size * size) / 2; break;
                default: printf("Wrong choice, default difficulty is set to Medium\n"); k = (size * size) / 3;
            }
            sudokuGenerator(size, grid, solutionGrid, k);
        }
        else if(choice == '2') {
        printf("Loading saved game...\n");
        size = MAX_SIZE;
        grid = createDynamicGrid(size);
        solutionGrid = createDynamicGrid(size);
        if(!grid || !solutionGrid) {
            printf("Memory allocation error!\n");
            if(grid) freeDynamicGrid(grid, size);
            if(solutionGrid) freeDynamicGrid(solutionGrid, size);
            return 1;
        }
        if(!loadGameFromFile(grid, solutionGrid, &size, &moves)) { 
            freeDynamicGrid(grid, size);
            freeDynamicGrid(solutionGrid, size);
        continue;

            }
        } 
        else {
            printf("See you!\n");
            break;
        }
        
        printf("Press enter to continue\n");
        scanf("%c", &choice);
        system(CLEAR);
        while (1) {
            printGrid(size, grid, &moves);
            if (isComplete(size, grid, solutionGrid)) {
                printf("Congratulations! You solved Sudoku!\n");
                printf("Press enter to continue..\n");
                while (getchar() != '\n'); // wait for enter
                    getchar(); 
                break;
            }
            printf("What you want to do?\n1. Insert number\n2. Save game\n3. Exit\nchoose: ");
            int op;
            scanf("%d", &op);
            if (op == 1) {
                userInsert(size, grid, solutionGrid, &moves);
            } else if (op == 2) {
                saveGameToFile(size, grid, solutionGrid, &moves);
            } else {
                break;
            }

        }
        freeDynamicGrid(grid, size);
        freeDynamicGrid(solutionGrid, size);
    }
    return 0;
}