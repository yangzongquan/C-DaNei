
/*
 * 简单计算器。
 * 支持加减乘除运算，支持正负数、浮点数，支持括号。
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parray.h"
#include "pllink.h"

#define     DEBUG       0
#define     LOG         1
#define     ERROR       1

typedef struct {
    int t; // enum TYPE
    double num;
} Item;

// 新建一个Item，存放运算符或括号或数字
Item *newItem(int type, double num) {
    Item *item = (Item *) malloc(sizeof(Item));
    item->t = type;
    item->num = num;
    return item;
}

// for debug
void printItem(char *str, Item *i) {
    printf("%s->type:%d, num:%lg\n", str, i->t, i->num);
}

enum TYPE {UNKOWN, NUM/*数字*/, ADD, MINUS, MULTI, DIV, LEFT_PAR/*左括号*/, RIGHT_PAR/*右括号*/};

enum STATUS {OK, ERR};

// 判断是否为运算符字符
int isOp(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/') ? 1 : 0;
}

// 判断是否为数字字符
int isNum(char c) {
    return (c >= '0' && c <='9') || c == '.' ? 1 : 0;
}

// 字符转为TYPE
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

// for debug
// type转化为字符串
char *typeStr(int type) {
    switch (type) {
        case ADD:
            return "+";
        case MINUS:
            return "-";
        case MULTI:
            return "*";
        case DIV:
            return "/";
        case LEFT_PAR:
            return "(";
        case RIGHT_PAR:
            return ")";
        case NUM:
            return "num";
        default:
            return "unkown";
    }
}

// 判断是否为运算符type
int isOpType(int t) {
    return t == ADD || t == MINUS || t == MULTI || t == DIV;
}

// 从指定str指针读取一个数
// 返回：读取用来转化为数字的字符个数
int readNum(char *str, double *pNum) {
    // 确定数字字符的个数
    int skip = 0;
    char c = *str;
    while (isNum(c)) {
        skip++;
        c = *(str + skip);
    }

    // 把数字字符复制出来
    char ss[skip + 1];
    memcpy(ss, str, skip);
    ss[skip] = '\0'; // 字符串结束符
    // 转换为数字
    *pNum = atof(ss);
    
    return skip;
}

// 清除算数表达式以外的字符
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
void handleSign(PLLinkList *l, double *pNum) {
    if (pllEmpty(l)) {
        // 没有正负号
        return;
    }
    int size = pllSize(l);
    Item *item1 = (Item *) pllGetLast(l);
    if (item1->t != ADD && item1->t != MINUS) {
        // 倒数第一个非正负号，不处理
        return;
    }
    if (size == 1) {
        if (item1->t == MINUS) {
            // 负号，需要写入数
            *pNum = 0 - *pNum;
        }
        // 正负号已经写入数，从List中删除
        pllRemoveLast(l);
        free(item1);
        item1 = NULL;
        return;
    }
    // size > 1
    Item *item2 = (Item *) pllGet(l, size - 2);
    // 只有倒数第二个是运算符或左括号时，认为倒数第一的+-是正负号
    if (isOpType(item2->t) || item2->t == LEFT_PAR) {
        if (item1->t == MINUS) {
            // 负号，需要写入数
            *pNum = 0 - *pNum;
        }
        // 正负号已经写入数，从List中删除
        pllRemoveLast(l);
        free(item1);
        item1 = NULL;
        return;
    }
}

// for debug
void dumpPLLinkList(PLLinkList *l) {
    printf("[");
    pllMoveToBeforeFirst(l);
    int size = pllSize(l);
    for (int i = 0; i < size && pllHasNext(l); i++) {
        Item *item = (Item *) pllNext(l);
        printf(i == 0 ? "%s(%g)" : ", %s(%g)", typeStr(item->t), item->num);
    }
    printf("]\n");
}

