/*
 * 费式数列递归实现
 */
#include <stdio.h>


int sum(int num) {
    if (num == 1) {
        return 1;
    }
    return num + sum(num - 1);
}

int sumLast(int index) {
    if (index == 0 || index == 1) return 1;

    // 随着递归深度增加，大量重复计算，缓存结果可极大优化性能
    //              10
    //      9               8
    //  8       7       7       6
    // 7 6     6 5     6 5     5 4
    static int num[100];
    if (!num[index - 1]) {
        num[index -1] = sumLast(index -1);
    }
    if (!num[index - 2]) {
        num[index - 2] = sumLast(index - 2);
    }

    return num[index -1] + num[index -2];
}

int main() {
    int num = 0;
    printf("请输入一个数字：");
    scanf("%d", &num);

    //printf("结果：%d\n", sum(num));
    printf("sumLast：%d\n", sumLast(num));

    return 0;
}
