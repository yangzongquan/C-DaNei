/*
 * 扫雷
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int addMineNum(int nums[10][10], int row, int col) {
    // 超出矩阵范围
    if (row < 0 || row >= 10 || col < 0 || col >= 10) {
        return 0;
    }
    // 地雷位置，不能计数
    if (nums[row][col] == -1) {
        return 0;
    }

    nums[row][col]++;

    return 1;
}

int main() {
    int num[10][10] = {0};
    srand(time(0));
    for (int i = 0; i < 10; i++) {
        int row = rand() % 10;
        int col = rand() % 10;

        // 如果已经是雷则重新计算
        if (num[row][col] == -1) {
            i--;
            continue;
        }

        // 设置为雷
        num[row][col] = -1;

        // 也可以把8个位置放在数组里面，然后循环处理
        //
        // 同行前后两个位置加1(位置存在，并为非雷)
        addMineNum(num, row, col - 1);
        addMineNum(num, row, col + 1);

        // 上一行三个位置加1(位置存在，并为非雷)
        addMineNum(num, row - 1, col - 1);
        addMineNum(num, row - 1, col);
        addMineNum(num, row - 1, col + 1);
        
        // 下一行三个位置加1(位置存在，并为非雷)
        addMineNum(num, row + 1, col - 1);
        addMineNum(num, row + 1, col);
        addMineNum(num, row + 1, col + 1);
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (num[i][j] == 0)
                printf("%d ", 0);
            
            if (num[i][j] > 0)
                printf("%d ", num[i][j]);

            if (num[i][j] == -1)
                printf("%c ", 'x');
        }
        printf("\n");
    }
    
    return 0;
}
