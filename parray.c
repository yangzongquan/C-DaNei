/*
 * 采用数组实现的List<void *>、Stack<void *>、Queue<void *>数据结构。
 *
 * 自动扩容：
 *     当容量不够用时，自动扩充容量。扩充容量过程中需要重新分配
 *     内存，并复制原有数据。所以，在初始化或者使用过程中尽量明
 *     确容量。
 *
 * 使用前需要初始化分配内存，使用完需要调用销毁函数释放内存。
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parray.h"

#define         DEFAULT_CAPACITY      10

/*
#define       PA_SUCCESS      1
#define       PA_FAILED       0

typedef struct {
    void **pBuf;
    int capacity;
    int size;
} PArray;
*/

// 出错直接退出
void paErr(char *str) {
    printf("%s\n", str);
    exit(1);
}

/*
 * 返回：初始化成功则返回传参a，失败则返回NULL。
 */
PArray *paInitDef(PArray *a) {
    return paInit(a, DEFAULT_CAPACITY);
}

/*
 * 返回：初始化成功则返回传参a，失败则返回NULL。
 */
PArray *paInit(PArray *a, int initCap) {
    if (a->pBuf) {
        // 已经初始化过
        paErr("Multi init");
    }
    if (initCap <= 0) {
        printf("Illegal arg initCap:%d\n", initCap);
        paErr("");
    }
    // 分配内存
    a->pBuf = (void **)malloc(initCap * sizeof(void *));
    // 分配失败
    if (!a->pBuf) return NULL;
    a->capacity = initCap;
    a->size = 0;
    
    return a;
}

/*
 * 清除元素，释放内存。
 */
void paDestroy(PArray *a) {
    free(a->pBuf);
    a->pBuf = NULL;
    a->capacity = 0;
    a->size = 0;
}

/*
 * 清除所有元素。
 */
void paClear(PArray *a) {
    a->size = 0;
}

// 检查下标是否超出范围
void paRangeCheck(PArray *a, int index) {
    if (index < 0 || index >= a->size) {
        printf("paRangeCheck(), out of bounds:%d\n", index);
        paErr("");
    }
}

/*
 * 获取指定位置的元素。
 */
void *paGet(PArray *a, int index) {
    paRangeCheck(a, index);
    return a->pBuf[index];
}

int paGrow(PArray *a, int minCapacity) {
    // 扩容1.5倍
    int newCapacity = a->capacity + (a->capacity >> 1); 
    // 至少满足要求的容量
    newCapacity = newCapacity < minCapacity ? minCapacity : newCapacity;

    void **ppNew = (void **) malloc(newCapacity * sizeof(void *));
    if (ppNew) {
        memcpy(ppNew, a->pBuf, sizeof(void *) * a->size);
        free(a->pBuf);
        a->pBuf = ppNew;
        a->capacity = newCapacity;
        return PA_SUCCESS;
    } else {
        return PA_FAILED;
    }   
}

/*
 * 返回：#PA_SUCCESS表示成功，#PA_FAILED表示失败
 */
int paEnsureCapacity(PArray *a, int minCapacity) {
    if (a->capacity >= minCapacity) {
        // 现有容量已经满足
        return PA_SUCCESS;
    }   
    // 至少扩容到DEFAULT_CAPACITY
    minCapacity = minCapacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : minCapacity;

    return paGrow(a, minCapacity);
}

/*
 * 返回：添加元素的下标。
 */
int paAdd(PArray *a, void *p) {
    if(!paEnsureCapacity(a, a->size + 1)) {
        paErr("Failed to alloc mem, can't add elem");
    }
    a->pBuf[a->size] = p;
    a->size += 1;
    return a->size - 1;
}

/*
 * 添加元素到index下标处，index范围：index >=0 && index <= size。
 */