// for debug
void dumpPArrayStack(PArrayStack *s) {
    printf("[");
    int size = paStkSize(s);
    for (int i = 0; i < size; i++) {
        Item *item = (Item *) paStkGet(s, i);
        printf(i == 0 ? "%s(%g)" : ", %s(%g)", typeStr(item->t), item->num);
    }
    printf("]\n");
}

// 拆分表达式。
// 一个数为一个Item，一个运算符为一个Item，一个括号为一个Item。
void split(char *src, int len, PLLinkList *l) {
    int i, t = UNKOWN;
    for (i = 0; i < len; i++) {
        char c = src[i];
        t = type(c);
        if (t == NUM) {
            // 读取数字
            double num = 0;
            int skip = readNum(src + i, &num);
            // 处理符号位
            handleSign(l, &num);
            // 记录类型和数字
            Item *item = newItem(t, num);
            pllAdd(l, item);

            if (DEBUG) {
                printf("num:%g, type:%d, skip:%d, char:%c\n", 
                        item->num, item->t, skip, c);
                dumpPLLinkList(l);
            }

            // 跳过数字字符
            i += (skip - 1);
        } else if (isOpType(t) || t == LEFT_PAR || t == RIGHT_PAR) {
            // 记录运算符或括号Item
            Item *item = newItem(t, 0);
            pllAdd(l, item);

            if (DEBUG) {
                printf("num:%g, type:%d, skip:%d, char:%c\n", 
                        item->num, item->t, 1, c);
                dumpPLLinkList(l);
            }
        } else {
            if(ERROR) printf("err char:%c, type:%d.\n", c, t);
            exit(1);
        }
    }
}

// 判断t1操作符是否优先计算。
int priorOp(int t1, int t2) {
    if(t1 == MULTI || t1 == DIV) {
        // t1为乘除，优先计算
        return 1;
    } else {
        // t1为加减，只有t2也为加减时，优先计算
        // 否则不计算
        return (t2 == ADD || t2 == MINUS);
    }
}

// 计算简单算式：数+运算符+数
void simpleCalculate(double n1, double n2, int op, double *pResult) {
    switch (op) {
        case ADD:
            *pResult = n1 + n2;
            if (DEBUG) printf("smpCalc() %lg + %lg = %lg\n", n1, n2, n1 + n2);
            break;
        case MINUS:
            *pResult = n1 - n2;
            if (DEBUG) printf("smpCalc() %lg - %lg = %lg\n", n1, n2, n1 - n2);
            break;
        case MULTI:
            *pResult = n1 * n2;
            if (DEBUG) printf("smpCalc() %lg * %lg = %lg\n", n1, n2, n1 * n2);
            break;
        case DIV:
            *pResult = n1 / n2;
            if (DEBUG) printf("smpCalc() %lg / %lg = %lg\n", n1, n2, n1 / n2);
            break;
        default:
            if(ERROR) printf("Unkown operator\n");
            exit(1);
    }
}

// for debug
void printStatus(char *str, PArrayStack *s, Item *i1, Item *i2) {
    printf("%s stk2:%s stk:%s,i1:%s,t2:%s\n", str,
        paStkSize(s) < 2 ? "null" : typeStr(((Item *)paStkGet(s, paStkSize(s) - 2))->t),
        paStkEmpty(s) ? "null" : typeStr(((Item *)paStkPeek(s))->t),
        i1 == NULL ? "null" : typeStr(i1->t),
        i2 == NULL ? "null" : typeStr(i2->t));
}

