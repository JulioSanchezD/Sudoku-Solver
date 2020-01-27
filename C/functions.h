#include <string.h>

#ifndef SUDOKU_FUNCTIONS_H
#define SUDOKU_FUNCTIONS_H

void fillPuzzle(int puzzle[][9], int argc, char *argv[]) {
    int argument, i;
    char *p;
    for(int row=0; row<9; row++) {
        for(int col=0; col<9; col++) {
            i = row*9 + col + 1;
            if (i == argc) return;
            argument = strtol(argv[i], &p, 10);
            puzzle[row][col] = argument;
        }
    }
}

void printSudoku(int puzzle[][9], int plain){
    for (int row=0; row<9; row++) {
        if (!plain && row % 3 == 0 && row > 0) puts("---------------------");
        for (int col = 0; col < 9; col++) {
            if (!plain && col % 3 == 0 && col > 0) printf("| ");
            printf("%d ", puzzle[row][col]);
        }
        if(!plain) puts("");
    }
    if(!plain) puts("");
}

int isValid(int puzzle[][9], int row, int col, int num) {
    int row_inf = (row / 3) * 3;
    int col_inf = (col / 3) * 3;

    for (int i=0; i<9; i++) {
        if (puzzle[row][i] == num) return 0;
        if (puzzle[i][col] == num) return 0;
        if (puzzle[row_inf + (i%3)][col_inf + ((i/3))] == num) return 0;
    }
    return 1;
}

int getObvious(int puzzle[][9], int row, int col) {
    int obvious = 0;
    for (int i=1; i<=9; i++) {
        if (isValid(puzzle, row, col, i)) {
            if (obvious == 0)
                obvious = i;
            else
                return 0;
        }
    }
    if (!obvious) return -1;
    else return obvious;
}

int isSolved(int puzzle[][9]) {
    for (int row=0; row<9; row++) {
        for (int col=0; col<9; col++) {
            if (puzzle[row][col] == 0) return 0;
        }
    }
    return 1;
}

int fillObvious(int puzzle[][9]) {
    int obvious;
    int something_changed;
    do {
        something_changed = 0;
        for (int row=0; row<9; row++) {
            for (int col=0; col<9; col++) {
                if (puzzle[row][col] == 0) {
                    obvious = getObvious(puzzle, row, col);
                    if (obvious == -1) return -1;
                    else if (obvious != 0) {
                        puzzle[row][col] = obvious;
                        // printSudoku(puzzle);
                        something_changed = 1;
                    }
                }
            }
        }
    } while (something_changed);
    if (isSolved(puzzle)) return 1;
    else return  0;
}

void nextMove(int* row, int* col, int forward) {
    if (forward) {
        if (*col == 8) {
            *row += 1;
            *col = 0;
        } else *col += 1;
    }
    else {
        if (*col == 0) {
            *row -= 1;
            *col = 8;
        }
        else *col -= 1;
    }
}

int solve(int puzzle[][9], int row, int col) {
    int backup[9][9];
    int res;
    // Check if its solved, fill obvious and check if it is solved or it is not solvable
    if (isSolved(puzzle)) return 1;
    int status = fillObvious(puzzle);
    if (status != 0) return status;
    // Solve
    if (puzzle[row][col]!=0) {
        nextMove(&row, &col, 1);
        res = solve(puzzle, row, col);
        return res;
    }
    else {
        for (int num=1; num<=9; num++) {
            if (isValid(puzzle, row, col, num)) {
                puzzle[row][col] = num;
                // printSudoku(puzzle);
                nextMove(&row, &col, 1);
                memcpy(&backup, puzzle, sizeof(backup));
                res = solve(puzzle, row, col);
                if (res == 1) return 1;
                else {
                    memcpy(puzzle, backup, sizeof(backup));
                    // printSudoku(puzzle);
                    nextMove(&row, &col, 0);
                }
            }
        }
        nextMove(&row, &col, 0);
        puzzle[row][col] = 0;
        // printSudoku(puzzle);
        return 0;
    }
}

#endif //SUDOKU_FUNCTIONS_H
