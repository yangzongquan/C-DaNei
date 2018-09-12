/*
 * 彩票生成程序
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int contain(int source[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (*(source + i) == target) {
            return 1;
        }
    }
    return 0;
}

int main() {
    srand(time(0));

    int nums[SIZE] = {0}, i = 0;
    do {
        int num = rand() % MAX + 1;
        if (contain(nums, SIZE, num)) {
            continue;
        }
        nums[i] = num;
        printf(i == 0 ? "%d" : " %d", nums[i]); 

        i++;
    } while (i < SIZE);

    //for (i = 0; i < SIZE; i++) {
    //    printf(i == 0 ? "%d" : " %d", nums[i]); 
    //}
    printf("\n");

    return 0; 
}