// 计算PLLinkList中拆分过的算式
void calculate(PLLinkList *l, int *pStatus, double *pResult) {
    PArrayStack stack = {0};
    PArrayStack *s = paStkInitDef(&stack);

    int len = pllSize(l);
    // 存放从List中删除的Item
    // 被重新赋值时一定要free
    Item *i1 = pllRemoveFirst(l), *i2 = NULL;
    // 一般存放从s栈顶peek/get两个Item，不要free
    // 但如果pop出来了，一定要free
    Item *stkItem = NULL, *stkItem2 = NULL;
    if (DEBUG) printItem("calc(1) item1", i1);
    
    while (1) {
        // item2中没有数据时，才取新数据
        // 偶尔item2中数据一次循环处理不完
        if (i2 == NULL) {
            if (pllSize(l) == 0) {
                // i2已经被处理，且PLLinkList为空时，退出循环
                break;
            }
            i2 = pllRemoveFirst(l);
            if (DEBUG) printItem("calc(2) item2:", i2);
        }
        
        if (i1->t == LEFT_PAR) {
            if (
                // 前面必须：无字符或是左括号或是操作符
                (paStkEmpty(s) || ((Item *)paStkPeek(s))->t == LEFT_PAR
                 || isOpType(((Item *)paStkPeek(s))->t))
                // 后面必须：左括号或数
                && (i2->t == LEFT_PAR || i2->t == NUM)) {
                    if (DEBUG) printStatus("calc(3) push i1 to stack,", s, i1, i2);
                    // 左括号入栈
                    paStkPush(s, i1);
                    // 字段前移
                    i1 = i2;
                    i2 = NULL;
                    continue;
            }
            // 非法状态，结束计算
            *pStatus = 1;
            if (ERROR) printStatus("calc(4) illegal status 1,", s, i1, i2);
            goto OVER;
        } else if (isOpType(i1->t)) {
            if (!paStkEmpty(s)) {
                stkItem = (Item *)paStkPeek(s);
                if (stkItem->t == NUM) {
                    if (i2->t == LEFT_PAR
                            || (i2->t == NUM && (i1->t == ADD || i1->t == MINUS))) {
                        if (DEBUG) printStatus("calc(5) push i1 to stack,", s, i1, i2);
                        // i1入栈
                        paStkPush(s, i1);
                        // 字段前移
                        i1 = i2;
                        i2 = NULL;
                        continue;
                    } else if (i2->t == NUM && (i1->t == MULTI || i1->t == DIV)) {
                        if (DEBUG) printStatus("calc(6) calculate,", s, i1, i2);
                        // 计算
                        double num = 0;
                        simpleCalculate(stkItem->num, i2->num, i1->t, &num);
                        // 释放stkItem
                        paStkPop(s);
                        free(stkItem);
                        stkItem = NULL;
                        // 释放i2
                        free(i2);
                        i2 = NULL;
                        // 字段前移
                        i1->t = NUM;
                        i1->num = num;
                        continue;
                    }
                } else if (stkItem->t == RIGHT_PAR) {
                    // 算式合法, 但是右括号不会入栈，状态非法
                }
            }
            // 非法状态，结束计算
            *pStatus = 2;
            if (ERROR) printStatus("calc(7) illegal status 2,", s, i1, i2);
            goto OVER;
        } else if (i1->t == NUM) {
            if (
                // 前面必须：无字符或运算符或左括号
                (paStkEmpty(s) || ((Item *)paStkPeek(s))->t == LEFT_PAR
                                || isOpType(((Item *)paStkPeek(s))->t)) && 
                // 后面必须：右括号或操作符
                (i2->t == RIGHT_PAR || isOpType(i2->t))) {
                    if (paStkEmpty(s)) {
                        if (DEBUG) printStatus("calc(8) push i1 to stack,", s, i1, i2);
                        // i1入栈
                        paStkPush(s, i1);
                        // 字段前移
                        i1 = i2;
                        i2 = NULL;
                        continue;
                    }
                    stkItem = (Item *) paStkPeek(s);
                    if (stkItem->t == LEFT_PAR && isOpType(i2->t)) {
                        if (DEBUG) printStatus("calc(9) push i1 to stack,", s, i1, i2);
                        // i1入栈
                        paStkPush(s, i1);
                        // 字段前移
                        i1 = i2;
                        i2 = NULL;
                        continue;
                    } else if (stkItem->t == LEFT_PAR && i2->t == RIGHT_PAR) {
                        if (DEBUG) printStatus("calc(10) del left&right parenthesis,",
                                s, i1, i2);
                        // 丢弃左右括号
                        paStkPop(s);
                        free(stkItem);
                        stkItem = NULL;
                        free(i2);
                        i2 = NULL;
                        continue;
                    } else if (isOpType(stkItem->t) && i2->t == RIGHT_PAR) {
                        Item *stkItem2 = paStkGet(s, paStkSize(s) - 2);
                        if (stkItem2->t == NUM) {
                            if (DEBUG) printStatus("calc(11) calculate,", s, i1, i2);
                            // 计算
                            double num = 0;
                            simpleCalculate(stkItem2->num, i1->num, stkItem->t, &num);
                            // 释放stkItem
                            paStkPop(s);
                            free(stkItem);
                            stkItem = NULL;
                            // 释放stkItem2
                            paStkPop(s);
                            free(stkItem2);
                            stkItem2 = NULL;
                            // i1赋值为计算后的值
                            // i2值未处理，下次循环处理
                            i1->t = NUM;
                            i1->num = num;
                            continue;
                        }
                    } else if (isOpType(stkItem->t) && isOpType(i2->t)) {
                        if (priorOp(stkItem->t, i2->t)) {
                            // 取出stkItem2
                            Item *stkItem2 = paStkGet(s, paStkSize(s) - 2);
                            if (stkItem2->t == NUM) {
                                if (DEBUG) printStatus("calc(12) calculate,", s, i1, i2);
                                // 计算
                                double num = 0;
                                simpleCalculate(stkItem2->num, i1->num, stkItem->t, &num);
                                // 释放stkItem
                                paStkPop(s);
                                free(stkItem);
                                stkItem = NULL;
                                // 释放stkItem2
                                paStkPop(s);
                                free(stkItem2);
                                stkItem2 = NULL;
                                // t1、p1赋值为计算后的值
                                // t2、p2值未处理，下次循环处理
                                i1->t = NUM;
                                i1->num = num;
                                continue;
                            }
                        } else {
                            if (DEBUG) printStatus("calc(13) push i1 to stack,", s, i1, i2);
                            // i1入栈
                            paStkPush(s, i1);
                            // 字段前移
                            i1 = i2;
                            i2 = NULL;
                            continue;
                        }
                    }
            }
            // 非法状态
            *pStatus = 3;
            if (ERROR) printStatus("calc(14) illegal status 3,", s, i1, i2);
            goto OVER;
        } else {
            // 非法状态
            *pStatus = 4;
            if (ERROR) printStatus("calc(15) illegal status 4,", s, i1, i2);
            goto OVER;
        }
    }

    if (i1->t != NUM) {
        // 非法状态
        if (ERROR) printStatus("calc(16) illegal status 5,", s, i1, i2);
        *pStatus = 5;
        goto OVER;
    }

    if (DEBUG) printf("calc(17) t1:%d, p1:%g\n", i1->t, i1->num);
    if (DEBUG) printStatus("calc(17) while end,", s, i1, i2);

    // 只剩下i1的数及栈数据
    while (paStkSize(s) > 1) {
        stkItem = paStkPeek(s);
        stkItem2 = paStkGet(s, paStkSize(s) - 2);
        
        if (isOpType(stkItem->t) && stkItem2->t == NUM) {
            // 计算
            double num = 0;
            simpleCalculate(stkItem2->num, i1->num, stkItem->t, &num);
            // 释放stkItem
            paStkPop(s);
            free(stkItem);
            stkItem = NULL;
            // 释放stkItem2
            paStkPop(s);
            free(stkItem2);
            stkItem2 = NULL;
            // i1赋值为计算后的值
            i1->t = NUM;
            i1->num = num;
        } else {
            if (ERROR) {
                printf("Illegal status\n");
                dumpPArrayStack(s);
            }
            goto OVER;
        }
    }

    // 保存结果
    *pResult = i1->num;

OVER:
    if (i1 != NULL) {
        free(i1);
        i1 = NULL;
    }
    if (i2 != NULL) {
        free(i2);
        i2 = NULL;
    }

    // stkItem和stkItem2调用pop时都free了，所以不能再次free

    int listSize = pllSize(l);
    for (int i = 0; i < listSize; i++) {
        Item *tmp = (Item *) pllRemoveLast(l);
        if (tmp) {
            free(tmp);
            tmp = NULL;
        }
    }
    int stkSize = paStkSize(s);
    for (int i = 0; i < stkSize; i++) {
        Item *tmp = (Item *) paStkPop(s);
        if (tmp) {
            free(tmp);
            tmp = NULL;
        }
    }
    paStkDestroy(s);
    s = NULL;
}



