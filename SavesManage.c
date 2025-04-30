#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>     
#include <dirent.h>
#include <time.h>
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif
#include "SudokuGame.h"




void saveGameToFile(int size, char ***grid, char ***solution, int *moves) { // Save game to file
    #ifdef _WIN32 // if code is used on linux or macOS, _mkdir is for them
        _mkdir("saves");
    #else
        mkdir("saves", 0777);
    #endif  
    time_t t = time(NULL); // to get current time for save name         
    struct tm tm = *localtime(&t); 
    char filename[100];
    snprintf(filename, sizeof(filename), "saves/sudoku_%04d-%02d-%02d_%02d-%02d.txt",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min); //tm.tm_year + 1900 to get year in YYYY format and tm.tm_mon + 1 to get month in MM format

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Save error :c\n");
        return;
    }

    fprintf(file, "%d\n", size); //save size of grid
    fprintf(file, "%d\n", *moves); // save number of moves
    for (int i = 0; i < size; i++) // save gameplay grid
        for (int j = 0; j < size; j++)
            fprintf(file, "%s\n", grid[i][j]);
    for (int i = 0; i < size; i++) // save solution grid
        for (int j = 0; j < size; j++)
            fprintf(file, "%s\n", solution[i][j]);
    fclose(file);
    system(CLEAR);
    printf("Gameplay saved as: %s\n", filename);
}

int listSavedGames(char filenames[][100], int maxFiles) { // generate and show list of saved games
    DIR *d; 
    struct dirent *dir; // dirent.h is used to read directory entries
    d = opendir("saves");
    if (!d) {
        printf("No such a folder as saves.\n");
        return 0;
    }
    int count = 0;
    while ((dir = readdir(d)) != NULL && count < maxFiles) {//readdir reads directory entries one by one
        if (strstr(dir->d_name, ".txt")) { // check if file is .txt
            snprintf(filenames[count], 100, "saves/%s", dir->d_name); //  add path to filenames
            count++;
        }
    }
    closedir(d);
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, filenames[i]); // print file list
    }
    return count;
}

bool loadGameFromFile(char ***grid, char ***solution, int *size, int *moves) { // Load game from file
    char files[50][100]; // array to store filenames, Maximum 50 files
    int count = listSavedGames(files, 50); // get list of saved games
    if (count == 0) {
        printf("No saved Gameplays.\n");
        return false;
    }
    printf("Which Gameplay you want to open:");
    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > count) {
        printf("Wrong choice.\n");
        return false;
    }
    FILE *file = fopen(files[choice - 1], "r"); 
    if (!file) { //error handling
        printf("Can't open file.\n");
        return false;
    }
    fscanf(file, "%d\n", size); // load size of grid
    fscanf(file, "%d\n", moves);  // load attempts
    for (int i = 0; i < *size; i++) // load gameplay grid
        for (int j = 0; j < *size; j++)
            fscanf(file, "%s\n", grid[i][j]);
    for (int i = 0; i < *size; i++) // load solution grid
        for (int j = 0; j < *size; j++)
            fscanf(file, "%s\n", solution[i][j]);
    fclose(file);
    printf("Game loaded succesfuly.\n");
    return true;
}