void paInsert(PArray *a, void *p, int index) {
        // index等于size位置合法
    if (index > a->size || index < 0) {
        printf("paInsert(), out of bounds:%d\n", index);
        paErr("");
    }
    if (!paEnsureCapacity(a, a->size + 1)) {
        paErr("Failed to alloc mem, can't add elem");
    }
    // 把index位置的元素及后面元素都往后移动一个位置
    int numMoved = a->size - index;
    if (numMoved > 0) {
        // memcpy不可用，复制过程会锁定对于内存??
        for (int i = a->size; i > index; i--) {
            a->pBuf[i] = a->pBuf[i - 1];
        }
    }
    a->pBuf[index] = p;
    a->size += 1;
}

/*
 * 返回：被删除的元素。
 */
void *paRemove(PArray *a, int index) {
    paRangeCheck(a, index);

    void *p = a->pBuf[index];

    // 后面元素向前移动一个位置
    int numMoved = a->size - index - 1;
    if (numMoved > 0) {
        // memcpy不可用，复制过程会锁定对于内存??
        // memcpy(p->pBuf + index, p->pBuf + index + 1, numMoved);
        void **pDest = a->pBuf + index;
        for (int i = 0; i < numMoved; i++) {
            *(pDest + i) =  *(pDest + i + 1);
        }
    }
    // 大小减一
    a->size -= 1;

    return p;
}

/*
 * 首次找到指定元素，并删除。
 * 返回：找到并删除则返回被删除元素的位置，未找到则返回-1。
 */
int paRemoveElem(PArray *a, void *p) {
    int index = paIndexOf(a, p);
    if (index >= 0) {
        paRemove(a, index);
    }
    return index;
}

/*
 * 返回：指定位置原来元素的值。
 */
void *paSet(PArray *a, int index, void *p) {
    paRangeCheck(a, index);

    void *tmp = a->pBuf[index];

    a->pBuf[index] = p;

    return tmp;
}

/*
 * 返回：首次找到指定元素的下标，没找到返回-1。
 */
int paIndexOf(PArray *a, void *p) {
    for (int i = 0; i < a->size; i++) {
        if (a->pBuf[i] == p)
            return i;
    }
    return -1;
}

/*
 * 判断是否包含某元素。
 */
int paContain(PArray *a, void *p) {
    return paIndexOf(a, p) != -1;
}

/*
 * 返回元素个数。
 */
int paSize(PArray *a) {
    return a->size;
}

/*
 * 返回数组容量。
 */
int paCapacity(PArray *a) {
    return a->capacity;
}

/*
 * 判断是否为空。
 */
int paEmpty(PArray *a) {
    return a->size == 0;
}

/*
 * 打印所有元素
 */
void paPrint(PArray *a) {
    printf("<");
    for (int i = 0; i < a->size; i++) {
        printf(i == 0 ? "%p" : " %p", a->pBuf[i]);
    }
    printf(">\n");
}

/*
 * 查看栈顶元素。
 */
void *paStackPeek(PArray *a) {
    return paGet(a, a->size - 1);
}

/*
 * 弹出栈顶元素。
 */
void *paStackPop(PArray *a) {
    return paRemove(a, a->size - 1);
}

/*
 * 元素入栈。
 */
void paStackPush(PArray *a, void *p) {
    paAdd(a, p);
}

/*
 * 查看队列第一个元素。
 */
void *paQueuePeek(PArray *a) {
    return paGet(a, 0);
}

/*
 * 弹出队列第一个元素。
 */
void *paQueuePop(PArray *a) {
    return paRemove(a, 0);
}

/*
 * 将元素放入队列末尾。
 */
void paQueuePush(PArray *a, void *p) {
    paAdd(a, p);
}




// ------ test code ------------------------------------------------------