int startCalc(char *src) {
    int len = strlen(src);
    // 没输入算式，退出
    if (len == 0) {
        if(ERROR) printf("No expression\n");
        return 0;
    }
    if (DEBUG) printf("input->len:%d, exp:<%s>\n", len, src);

    // 去除算式中无效字符
    char *dest = (char *)malloc(1024*sizeof(char));
    trim(src, dest, len);
    len = strlen(dest);
    // 算式无效，退出
    if (len == 0) {
        if(ERROR) printf("Invalid expression\n");
        return 0;
    }
    if (DEBUG) printf("trimed->len:%d, exp:<%s>\n", len, dest);
    if (LOG) printf("算式:%s\n", dest);

    // 初始化列表
    PLLinkList pList = {0};
    PLLinkList *l = pllInit(&pList);

    // 分段
    split(dest, len, l);
    free(dest);
    if(DEBUG) printf("\n");

    // 计算
    int status = OK;
    double result = 0;
    calculate(l, &status, &result);
    if (status == OK) {
        if(LOG) printf("结果:%lg\n", result);
    } else {
        if(ERROR) printf("Error %d\n", status);
    }

    // 释放资源
    pllDestroy(l);

    return 0;
}


//**************** test code ******************************

// 随机一个double数
// 小于100
double randNum() {
    double num = (rand() % 10000) / 100.0;
    // 25%概率出现负数
    return rand() % 4 == 0 ? -num : num;
}

