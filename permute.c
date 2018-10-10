
#include <stdio.h>
#include <time.h>

void print(int nums[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", *(nums + i));
    }
    printf("\n");
}

void swap(int nums[], int pos1, int pos2) {
    int tmp = nums[pos1];
    nums[pos1] = nums[pos2];
    nums[pos2] = tmp;
}

void permute(int src[], int startPos, int srcLen) {
    if (startPos == srcLen - 1) {
        print(src, srcLen);
    } else {
        for (int i = startPos; i < srcLen; i++) {
            swap(src, startPos, i);
            permute(src, startPos + 1, srcLen);
            swap(src, startPos, i);
        }
    }
}

void combine(int src[], int srcStartPos, int srcLen, int sub[], int subStartPos, int subLen){
    for (int i =srcStartPos; i < srcLen; i++) {
        sub[subStartPos] = src[i];
        if (subStartPos < subLen - 1) {
            combine(src, i + 1, srcLen, sub, subStartPos + 1, subLen);
        } else {
            // 这时，sub为新的组合
            //print(sub, subLen);
        }
    }
}

void comb(int src[], int len, int sublen) {
    // 存放组合的数组
    int sub[sublen];
    for (int i = 0; i < len; i++) {
        // 从i开始取出sublen-1个数，依次从sub数组0位置开始往后放，
        // 循环完，sub数组中最后位置没放数
        for (int j = i; j < i + sublen - 1; j++) {
            sub[j - i] = src[j];
        }
        // 从tail=i+sublen-1开始，依次把每个数当作sub数组的尾数
        for (int tail = i + sublen - 1; tail < len; tail++) {
            sub[sublen - 1] = src[tail];
            // 这时，sub为新的组合
            //print(sub, sublen);
        }
    }
}

int main(void) {

    // test permute
    int nums[] = {1, 2, 3};
    permute(nums, 0, 3);

    printf("\n\n");


    // test combine
    int len = 50;
    int src[len];
    for (int i = 0; i < len; i++) {
        src[i] = i + 1;
    }

    long start = time(0);
    int subset[10] = {0};
    combine(src, 0, len, subset, 0, 10);
    printf("combine elapse time:%lds\n", time(0) - start);

    start = time(0);
    comb(src, len, 10);
    printf("comb elapse time:%lds\n", time(0) - start);
    // 注：随着组合长度sublen长度增加，combine的速度极慢
    return 0;
}

