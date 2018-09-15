/*
 * 采用数组实现的List<void *>、Stack<void *>数据结构。
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
#include <stdlib.h>
#include <string.h>
#include "arr_point_coll.h"

#define         DEFAULT_CAPACITY      10

//#define       SUCCESS      1
//#define       FAILED       0


// ------ IList -----------------------------------------------

/*
typedef struct {
    void **ppBuf;
    int capacity;
    int size;
} PList;
*/

// 出错直接退出
void plErr(char *str) {
    printf("%s\n", str);
    exit(1);
}

/*
 * 返回：初始化成功则返回传参ppl，失败则返回NULL。
 */
PList *plInitDef(PList *ppl) {
    return plInit(ppl, DEFAULT_CAPACITY);
}

/*
 * 返回：初始化成功则返回传参ppl，失败则返回NULL。
 */
PList *plInit(PList *ppl, int initCap) {
    if (ppl->ppBuf) {
        // 已经初始化过
        plErr("Multi init");
    }
    if (initCap <= 0) {
        printf("Illegal arg:%d\n", initCap);
        plErr("");
    }
    // 分配内存
    ppl->ppBuf = (void **)malloc(initCap * sizeof(void *));
    // 分配失败
    if (!ppl->ppBuf) return NULL;
    ppl->capacity = initCap;
    ppl->size = 0;
    
    return ppl;
}

void plDestroy(PList *ppl) {
    free(ppl->ppBuf);
    ppl->ppBuf = NULL;
    ppl->capacity = 0;
    ppl->size = 0;
}

void plClear(PList *ppl) {
    ppl->size = 0;
}

// 检查下标是否超出范围
void plRangeCheck(PList *ppl, int index) {
    if (index < 0 || index >= ppl->size) {
        printf("plRangeCheck(), out of bounds:%d\n", index);
        plErr("");
    }
}

void *plGet(PList *ppl, int index) {
    plRangeCheck(ppl, index);
    return ppl->ppBuf[index];
}

int plGrow(PList *ppl, int minCapacity) {
    // 扩容1.5倍
    int newCapacity = ppl->capacity + (ppl->capacity >> 1); 
    // 至少满足要求的容量
    newCapacity = newCapacity < minCapacity ? minCapacity : newCapacity;

    void **ppNew = (void **) malloc(newCapacity * sizeof(void *));
    if (ppNew) {
        memcpy(ppNew, ppl->ppBuf, sizeof(void *) * ppl->size);
        free(ppl->ppBuf);
        ppl->ppBuf = ppNew;
        ppl->capacity = newCapacity;
        return SUCCESS;
    } else {
        return FAILED;
    }   
}

/*
 * 返回：#SUCCESS表示成功，#FAILED表示失败
 */
int plEnsureCapacity(PList *ppl, int minCapacity) {
    if (ppl->capacity >= minCapacity) {
        // 现有容量已经满足
        return SUCCESS;
    }   
    // 至少扩容到DEFAULT_CAPACITY
    minCapacity = minCapacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : minCapacity;

    return plGrow(ppl, minCapacity);
}

/*
 * 返回：添加元素的下标。
 */
int plAdd(PList *ppl, void *pValue) {
    if(!plEnsureCapacity(ppl, ppl->size + 1)) {
        plErr("Failed to alloc mem, can't add elem");
    }
    ppl->ppBuf[ppl->size] = pValue;
    ppl->size += 1;
    return ppl->size - 1;
}

/*
 * 添加元素到index下标处，index范围：index >=0 && index <= size。
 */
void plInsert(PList *ppl, int index, void *pValue) {
        // index等于size位置合法
    if (index > ppl->size || index < 0) {
        printf("plInsert(), out of bounds:%d\n", index);
        plErr("");
    }
    if (!plEnsureCapacity(ppl, ppl->size + 1)) {
        plErr("Failed to alloc mem, can't add elem");
    }
    // 把index位置的指针及后面指针都往后移动一个位置
    int numMoved = ppl->size - index;
    if (numMoved > 0) {
        // memcpy不可用，复制过程会锁定对于内存??
        for (int i = ppl->size; i > index; i--) {
            ppl->ppBuf[i] = ppl->ppBuf[i - 1];
        }
    }
    ppl->ppBuf[index] = pValue;
    ppl->size += 1;
}

/*
 * 返回：被删除的指针
 */
void *plRemove(PList *ppl, int index) {
    plRangeCheck(ppl, index);

    void *p = ppl->ppBuf[index];

    // 后面指针向前移动一个位置
    int numMoved = ppl->size - index - 1;
    if (numMoved > 0) {
        // memcpy不可用，复制过程会锁定对于内存??
        // memcpy(p->pBuf + index, p->pBuf + index + 1, numMoved);
        void **pDest = ppl->ppBuf + index;
        for (int i = 0; i < numMoved; i++) {
            *(pDest + i) =  *(pDest + i + 1);
        }
    }
    // 大小减一
    ppl->size -= 1;

    return p;
}

/*
 * 返回：原指针
 */
void *plSet(PList *ppl, int index, void *pValue) {
    plRangeCheck(ppl, index);

    void *tmp = ppl->ppBuf[index];

    ppl->ppBuf[index] = pValue;

    return tmp;
}

/*
 * 返回：首次找到指针的下标，没找到返回-1。
 */
int plIndex(PList *ppl, void *pValue) {
    for (int i = 0; i < ppl->size; i++) {
        if (ppl->ppBuf[i] == pValue)
            return i;
    }
    return -1;
}

int plSize(PList *ppl) {
    return ppl->size;
}

