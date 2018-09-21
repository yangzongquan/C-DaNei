/*
 * 快速排序。
 */
#include <stdio.h>

void swap(int *num1, int *num2) {
    int tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}

void sort(int nums[], int len) {
    // 头尾下标
    int head = 0, tail = len - 1;
    // 基准数，初始在head位置
    int base = nums[0];
    while (head < tail) {
        if (base == nums[head]) {
            // 基准数在head位置，与tail位置比较
            if (base > nums[tail]) {
                // 基准数比tail位置数大，交换位置
                swap(nums + head, nums + tail);
                // head位置数已经确定了位置，head指向后一个位置
                head++;
            } else {
                // 基准数比tail位置数小，无需位移
                // tail位置数已经确定了位置，tail指向前一个位置
                tail--;
            }
        } else {
            if (base < nums[head]) {
                swap(nums + head, nums + tail);
                tail--;
            } else {
                head++;
            }
        }
    }
    // 此时head与tail相等

    int *left = nums, leftLen = head;
    if (leftLen > 1) {
        sort(left, leftLen);
    }
    int *right = nums + head + 1, rightLen = len - head - 1;
    if (rightLen > 1) {
        sort(right, rightLen);
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

