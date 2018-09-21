/*
 * 冒泡排序。
 */
#include <stdio.h>

void swap(int *num1, int *num2) {
    int tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}

void sort(int nums[], int len) {
    // 需要从下标len-1到下标1，依次确定最大数
    for (int pos = len - 1; pos > 0; pos--) {
        // 确定pos位置的最大数
        for (int i = 0; i < pos; i++) {
            if (nums[i] > nums[i+1]) {
                swap(nums + i, nums + i + 1);
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

int main() {
    int nums[9] = {50, 30, 80, 60, 20, 40, 10, 90, 20};
    print(nums, 9);
    sort(nums, 9);
    print(nums, 9);
    return 0;
}

