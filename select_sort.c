/*
 * 选择排序。
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
    // 从下标len-1到下标1，依次确定最大数
    for (int pos = len - 1; pos > 0; pos--) {
        // 确定最大数的位置
        // 首先假设0位置最大
        int maxPos = 0;
        for (int i = 1; i <= pos; i++) {
            if (nums[i] > nums[maxPos]) {
                maxPos = i;
            }
        }
        if (maxPos != pos) {
            // 如果最大数不在pos位置，交换过去
            swap(nums + maxPos, nums + pos);
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

