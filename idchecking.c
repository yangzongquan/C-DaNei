/*
 * 计算身份证后两位
 * */
#include <stdio.h>

int main() {
    int arr17[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    char arr11[] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};

    int num[17];
ReInput:
    printf("请依次输入前17位身份证号，空格空开:");
    for (int i = 0; i < 17; i++) {
        num[i] = -1;
        scanf("%d", &num[i]);
        if (num[i] < 0 || num[i] > 9) {
            printf("输入错误！\n");
            goto ReInput;
        }
    }

    int sum = 0;
    for (int i = 0; i < 17; i++) {
        sum += arr17[i] * num[i];
    }

    printf("身份中最后编号：%c\n", arr11[sum % 11]);

    return 0;
}
