
// 重构代码：用结构体存储分段和类型、内存释放

/*
 * 简单计算器。
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arr_int_coll.h"
#include "arr_point_coll.h"

#define     LOG       0


enum TYPE {UNKOWN, NUM/*数字*/, ADD, MINUS, MULTI, DIV, LEFT_PAR/*左括号*/, RIGHT_PAR/*右括号*/};

enum STATUS {OK, ERR};

int isOp(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/') ? 1 : 0;
}

int isNum(char c) {
    return (c >= '0' && c <='9') || c == '.' ? 1 : 0;
}

int type(char c) {
    if (isNum(c)) {
        return NUM;
    } else if (c == '+') {
        return ADD;
    } else if (c == '-') {
        return MINUS;
    } else if (c == '*') {
        return MULTI;
    } else if (c == '/') {
        return DIV;
    } else if (c == '(') {
        return LEFT_PAR;
    } else if (c == ')') {
        return RIGHT_PAR;
    } else {
        return UNKOWN;
    }
}

int isOpType(int t) {
    return t == ADD || t == MINUS || t == MULTI || t == DIV;
}

int readNum(char *str, double *pNum) {
    // 确定数字字符的个数
    int skip = 0;
    char c = *str;
    while (isNum(c)) {
        skip++;
        c = *(str + skip);
    }

    char ss[skip + 1];
    memcpy(ss, str, skip);
    *pNum = atof(ss);
    
    return skip;
}

void trim(char *src, char *dest, int len) {
    int sIndex = 0, dIndex = 0;
    for (; sIndex < len; sIndex++) {
        char c = src[sIndex];
        if (isNum(c) || isOp(c) || c == '(' || c == ')') {
            dest[dIndex] = c;
            dIndex++;
        }
    }
    dest[dIndex] = 0;
}

/*
 * 处理数字前面的符号位。
 * 如果确定上一个字段是符号位，则将符号位与数字合并。
 */
void handleSign(PList *pl, IList *il, double *pNum) {
    if (ilEmpty(il)) {
        // 没有正负号
        return;
    }
    int size = ilSize(il);
    int last1 = ilGet(il, size - 1);
    if (last1 != ADD && last1 != MINUS) {
        // 倒数第一个非正负号，不处理
        return;
    }
    if (size == 1) {
        if (last1 == MINUS) {
            // 负号，需要写入数
            *pNum = 0 - *pNum;
        }
        // 正负号已经写入数，从List中删除
        plRemove(pl, size - 1);
        ilRemove(il, size - 1);
        return;
    }
    // size > 1
    int last2 = ilGet(il, size - 2);
    // 只有倒数第二个是运算符或左括号时，认为倒数第一的+-是正负号
    if (isOpType(last2) || last2 == LEFT_PAR) {
        if (last1 == MINUS) {
            // 负号，需要写入数
            *pNum = 0 - *pNum;
        }
        // 正负号已经写入数，从List中删除
        plRemove(pl, size - 1);
        ilRemove(il, size - 1);
        return;
    }
}

void split(char *src, PList *pl, IList *il, int len) {
    int i, t = UNKOWN;
    for (i = 0; i < len; i++) {
        char c = src[i];
        t = type(c);
        if (t == NUM) {
            // 读取数字
            double *p = (double *)calloc(1, sizeof(double));
            int skip = readNum(src + i, p);
            // 处理符号位
            handleSign(pl, il, p);
            // 记录类型和数字
            ilAdd(il, t);
            plAdd(pl,p);
            // 跳过数字字符
            i += (skip - 1);
            if (LOG) printf("split() nu:%lg\n", *p);
        } else if (t == ADD) {
            ilAdd(il, t);
            plAdd(pl, "+");
            if (LOG) printf("split() op:+\n");
        } else if (t == MINUS) {
            ilAdd(il, t);
            plAdd(pl, "-");
            if (LOG) printf("split() op:-\n");
        } else if (t == MULTI) {
            ilAdd(il, t);
            plAdd(pl, "*");
            if (LOG) printf("split() op:*\n");
        } else if (t == DIV) {
            ilAdd(il, t);
            plAdd(pl, "/");
            if (LOG) printf("split() op:/\n");
        } else if (t == LEFT_PAR) {
            ilAdd(il, t);
            plAdd(pl, "(");
            if (LOG) printf("split() op:(\n");
        } else if (t == RIGHT_PAR) {
            ilAdd(il, t);
            plAdd(pl, ")");
            if (LOG) printf("split() op:)\n");
        } else {
            printf("err char:%c, type:%d.\n", c, t);
            exit(0);
        }
    }
}