void test() {
    printf("---- test PArray -----------------------------\n");
    PArray array = {0};
    PArray *p = paInitDef(&array);
    printf("initDef() capacity:%d\n", paCapacity(p));
    paDestroy(p);

    p = paInit(p, 3);
    printf("init(3) capacity:%d\n", paCapacity(p));

    int n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5, n6 = 6;
    printf("n1:%p, n2:%p, n3:%p, n4:%p, n5:%p, n6:%p\n", &n1, &n2, &n3, &n4, &n5, &n6);

    paAdd(p, &n1);
    printf("add(&n1) size:%d\n", paSize(p));

    paAdd(p, &n2);
    printf("add(&n2) size:%d\n", paSize(p));

    paAdd(p, &n3);
    printf("add(&n3) size:%d\n", paSize(p));

    paAdd(p, &n4);
    printf("add(&n4) size:%d\n", paSize(p));

    paAdd(p, &n5);
    printf("add(&n5) size:%d\n", paSize(p));

    paPrint(p);

    void *pNum = paRemove(p, 1);
    printf("remove(1)=%p, size:%d\n", pNum, paSize(p));
    paPrint(p);

    pNum = paGet(p, 3);
    printf("get(3)=%p, size:%d, contain(&n3)=%d\n", pNum, paSize(p), paContain(p, &n3));

    pNum = paSet(p, 0, &n6);
    printf("set(0, &n6)=%p, size:%d\n", pNum, paSize(p));
    paPrint(p);

    paInsert(p, &n2, 1);
    printf("insert(&n2, 1), size:%d\n", paSize(p));

    printf("paRemoveElem(&n4)=%d\n", paRemoveElem(p, &n4));
    paPrint(p);

    paEnsureCapacity(p, 20);
    printf("ensureCapacity(20) capacity:%d\n", paCapacity(p));

    paClear(p);
    printf("clear(), size=%d, empty=%d, contain(&n1)=%d\n", paSize(p), paEmpty(p), paContain(p, &n1));

    paDestroy(p);
    printf("destroy() buf:%p, capacity:%d, size:%d\n", p->pBuf, p->capacity, p->size);




    printf("\n---- test Stack and Queue -----------------------------\n");

    PArray *pa = paInitDef(p);
    printf("initDef() capacity:%d\n", paCapacity(pa));
    paDestroy(pa);

    pa = paInit(pa, 2);
    printf("init(2) capacity:%d\n", paCapacity(pa));

    printf("n1:%p, n2:%p, n3:%p, n4:%p, n5:%p, n6:%p\n", &n1, &n2, &n3, &n4, &n5, &n6);

    paStackPush(pa, &n1);
    printf("stackpush(&n1) size:%d\n", paSize(pa));

    paStackPush(pa, &n2);
    printf("stackpush(&n2) size:%d\n", paSize(pa));

    paStackPush(pa, &n3);
    printf("stackpush(&n3) size:%d\n", paSize(pa));

    paPrint(pa);

    pNum = paStackPop(pa);
    printf("stackpop()=%p, size:%d\n", pNum, paSize(pa));
    paPrint(pa);

    printf("stackpeek()=%p, size:%d\n", paStackPeek(pa), paSize(pa));

    paEnsureCapacity(pa, 20);
    printf("ensureCapacity(20) capacity:%d\n", paCapacity(pa));

    paStackPush(pa, &n5);
    printf("stackpush(&n5) size:%d\n", paSize(pa));
    paPrint(pa);

    pNum = paQueuePeek(pa);
    printf("queuePeek(pa)=%p, size=%d\n", pNum, paSize(pa));

    pNum = paQueuePop(pa);
    printf("queuePop(pa)=%p, size=%d\n", pNum, paSize(pa));

    paQueuePush(pa, &n6);
    printf("queuePush(&n6), size=%d\n", paSize(pa));
    paPrint(pa);

    paClear(pa);
    printf("clear(), size=%d, empty=%d\n", paSize(pa), paEmpty(pa));

    paDestroy(pa);
    printf("destroy() buf:%p, capacity:%d, size:%d\n", pa->pBuf, pa->capacity, pa->size);
}

/*
int main() {
    test();
    return 0;
}
*/
