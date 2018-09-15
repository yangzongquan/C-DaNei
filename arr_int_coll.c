
/*
 * 采用数组实现的List<int>、Stack<int>数据结构。
 *
 * 自动扩容：
 *     当容量不够用时，自动扩充容量。扩充容量过程中需要重新分配
 *     内存，并复制原有数据。所以，在初始化或者使用过程中明确容
 *     量。
 *
 * 使用前需要初始化分配内存，使用完需要调用销毁函数释放内存。
 *
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arr_int_coll.h"

#define      DEFAULT_CAPACITY       10

/*
typedef struct {
    int *pBuf;
    int capacity;
    int size;
} IList;
*/

// 出错直接退出
void ilErr(char *str) {
    printf("%s\n", str);
    exit(1);
}

/*
 * 返回：初始化成功则返回传参p，失败则返回NULL。
 */
IList *ilInitDef(IList *p) {
    return ilInit(p, DEFAULT_CAPACITY);
}

/*
 * 返回：初始化成功则返回传参p，失败则返回NULL。
 */
IList *ilInit(IList *p, int initCap) {
    if (p->pBuf) {
        // 已经初始化过
        ilErr("Multi init");
    }
    if (initCap <= 0) {
        printf("Illegal arg 'initCap':%d\n", initCap);
        ilErr("");
    }
    // 分配内存
    p->pBuf = (int *)malloc(sizeof(int) * initCap);
    // 分配失败，返回NULL
    if (!p->pBuf) return NULL;
    p->capacity = initCap;
    p->size = 0;
    return p;
}

void ilDestroy(IList *p) {
    free(p->pBuf);
    p->pBuf = NULL;
    p->capacity = 0;
    p->size = 0;
}

void ilClear(IList *p) {
    p->size = 0;
}

// 检查下标是否超出范围
void ilRangeCheck(IList *p, int index) {
    if (index < 0 || index >= p->size) {
        printf("ilRangeCheck(), out of bounds:%d\n", index);
        ilErr("");
    }
}

int ilGet(IList *p, int index) {
    ilRangeCheck(p, index);

    return p->pBuf[index];
}

int ilGrow(IList *p, int minCapacity) {
    // 扩容1.5倍
    int newCapacity = p->capacity + (p->capacity >> 1);
    // 至少满足要求的容量
    newCapacity = newCapacity < minCapacity ? minCapacity : newCapacity;

    int *pNew = (int *) malloc(newCapacity * sizeof(int));
    if (pNew) {
        memcpy(pNew, p->pBuf, sizeof(int) * p->size);
        free(p->pBuf);
        p->pBuf = pNew;
        p->capacity = newCapacity;
        return SUCCESS;
    } else {
        return FAILED;
    }
}

/*
 * 返回：#SUCCESS表示成功，#FAILED表示失败
 */
int ilEnsureCapacity(IList *p, int minCapacity) {
    if (p->capacity >= minCapacity) {
        // 现有容量已经满足
        return SUCCESS;
    }
    // 至少扩容到DEFAULT_CAPACITY
    minCapacity = minCapacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : minCapacity;

    return ilGrow(p, minCapacity);
}

/*
 * 返回：添加元素的下标。
 */
int ilAdd(IList *p, int num) {
    if(!ilEnsureCapacity(p, p->size + 1)) {
        ilErr("Failed to alloc mem, can't add elem");
    }
    p->pBuf[p->size] = num;
    p->size += 1;
    return p->size - 1;
}

/*
 * 添加元素到index下标处，index范围：index >=0 && index <= size。
 */
void ilInsert(IList *p, int index, int num) {
    // index等于size位置合法
    if (index > p->size || index < 0) {
        printf("ilInsert(), out of bounds:%d\n", index);
        ilErr("");
    }
    if (!ilEnsureCapacity(p, p->size + 1)) {
        ilErr("Failed to alloc mem, can't add elem");
    }
    // 把index位置的数及后面数都往后移动一个位置
    int numMoved = p->size - index;
    if (numMoved > 0) {
        // memcpy不可用，复制过程会锁定对于内存??
        for (int i = p->size; i > index; i--) {
            p->pBuf[i] = p->pBuf[i - 1];
        }
    }
    p->pBuf[index] = num;
    p->size += 1;
}