int priorOp(int t1, int t2) {
    if(t1 == MULTI || t1 == DIV) {
        return 1;
    }
    if (t1 == ADD || t1 == MINUS) {
        if (t2 == MULTI || t2 == DIV) {
            return 0;
        } else {
            return 1;
        }
    }
}

void simpleCalculate(void *pNum1, void *pNum2, int op, double *pResult) {
    double n1 = *((double *)pNum1);
    double n2 = *((double *)pNum2);
    switch (op) {
        case ADD:
            *pResult = n1 + n2;
            if (LOG) printf("smpCalc() %lg + %lg = %lg\n", n1, n2, n1 + n2);
            break;
        case MINUS:
            *pResult = n1 - n2;
            if (LOG) printf("smpCalc() %lg - %lg = %lg\n", n1, n2, n1 - n2);
            break;
        case MULTI:
            *pResult = n1 * n2;
            if (LOG) printf("smpCalc() %lg * %lg = %lg\n", n1, n2, n1 * n2);
            break;
        case DIV:
            *pResult = n1 / n2;
            if (LOG) printf("smpCalc() %lg / %lg = %lg\n", n1, n2, n1 / n2);
            break;
        default:
            printf("Unkown operator\n");
            exit(1);
    }
}

void calculate(PList *pl, IList *il, PStack *ps, IStack *is, int *pStatus, double *pResult) {
    int len = ilSize(il);
    int t1 = ilRemove(il, 0), t2 = UNKOWN;
    void *p1 = plRemove(pl, 0), *p2 = NULL;
    if (LOG) printf("calc()1 t1:%d, p1:%p\n", t1, p1);
    
    for (int i = 0; i < len - 1;) {
        // t2中没有数据时，才取新数据
        // 偶尔t2、p2中数据一次循环处理不完
        if (t2 == UNKOWN) {
            t2 = ilRemove(il, 0);
            p2 = plRemove(pl, 0);
            if (LOG) printf("calc()2 t2:%d, p2:%p\n", t2, p2);
            i++;
        }
        
        if (t1 == LEFT_PAR) {
            if (
                // 前面必须：无字符或是左括号或是操作符
                (isEmpty(is) || isPeek(is) == LEFT_PAR || isOpType(isPeek(is)))
                // 后面必须：左括号或数
                && (t2 == LEFT_PAR || t2 == NUM)) {
                    // 左括号入栈
                    isPush(is, t1);
                    psPush(ps, p1);
                    // 字段前移
                    t1 = t2;
                    p1 = p2;
                    t2 = UNKOWN;
                    p2 = NULL;
                    if (LOG) printf("calc()3 stk:%d,t1:%d,t2:%d -> push t1 to stack\n", 
                            isEmpty(is) ? -1 : isPeek(is), t1, t2);
                    continue;
            }
            // 非法状态，结束计算
            *pStatus = 1;
            if (LOG) printf("calc()4 stk:%d,t1:%d,t2:%d -> illegal status 1.\n", 
                            isEmpty(is) ? -1 : isPeek(is), t1, t2);
            return;
        } else if (isOpType(t1)) {
            if (!isEmpty(is)) {
                int stackType = isPeek(is);
                if (stackType == NUM) {
                    if (t2 == LEFT_PAR || (t2 == NUM && (t1 == ADD || t1 == MINUS))) {
                        if (LOG) printf("calc()5 stk:%d,t1:%d,t2:%d -> push t1 to stack\n", isEmpty(is) ? -1 : isPeek(is), t1, t2);
                        // 入栈
                        isPush(is, t1);
                        psPush(ps, p1);
                        // 字段前移
                        t1 = t2;
                        p1 = p2;
                        t2 = UNKOWN;
                        p2 = NULL;
                        continue;
                    } else if (t2 == NUM && (t1 == MULTI || t1 == DIV)) {
                        if (LOG) printf("calc()6 stk:%d,t1:%d,t2:%d -> calculate\n", 
                            isEmpty(is) ? -1 : isPeek(is), t1, t2);
                        // 计算
                        double *pResult = (double*)malloc(sizeof(double));
                        isPop(is);// stackType已经取出
                        void *stackPoint = psPop(ps);
                        simpleCalculate(stackPoint, p2, t1, pResult);
                        // 释放原来资源
                        free(stackPoint);free(p2);
                        // 字段前移
                        t1 = NUM;
                        p1 = pResult;
                        t2 = UNKOWN;
                        p2 = NULL;
                        continue;
                    }
                } else if (stackType == RIGHT_PAR) {
                    // 算式合法, 但是右括号不会入栈，状态非法
                }
            }
            // 非法状态，结束计算
            *pStatus = 2;
            if (LOG) printf("calc()7 stk:%d,t1:%d,t2:%d -> illegal status 2.\n", 
                            isEmpty(is) ? -1 : isPeek(is), t1, t2);
            return;
        } else if (t1 == NUM) {
            if (
                // 前面必须：无字符或运算符或左括号
                (isEmpty(is) || isPeek(is) == LEFT_PAR || isOpType(isPeek(is))) && 
                // 后面必须：右括号或操作符
                (t2 == RIGHT_PAR || isOpType(t2))) {
                    if (isEmpty(is)) {
                        if (LOG) printf("calc()8 stk:%d,t1:%d,t2:%d -> push t1 to stack\n", isEmpty(is) ? -1 : isPeek(is), t1, t2);
                        // t1、p1入栈
                        isPush(is, t1);
                        psPush(ps, p1);
                        // 字段前移
                        t1 = t2;
                        p1 = p2;
                        t2 = UNKOWN;
                        p2 = NULL;
                        continue;
                    }
                    int stackType = isPeek(is);
                    void *stackPoint = psPeek(ps);
                    if (stackType == LEFT_PAR && isOpType(t2)) {
                        if (LOG) printf("calc()9 stk:%d,t1:%d,t2:%d -> push t1 to stack\n", isEmpty(is) ? -1 : isPeek(is), t1, t2);
                        // t1、p1入栈
                        isPush(is, t1);
                        psPush(ps, p1);
                        // 字段前移
                        t1 = t2;
                        p1 = p2;
                        t2 = UNKOWN;
                        p2 = NULL;
                        continue;
                    } else if (stackType == LEFT_PAR && t2 == RIGHT_PAR) {
                        if (LOG) printf("calc()10 stk:%d,t1:%d,t2:%d -> delete left and right parenthesis\n", isEmpty(is) ? -1 : isPeek(is), t1, t2);
                        // 丢弃左右括号
                        isPop(is);
                        psPop(ps);
                        t2 = UNKOWN;
                        p2 = NULL;
                        continue;
                    } else if (isOpType(stackType) && t2 == RIGHT_PAR) {
                        isPop(is); // stackType已经取出
                        psPop(ps); // 无用
                        int stackType2 = isPop(is);
                        if (stackType2 == NUM) {
                            if (LOG) printf("calc()11 stk2:%d,stk1:%d,t1:%d,t2:%d -> calculate.\n", stackType2, stackType, t1, t2);
                            // 计算
                            double *pResult = (double*)malloc(sizeof(double));
                            void *stackPoint2 = psPop(ps);
                            simpleCalculate(stackPoint2, p1, stackType, pResult);
                            // 释放资源
                            free(stackPoint2);free(p1);
                            // t1、p1赋值为计算后的值
                            // t2、p2值未处理，下次循环处理
                            t1 = NUM;
                            p1 = pResult;
                            continue;
                        }
                    } else if (isOpType(stackType) && isOpType(t2)) {
                        if (priorOp(stackType, t2)) {
                            isPop(is); // stackType已经取出
                            psPop(ps); // 无用
                            int stackType2 = isPop(is);
                            if (stackType2 == NUM) {
                            if (LOG) printf("calc()12 stk2:%d,stk1:%d,t1:%d,t2:%d -> calculate.\n", stackType2, stackType, t1, t2);
                                // 计算
                                double *pResult = (double*)malloc(sizeof(double));
                                void *stackPoint2 = psPop(ps);
                                simpleCalculate(stackPoint2, p1, stackType, pResult);
                                // 释放资源
                                free(stackPoint2);free(p1);
                                // t1、p1赋值为计算后的值
                                // t2、p2值未处理，下次循环处理
                                t1 = NUM;
                                p1 = pResult;
                                continue;
                            }
                        } else {
                            if (LOG) printf("calc()13 stk:%d,t1:%d,t2:%d -> push t1 to stack\n", isEmpty(is) ? -1 : isPeek(is), t1, t2);
                            // t1、p1入栈
                            isPush(is, t1);
                            psPush(ps, p1);
                            // 字段前移
                            t1 = t2;
                            p1 = p2;
                            t2 = UNKOWN;
                            p2 = NULL;
                            continue;
                        }
                    }
            }
            // 非法状态
            *pStatus = 3;
            if (LOG) printf("calc()14 stk:%d,t1:%d,t2:%d -> illegal status 3.\n", 
                            isEmpty(is) ? -1 : isPeek(is), t1, t2);
            return;
        } else {
            // 非法状态
            *pStatus = 4;
            if (LOG) printf("calc()15 stk:%d,t1:%d,t2:%d -> illegal status 4.\n", 
                            isEmpty(is) ? -1 : isPeek(is), t1, t2);
            return;
        }
    }

    if (t1 != NUM) {
        // 非法状态
        if (LOG) printf("calc()14 stk:%d,t1:%d,t2:%d -> illegal status 5.\n", 
                isEmpty(is) ? -1 : isPeek(is), t1, t2);
        *pStatus = 5;
        return;
    }

    if (LOG) printf("calc()16 t1:%d, p1:%lg, t2:%d, p2:%p\n", t1, *((double *)p1), t2, p2);

    // 只剩下p1的数及栈数据
    while (isSize(is) > 1) {
        int stackType1 = isPop(is);
        void *stackPoint1 = psPop(ps);

        int stackType2 = isPop(is);
        void *stackPoint2 = psPop(ps);
        
        if (isOpType(stackType1) && stackType2 == NUM) {
            // 计算
            double *pResult = (double*)malloc(sizeof(double));
            simpleCalculate(stackPoint2, p1, stackType1, pResult);
            // 释放资源
            free(stackPoint2);free(p1);
            // t1、p1赋值为计算后的值
            t1 = NUM;
            p1 = pResult;
        } else {
            if (LOG) printf("Illegal status\n");
        }
    }

    // 保存结果
    *pResult = *((double *)p1);
    free(p1);
}



