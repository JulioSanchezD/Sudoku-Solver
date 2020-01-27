from itertools import chain
import time
import copy


class Timer(object):
    def __init__(self, name=None):
        self.name = name

    def __enter__(self):
        self.tstart = time.time()

    def __exit__(self, type, value, traceback):
        if self.name:
            print('[%s]' % self.name, )
        print('Elapsed: %s seconds' % (time.time() - self.tstart))


class Sudoku:

    def __init__(self, grid):
        self.grid = grid

    def __str__(self):
        for row in range(9):
            if row % 3 == 0 and row > 0:
                print("---------------------")
            for col in range(9):
                if col % 3 == 0 and col > 0:
                    print("| ", end="")
                print(str(self.grid[row][col]) + " ", end="")
            print()
        return ""

    def getSquare(self, r, c):
        row1 = (r // 3) * 3
        row2 = row1 + 3
        col1 = (c // 3) * 3
        col2 = col1 + 3
        return [row[col1:col2] for row in self.grid[row1:row2]]

    def getRow(self, i):
        return tuple(self.grid[i])

    def getColumn(self, i):
        return list(zip(*self.grid))[i]

    def getValidNumbers(self, r, c):
        res = []
        column = self.getColumn(c)
        row = self.getRow(r)
        grid = self.getSquare(r, c)
        for j in range(1, 10):
            if not (j in column or j in row or self.findInGrid(grid, j)[0]):
                res.append(j)
        return res

    @staticmethod
    def findInGrid(grid, target):
        for i, row in enumerate(grid):
            if target in row:
                return (True, i, row.index(target))
        return [False]

    def fillObvious(self):
        while True:
            somethingChanged = False
            for r in range(0, 9):
                for c in range(0, 9):
                    if self.grid[r][c] == 0:
                        possibilities = self.getValidNumbers(r, c)
                        n = len(possibilities)
                        if n == 1:
                            self.grid[r][c] = possibilities[0]
                            somethingChanged = True
                        elif n == 0:
                            return "Invalid"
            if not somethingChanged:
                break
        if self.isSolved():
            return "Solved"
        else:
            return "Ok"

    def isSolved(self):
        if self.findInGrid(self.grid, 0)[0]:
            return False
        else:
            return True

    @staticmethod
    def nextMove(r, c):
        if c == 8:
            return (r + 1, 0)
        else:
            return (r, c + 1)

    def solve(self, r=0, c=0):
        if self.isSolved():
            return True
        status = self.fillObvious()
        if status == "Solved":
            return True
        elif status == "Invalid":
            return False

        if self.grid[r][c] == 0:
            possibilities = self.getValidNumbers(r, c)
            r2, c2 = self.nextMove(r, c)
            backup = copy.deepcopy(self.grid)
            for number in possibilities:
                self.grid[r][c] = number
                res = self.solve(r=r2, c=c2)
                if res:
                    return True
                else:
                    self.grid = copy.deepcopy(backup)
            return False
        else:
            r2, c2 = self.nextMove(r, c)
            res = self.solve(r=r2, c=c2)
            if res:
                return True
            else:
                return False


if __name__ == "__main__":
    grid = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
            [5, 0, 3, 0, 6, 7, 0, 0, 0],
            [9, 0, 0, 3, 4, 2, 1, 0, 0],
            [0, 0, 0, 0, 0, 4, 0, 0, 0],
            [0, 0, 1, 0, 0, 0, 7, 2, 0],
            [0, 0, 2, 0, 1, 0, 0, 0, 0],
            [0, 3, 0, 0, 0, 0, 0, 0, 9],
            [0, 8, 0, 1, 0, 0, 2, 0, 0],
            [0, 0, 0, 7, 5, 0, 8, 0, 6]]
    sudoku = Sudoku(grid)
    sudoku.fillObvious()
    print(str(sudoku))
