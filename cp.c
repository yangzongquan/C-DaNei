/*
 * 文件复制
 */
#include <stdio.h>
#include <stdlib.h>

int exist(char *pFname) {
    FILE *pFile = fopen(pFname, "rb");
    if (pFile) {
        fclose(pFile);
        pFile = NULL;
        return 1;
    } else {
        return 0; 
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("参数不正确\n");
    }
    char *pSourceFname = argv[1];
    char *pTargetFname = argv[2];

    FILE *pSourceFile = fopen(pSourceFname, "rb");
    if (!pSourceFile) {
        printf("源文件打开失败\n");
        return 0;
    }
    if (exist(pTargetFname)) {
        fclose(pSourceFile);
        pSourceFile = NULL;
        printf("目标文件已经存在\n");
        return 0;
    }
    FILE *pTargetFile = fopen(pTargetFname, "wb");
    if (!pTargetFile) {
        fclose(pSourceFile);
        pSourceFile = NULL;
        printf("目标文件创建失败\n");
        return 0;
    }
    
    char buf[1024] = {0};
    int size = 0;
    while(1) {
        size = fread(buf, sizeof(char), 1024, pSourceFile);
        if (size < 1) break;
        fwrite(buf, sizeof(char), size, pTargetFile);
    }

    fclose(pSourceFile);
    fclose(pTargetFile);
    pSourceFile = NULL;
    pTargetFile = NULL;

    return 0;
}
