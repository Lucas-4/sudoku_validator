#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define SUDOKU_SIZE 9

typedef struct sudoku {
    int matrix[SUDOKU_SIZE][SUDOKU_SIZE];
}sudoku;

typedef struct params {
    sudoku *s;
    bool valid[3];
}params;

void resetArray(int arr[]) {
    for (int i = 0;i < SUDOKU_SIZE;i++) {
        arr[i] = 0;
    }
}

bool isValid(int arr[]) {
    for (int i = 0;i < SUDOKU_SIZE;i++) {
        if (arr[i] != 1) {
            return false;
        }
    }
    return true;
}

void *validateColumns(void *param) {
    params *p = (params *)param;
    p->valid[0] = true;
    int arr[SUDOKU_SIZE];
    resetArray(arr);
    for (int i = 0; i < SUDOKU_SIZE;i++) {
        for (int j = 0;j < SUDOKU_SIZE;j++) {
            arr[p->s->matrix[j][i] - 1] += 1;
        }
        if (!isValid(arr)) {
            p->valid[0] = false;
            break;
        }
        resetArray(arr);
    }
}

void *validateRows(void *param) {
    params *p = (params *)param;
    p->valid[1] = true;
    int arr[9];
    resetArray(arr);
    for (int i = 0; i < SUDOKU_SIZE;i++) {
        for (int j = 0;j < SUDOKU_SIZE;j++) {
            arr[p->s->matrix[i][j] - 1] += 1;
        }
        if (!isValid(arr)) {
            p->valid[1] = false;
            break;
        }
        resetArray(arr);
    }
}

bool isSquareValid(int square, sudoku *s) {
    int arr[9];
    resetArray(arr);
    for (int i = (square / 3) * 3;i < ((square / 3) * 3) + 3;i++) {
        for (int j = (square % 3) * 3;j < ((square % 3) * 3) + 3;j++) {
            arr[s->matrix[i][j] - 1] += 1;
        }
    }
    return isValid(arr);

}

void *validateSquares(void *param) {
    params *p = (params *)param;
    p->valid[2] = true;

    for (int i = 0;i < SUDOKU_SIZE;i++) {
        if (!isSquareValid(i, p->s)) {
            p->valid[2] = false;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    int validSudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };
    params *p = (params *)malloc(sizeof(params));
    sudoku *s = (sudoku *)malloc(sizeof(sudoku));
    p->s = s;
    for (int i = 0; i < SUDOKU_SIZE;i++) {
        for (int j = 0;j < SUDOKU_SIZE;j++) {
            s->matrix[i][j] = validSudoku[i][j];
        }
    }

    pthread_t tid1;

    pthread_create(&tid1, NULL, validateRows, p);
    pthread_create(&tid1, NULL, validateColumns, p);
    pthread_create(&tid1, NULL, validateSquares, p);

    pthread_join(tid1, NULL);

    if (p->valid[0] == true && p->valid[1] == true && p->valid[2] == true) {
        printf("valid");
    }
    else {
        printf("invalid");
    }
    return 0;
}