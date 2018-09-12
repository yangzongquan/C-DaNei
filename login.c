/*
 * 模拟登录
 */
#include <stdio.h>
#include <string.h>

int main() {
    int success = 0;
    for (int i = 0; i < 3; i++) {
        printf("请输入用户名：");
        char userName[20] = {0};
        fgets(userName, 20, stdin);
        if (strlen(userName) == 19 && userName[18] != '\n') {
            scanf("%*[^\n]%*c");
            //scanf("%*c");
        }

        printf("请输入密码：");
        char password[20] = {0};
        fgets(password, 20, stdin);
        if (strlen(password) == 19 && password[18] != '\n') {
            scanf("%*[^\n]%*c");
            //scanf("%*c");
        }

        //printf("name:%s(%ld), pwd:%s(%ld))\n", userName, strlen(userName), password, strlen(password));

        if (!strcmp(userName, "admin\n") && !strcmp(password, "123456\n")) {
           success = 1;
           break;
        } else {
            printf("密码或用户名错误\n");
        }
    }
    printf(success ? "登录成功\n" : "登录失败\n");

    return 0;
}
