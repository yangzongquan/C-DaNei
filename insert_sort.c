/*
 * 插入排序。
 */
#include <stdio.h>

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

int main() {
    int nums[9] = {50, 30, 80, 60, 20, 40, 10, 90, 20};
    print(nums, 9);
    sort(nums, 9);
    print(nums, 9);
    return 0;
}

