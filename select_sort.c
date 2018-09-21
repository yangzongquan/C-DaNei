/*
 * 选择排序。
 */
#include <stdio.h>

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

int main() {
    int nums[9] = {50, 30, 80, 60, 20, 40, 10, 90, 20};
    print(nums, 9);
    sort(nums, 9);
    print(nums, 9);
    return 0;
}

