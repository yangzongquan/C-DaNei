/*
 * 插入排序。
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void swap(int *num1, int *num2) {
    int tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}

void sort(int nums[], int len) {
    // 从下标1到len-1，依次挑出数
    for (int i = 1; i < len; i++) {
        // 将i位置的数依次倒序与前面的数比较和交换，直到合适位置
        for (int j = i - 1; j >= 0; j--) {
            if (nums[j] > nums[j + 1]) {
                swap(nums + j, nums + j + 1);
            } else {
                break;
            }
        }
    }
}

void print(int nums[], int len) {
    printf("[");
    for (int i = 0; i < len; i++) {
        printf(i == 0 ? "%d" : ", %d", *(nums + i));
    }
    printf("]\n");
}

void randNums(int *nums, int len, int maxNum) {
    for (int i = 0; i < len; i++) {
        *(nums + i) = rand() % maxNum;
    }
}

void checkOrder(int *nums, int len) {
    for (int i = 1; i < len; i++) {
        if (nums[i] < nums[i - 1]) {
            printf("Sort error!\n");
            exit(0);
        }
    }
}

int main() {
/*
    for (int i = 0; i < 990000; i++) {
        int len = 10 + (rand() % 6);
        int nums[len];
        randNums(nums, len, 100);
        print(nums, len);
        sort(nums, len);
        print(nums, len);
        checkOrder(nums, len);
    }
*/

    for (int i = 0; i < 1; i++) {
        int len = 110000;
        int nums[len];
        randNums(nums, len, INT_MAX);
        long startTime = time(0);
        sort(nums, len);
        printf("elapse time:%lds\n", time(0) - startTime);
        checkOrder(nums, len);
    }

    return 0;
}

