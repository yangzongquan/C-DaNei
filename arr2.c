/*
 * 二维数组
 */
#include <stdio.h>

int main() {
    int num[5][5] = {0};

    int row = 0, col = 0;
    for (int i = 0; i < 5; i++) {
        num[i][i] = 4;
        num[i][4 - i] = 3;
        num[i][2] = 1;
        num[2][i] = 2;
    }
    num[2][2] = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d", num[i][j]); 
        }
        printf("\n");
    }
    return 0;
}