// 随机一个操作符
char randOp() {
    switch (rand() % 4) {
        case 0:
            return '+';
        case 1:
            return '-';
        case 2:
            return '*';
        default:
            return '/';
    }
}

// 随机一个表达式长度
// 至少能组成一个表达式
int randLen(int len) {
    int newLen = rand() % (len + 1);
    newLen = newLen < 14 ? 14 : newLen;
    newLen = newLen > 30 ? 30 : newLen;
    return newLen;
}

// 生成表达式字符串。
void expGen(char str[], int len) {
    if (len < 14) {
        return;
    }
    memset(str, '\0', len);
    // 保留结束符
    len--;

    // 首个数字
    double num = randNum();
    sprintf(str, "%lg", num);
    
    // 循环追加“运算符+数(或括号中表达式)”
    int size = strlen(str);
    while (size <= (len - 7)) {
        // 运算符
        sprintf(str + size, "%c", randOp());

        size = strlen(str);
        int percent = rand() % 100;
        if (len - size < 16 || percent < 50) {
            // 直接追加数字
            sprintf(str + size, "%lg", randNum());
        } else {
            // 追加括号表达式
            sprintf(str + size, "%c", '(');
            size = strlen(str);
            expGen(str + size, randLen(len - size - 2));
            size = strlen(str);
            sprintf(str + size, "%c", ')');
        }
        size = strlen(str);
    }
}

// 生产count个表达式测试
void test(int count, int strLen) {
    for (int i = 0; i < count; i++) {
        char str[strLen];
        expGen(str, strLen);
        if (LOG) printf("exp:%s\n", str);

        startCalc(str);
    }
}
//**************** test code ******************************

int main(int argc, char *argv[]) {
/*
    //while (1)
        test(30, 80);
*/

///*
    if (argc < 2) {
        printf("支持加减乘除运算、括号、正负号。\n");
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        char *src = argv[i];
        if (strlen(src) == 0) continue;
        startCalc(src);
    }
//*/
    return 0;
}