/*
 * 返回：被删除的数
 */
int ilRemove(IList *p, int index) {
    ilRangeCheck(p, index);
    
    int num = p->pBuf[index];

    // 后面数向前移动一个位置
    int numMoved = p->size - index - 1;
    if (numMoved > 0) {
        // memcpy不可用，复制过程会锁定对于内存??
        // memcpy(p->pBuf + index, p->pBuf + index + 1, numMoved);
        int *pDest = p->pBuf + index;
        for (int i = 0; i < numMoved; i++) {
            *(pDest + i) =  *(pDest + i + 1);
        }
    }
    // 大小减一
    p->size -= 1;

    return num;
}

/*
 * 返回：原数
 */
int ilSet(IList *p, int index, int num) {
    ilRangeCheck(p, index);
    
    int tmp = p->pBuf[index];

    p->pBuf[index] = num;

    return tmp;
}

/*
 * 返回：首次找到num的下标，没找到返回-1。
 */
int ilIndex(IList *p, int num) {
    for (int i = 0; i < p->size; i++) {
        if (p->pBuf[i] == num)
            return i;
    }
    return -1;
}

int ilSize(IList *p) {
    return p->size;
}

int ilCapacity(IList *p) {
    return p->capacity;
}

int ilEmpty(IList *p) {
    return p->size == 0;
}

int ilToStrLen(IList *p) {
    int len = 0;
    // 16B-int + 空格 + '\0'
    char s[42] = {0};
    for (int i = 0; i < p->size; i++) {
        sprintf(s, "%d ", p->pBuf[i]);
        len += strlen(s);
    }
    return len;
}

char *ilToString(IList *p, char *str, int len) {
    int strLen = 0;
    // 重置str数组
    memset(str, 0, len);
    // 16B-int + 空格 + '\0'
    char s[42] = {0};
    for (int i = 0; i < p->size; i++) {
        sprintf(s, "%d ", p->pBuf[i]);
        // 检查字符数组是否够长
        strLen += strlen(s);
        if (strLen > len) {
            ilErr("Too small arr, calculate length by ilToStrLen()\n");
        }
        // 合并字符串
        strcat(str, s);
    }
    // 最后一个空格修改为字符串结束符'\0'
    str[len - 1] = 0;

    return str;
}

void ilPrint(IList *p) {
    printf("<");
    for (int i = 0; i < p->size; i++) {
        printf(i == 0 ? "%d" : " %d", p->pBuf[i]);
    }
    printf(">\n");
}


// ------  IStack  ------------------------------------------------

/*
 * 返回：初始化成功则返回传参p，失败则返回NULL。
 */
IStack *isInitDef(IStack *p) {
    return ilInitDef(p);
}


/*
 * 返回：初始化成功则返回传参p，失败则返回NULL。
 */
IStack *isInit(IStack *p, int initCap) {
    return ilInit(p, initCap);
}

/*
 * 清除数据，释放内存
 */
void isDestroy(IStack *p) {
    ilDestroy(p);
}

/*
 * 清除栈中所有元素
 */
void isClear(IStack *p) {
    ilClear(p);
}

int isPeek(IStack *p) {
    return ilGet(p, p->size - 1);
}

int isPop(IStack *p) {
    return ilRemove(p, p->size - 1);
}

void isPush(IStack *p, int num) {
    ilAdd(p, num);
}

/*
 * 确保最小容量
 * 返回：#SUCCESS表示成功，#FAILED表示失败。
 */
int isEnsureCapacity(IStack *p, int minCapacity) {
    return ilEnsureCapacity(p, minCapacity);
}

/*
 * 返回：元素个数
 */
int isSize(IStack *p) {
    return ilSize(p);
}

/*
 * 返回：容量大小
 */
int isCapacity(IStack *p) {
    return ilCapacity(p);
}

