#include "main.h"
#include <stdio.h>

void SaveHighScore(const char *filename, int score) {
    FILE *fptr;

    fptr = fopen(filename, "w");

    if (fptr != NULL) {
        fprintf(fptr, "%d", score);
        fclose(fptr);
        printf("Saved High Score");
    }
}

bool LoadHighScore(const char *filename, int *score) {
    FILE *fptr;

    fptr = fopen(filename, "r");

    if (fptr != NULL) {
        fscanf(fptr, "%d", score);
        fclose(fptr);
        printf("Loaded High Score");

        return true;
    }
    return false;
}