int main() {
    
    printf("支持加减乘除运算、括号、正负号。\n"
            "算式最长1023个字符。\n"
            "直接输入回车退出。\n");
    while (1) {
        // 从用户键盘输入获取算式
        char *src = (char *)malloc(1024*sizeof(char));
        printf("请输入算式：");
        fgets(src, 1024, stdin);
        int len = strlen(src);
        if (len == 1023 && src[len - 1] != '\n') {
            scanf("%*[^\n]");
            scanf("%*c");
        }
        if (src[len - 1] == '\n') {
            src[len - 1] = 0;
            len--;
        }
        // 没输入算式，退出
        if (len == 0) break;
        if (LOG) printf("len:%d, exp:<%s>\n", len, src);

        // 去除算式中无效字符
        char *dest = (char *)malloc(1024*sizeof(char));
        trim(src, dest, len);
        free(src);
        len = strlen(dest);
        if (LOG) printf("lengthTrimed:%d\n", len);
        // 算式无效，重新输入
        if (len == 0) continue;
        printf("%s\n", dest);

        // 初始化列表
        PList pList = {0}; // 分段值指针的列表
        PList *pl = plInitDef(&pList);
        IList iList = {0}; // 分段类型的列表
        IList *il = ilInitDef(&iList);
        // 初始化栈
        PStack pStack = {0}; // 分段值的栈
        PStack *ps = psInitDef(&pStack);
        IStack iStack = {0}; // 分段类型的栈
        IStack *is = isInitDef(&iStack);

        // 分段
        split(dest, pl, il, len);
        free(dest);
        if(LOG) ilPrint(il);
        if(LOG) plPrint(pl);

        // 计算
        int status = OK;
        double result = 0;
        calculate(pl, il, ps, is, &status, &result);
        if (status == OK) {
            printf("结果:%lg\n", result);
        } else {
            printf("Error %d\n", status);
        }

        // 释放资源
        plDestroy(pl);
        ilDestroy(il);
        psDestroy(ps);
        isDestroy(is);
    }
    return 0;
}

//void split(char *src, PList *pl, IList *il, int len);
//void calculate(PList *pl, IList *il, int *pStatus, double *pResult);
