/*
 * 二分查找
 */

#include <stdio.h>
#include <stdlib.h>

int search(int target, int *nums, int len) {
    // 查找范围的头尾下标
    int head = 0, tail = len - 1;
    while (head <= tail) {
        // 中间位置下标
        int mid = (head + tail) / 2;
        if (nums[mid] == target) {
            return mid;
        }
        if (target < nums[mid]) {
            // 如果目标数小于中间位置数，
            // 则查找前半部分元素
            tail = mid - 1;
        } else {
            // 如果目标数大于中间位置数，
            // 则查找后半部分元素
            head = mid + 1;
        }
    }
    return -1;
}

void genNums(int *nums, int len, int min) {
    for (int i = 0; i < len; i++) {
        nums[i] = min + i;
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
    int maxLen = 15;
    int nums[maxLen];
    for (int i = 0; i < 100; i++) {
        int len = rand() % maxLen;
        genNums(nums, len, i);
        int target = len == 0 ? i : rand() % len + i;
        print(nums, len);
        printf("查找%d，位置在%d\n\n", target, search(target, nums, len));
    }
    return 0;
}