/*
 * 返回：0表示空，非0表示非空
 */
int isEmpty(IStack *p) {
    return ilEmpty(p);
}

/*
 * 返回：格式化所有元素用空格隔开的字符串
 */
int isToStrLen(IStack *p) {
    return ilToStrLen(p);
}

/*
 * 返回：传参str
 */

char *isToString(IStack *p, char *str, int len) {
    return ilToString(p, str, len);
}

/*
 * 打印所有元素
 */
void isPrint(IStack *p) {
    ilPrint(p);
}


// ------ test code ---------------------------------------
/*
int main() {
    printf("---- test IList -----------------------------\n");
    IList ilist = {0};
    IList *p = ilInitDef(&ilist);
    printf("initDef() capacity:%d\n", ilCapacity(p));
    ilDestroy(p);
    
    p = ilInit(p, 3);
    printf("init(3) capacity:%d\n", ilCapacity(p));

    ilAdd(p, 1);
    printf("add(1) size:%d\n", ilSize(p));

    ilAdd(p, 2);
    printf("add(2) size:%d\n", ilSize(p));

    ilAdd(p, 3);
    printf("add(3) size:%d\n", ilSize(p));

    ilAdd(p, 4);
    printf("add(4) size:%d\n", ilSize(p));

    ilAdd(p, 5);
    printf("add(5) size:%d\n", ilSize(p));

    int num = ilRemove(p, 1);
    printf("remove(1)=%d, size:%d\n", num, ilSize(p));

    num = ilGet(p, 3);
    printf("get(3)=%d, size:%d\n", num, ilSize(p));

    num = ilSet(p, 0, 9);
    printf("set(0, 9)=%d, size:%d\n", num, ilSize(p));

    ilInsert(p, 1, 2);
    printf("insert(1, 2), size:%d\n", ilSize(p));

    ilEnsureCapacity(p, 20);
    printf("ensureCapacity(20) capacity:%d\n", ilCapacity(p));

    ilAdd(p, -123456789);
    printf("add(-123456789) size:%d\n", ilSize(p));

    int len = ilToStrLen(p);
    printf("toStrLen()=%d\n", len);

    char s[len];
    printf("toString()=<%s>\n", ilToString(p, s, len));

    ilClear(p);
    printf("clear(), size=%d, empty=%d\n", ilSize(p), ilEmpty(p));

    ilDestroy(p);
    printf("destroy() buf:%p, capacity:%d, size:%d\n", p->pBuf, p->capacity, p->size);


    
    printf("\n---- test IStack -----------------------------\n");
    
    IStack *pStk = isInitDef(p);
    printf("initDef() capacity:%d\n", isCapacity(pStk));
    isDestroy(pStk);

    pStk = isInit(pStk, 2);
    printf("init(2) capacity:%d\n", isCapacity(pStk));

    isPush(pStk, 1);
    printf("push(1) size:%d\n", isSize(pStk));

    isPush(pStk, 2);
    printf("push(2) size:%d\n", isSize(pStk));

    isPush(pStk, 3);
    printf("push(3) size:%d\n", isSize(pStk));

    num = isPop(pStk);
    printf("pop()=%d, size:%d\n", num, isSize(pStk));

    printf("peek()=%d, size:%d\n", isPeek(pStk), isSize(pStk));

    isEnsureCapacity(pStk, 20);
    printf("ensureCapacity(20) capacity:%d\n", isCapacity(pStk));

    isPush(pStk, -123456789);
    printf("push(-123456789) size:%d\n", isSize(pStk));

    len = isToStrLen(pStk);
    printf("toStrLen()=%d\n", len);

    char s2[len];
    printf("toString()=<%s>\n", isToString(pStk, s2, len));

    printf("isPrint():\n");
    isPrint(pStk);

    isClear(pStk);
    printf("clear(), size=%d, empty=%d\n", isSize(pStk), isEmpty(pStk));

    isDestroy(pStk);
    printf("destroy() buf:%p, capacity:%d, size:%d\n", pStk->pBuf, pStk->capacity, pStk->size);

    return 0;
}
*/
