/*
 * 快速排序。
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
    if (len < 2) {
        return;
    }
    // 头尾下标
    int head = 0, tail = len - 1;
    // 基准数，初始在head位置
    int base = nums[0];
    while (head < tail) {
        // 如果head位置数大，则交换位置，否则不交换
        if (nums[head] > nums[tail]) {
            swap(nums + head, nums + tail);
        }
        if (base == nums[head]) {
            // 如果基准数在head位置，tail位置数位置确定，tail指向前一个位置
            tail--;
        } else {
            // 如果基准数在tail位置，head位置数位置确定，head指向后一个位置
            head++;
        }
    }
    // 此时head与tail相等

    // 左组数递归排序
    int *left = nums, leftLen = head;
    sort(left, leftLen);

    // 右组数递归排序
    int *right = nums + head + 1, rightLen = len - head - 1;
    sort(right, rightLen);
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
        int len = 99990000;
        int *nums = (int *) malloc(sizeof(int) * len);
        randNums(nums, len, INT_MAX);
        long startTime = time(0);
        sort(nums, len);
        printf("elapse time:%lds\n", time(0) - startTime);
        checkOrder(nums, len);
        free(nums);
    }

    return 0;
}

