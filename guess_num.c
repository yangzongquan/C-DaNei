/*
 * 猜数游戏
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    srand(time(0));
    int r = rand() % 100;
    int num = 0;
    for (;;) {
        printf("请输入猜得的数字：");
        scanf("%d", &num); 
        if (num > r) {
            printf("大了\n"); 
        } else if (num < r) {
            printf("小了\n"); 
        } else {
            printf("恭喜猜对了！数字是%d\n", r);
            break;
        }
    }

    return 0;
}