int plCapacity(PList *ppl) {
    return ppl->capacity;
}

int plEmpty(PList *ppl) {
    return ppl->size == 0;
}

/*
 * 打印所有元素
 */
void plPrint(PList *ppl) {
    printf("<");
    for (int i = 0; i < ppl->size; i++) {
        printf(i == 0 ? "%p" : " %p", ppl->ppBuf[i]);
    }
    printf(">\n");
}




// ------ PStack ------------------------------------------------------

typedef PList PStack;

/*
 * 返回：初始化成功则返回传参pps，失败则返回NULL。
 */
PStack *psInitDef(PStack *pps) {
    return plInitDef(pps);
}

/*
 * 返回：初始化成功则返回传参pps，失败则返回NULL。
 */
PStack *psInit(PStack *pps, int initCap) {
    return plInit(pps, initCap);
}

/*
 * 清除数据，释放内存
 */
void psDestroy(PStack *pps) {
    plDestroy(pps);
}

/*
 * 清除栈中所有元素
 */
void psClear(PStack *pps) {
    plClear(pps);
}

void *psPeek(PStack *pps) {
    return plGet(pps, pps->size - 1);
}

void *psPop(PStack *pps) {
    return plRemove(pps, pps->size - 1);
}

void psPush(PStack *pps, void *pValue) {
    plAdd(pps, pValue);
}

/*
 * 确保最小容量
 * 返回：#SUCCESS表示成功，#FAILED表示失败
 */
int psEnsureCapacity(PStack *pps, int minCapacity) {
    return plEnsureCapacity(pps, minCapacity);
}

/*
 * 返回：元素个数
 */
int psSize(PStack *pps) {
    return plSize(pps);
}

/*
 * 返回：容量大小
 */
int psCapacity(PStack *pps) {
    return plCapacity(pps);
}

/*
 * 返回：0表示空，非0表示非空
 */
int psEmpty(PStack *pps) {
    return plEmpty(pps);
}

/*
 * 打印所有元素
 */
void psPrint(PStack *pps) {
    plPrint(pps);
}


// ------ test code ------------------------------------------------------
/*
int main() {
    printf("---- test PList -----------------------------\n");
    PList plist = {0};
    PList *p = plInitDef(&plist);
    printf("initDef() capacity:%d\n", plCapacity(p));
    plDestroy(p);

    p = plInit(p, 3);
    printf("init(3) capacity:%d\n", plCapacity(p));

    int n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5, n6 = 6;
    printf("n1:%p, n2:%p, n3:%p, n4:%p, n5:%p, n6:%p\n", &n1, &n2, &n3, &n4, &n5, &n6);

    plAdd(p, &n1);
    printf("add(&n1) size:%d\n", plSize(p));

    plAdd(p, &n2);
    printf("add(n2) size:%d\n", plSize(p));

    plAdd(p, &n3);
    printf("add(&n3) size:%d\n", plSize(p));

    plAdd(p, &n4);
    printf("add(&n4) size:%d\n", plSize(p));

    plAdd(p, &n5);
    printf("add(&n5) size:%d\n", plSize(p));

    plPrint(p);

    void *pNum = plRemove(p, 1);
    printf("remove(1)=%p, size:%d\n", pNum, plSize(p));
    plPrint(p);

    pNum = plGet(p, 3);
    printf("get(3)=%p, size:%d\n", pNum, plSize(p));

    pNum = plSet(p, 0, &n6);
    printf("set(0, &n6)=%p, size:%d\n", pNum, plSize(p));
    plPrint(p);

    plInsert(p, 1, &n2);
    printf("insert(1, &n2), size:%d\n", plSize(p));
    plPrint(p);

    plEnsureCapacity(p, 20);
    printf("ensureCapacity(20) capacity:%d\n", plCapacity(p));

    plClear(p);
    printf("clear(), size=%d, empty=%d\n", plSize(p), plEmpty(p));

    plDestroy(p);
    printf("destroy() buf:%p, capacity:%d, size:%d\n", p->ppBuf, p->capacity, p->size);




    printf("\n---- test PStack -----------------------------\n");

    PStack *ps = psInitDef(p);
    printf("initDef() capacity:%d\n", psCapacity(ps));
    psDestroy(ps);

    ps = psInit(ps, 2);
    printf("init(2) capacity:%d\n", psCapacity(ps));

    printf("n1:%p, n2:%p, n3:%p, n4:%p, n5:%p, n6:%p\n", &n1, &n2, &n3, &n4, &n5, &n6);

    psPush(ps, &n1);
    printf("push(&n1) size:%d\n", psSize(ps));

    psPush(ps, &n2);
    printf("push(&n2) size:%d\n", psSize(ps));

    psPush(ps, &n3);
    printf("push(&n3) size:%d\n", psSize(ps));

    psPrint(ps);

    pNum = psPop(ps);
    printf("pop()=%p, size:%d\n", pNum, psSize(ps));
    psPrint(ps);

    printf("peek()=%p, size:%d\n", psPeek(ps), psSize(ps));

    psEnsureCapacity(ps, 20);
    printf("ensureCapacity(20) capacity:%d\n", psCapacity(ps));

    psPush(ps, &n5);
    printf("push(&n5) size:%d\n", psSize(ps));
    psPrint(ps);

    psClear(ps);
    printf("clear(), size=%d, empty=%d\n", psSize(ps), psEmpty(ps));

    psDestroy(ps);
    printf("destroy() buf:%p, capacity:%d, size:%d\n", ps->ppBuf, ps->capacity, ps->size);

    return 0;
}
*/
