/*
 * 计算成绩
 * */

#include <stdio.h>

int main() {
    int count = 0;
    printf("请输入人数：");
    scanf("%d", &count);
    if (count < 0) {
        printf("输入错误！\n");
        return 0;
    }

    int goals[count], max = -1, min = -1;
    float total = 0, average = 0;
    for (int i = 0; i < count; i++) {
        goals[i] = -1;
        printf("请输入第%d人成绩：", i + 1);
        scanf("%d", &goals[i]);
        
        if (goals[i] >= 0) {
            total += goals[i];
            if (goals[i] > max || max == -1)
                max = goals[i];
            if (goals[i] < min || min == -1)
                min = goals[i];
        }
    }

    average = total / count;

    printf("最高分%d，最低分%d，平均分%g，总成绩%g\n", max, min, average, total);

    return 0;
}
