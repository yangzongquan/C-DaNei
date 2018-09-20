/*
 * 采用数组实现的List<void *>、Stack<void *>、Queue<void *>数据结构。
 *
 * 使用前需要初始化分配内存，使用完需要调用销毁函数释放内存。
 *
 * 自动扩容：
 *     当容量不够用时，自动扩充容量。扩充容量过程中需要重新分配
 *     内存，并复制原有数据。所以，在初始化或者使用过程中尽量明
 *     确容量。
 * 
 * 当作List使用时：
 *      适合定长、随机访问多、删除插入少时使用。
 *      LinkList更适合变长、随机访问少、删除插入多的操作。
 *
 * 当作Stack使用时：
 *      一般定长、省内存时使用，其他情况使用LinkStack。
 *
 * 当做Queue使用时:
 *      一般定长、省内存时使用，其他情况使用LinkQueue。
 *      （后面单独实现：数组+循环队列+自动扩容）
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
} PArray, PArrayList;
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
        paErr("Multi init\n");
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
    a->size++;
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
    a->size++;
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
    a->size--;

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

//-------- Stack ----------------------------

typedef PArray PArrayStack;

/*
 * 采用默认容量初始化栈。
 */
PArrayStack *paStkInitDef(PArrayStack *s) {
    return paInitDef(s);
}

/*
 * 根据指定的容量初始化栈。
 */
PArrayStack *paStkInit(PArrayStack *s, int initCap) {
    return paInit(s, initCap);
}

/*
 * 清除元素，释放内存。
 */
void paStkDestroy(PArrayStack *s) {
    paDestroy(s);
}

/*
 * 清除所有元素。
 */
void paStkClear(PArrayStack *s) {
    paClear(s);
}

/*
 * 返回栈中元素个数。
 */
int paStkSize(PArrayStack *s) {
    return paSize(s);
}

/*
 * 判断栈是否为空。
 */
int paStkEmpty(PArrayStack *s) {
    return paEmpty(s);
}

/*
 * 获取栈容量。
 */
int paStkCapacity(PArrayStack *s) {
    return paCapacity(s);
}

/*
 * 确保minCapacity容量。
 *
 * 返回：#PA_SUCCESS表示成功，#PA_FAILED表示失败
 */
int paStkEnsureCapacity(PArrayStack *s, int minCapacity) {
    return paEnsureCapacity(s, minCapacity);
}

/*
 * 查看栈顶元素。
 */
void *paStkPeek(PArrayStack *s) {
    return paGet(s, s->size - 1);
}

/*
 * 弹出栈顶元素。
 */
void *paStkPop(PArrayStack *s) {
    return paRemove(s, s->size - 1);
}

/*
 * 元素入栈。
 */
void paStkPush(PArrayStack *s, void *p) {
    paAdd(s, p);
}

/*
 * 用于栈遍历。
 */
void *paStkGet(PArrayStack *s, int index) {
    return paGet(s, index);
}

/*
 * 判断栈是否包含指定元素。
 */
int paStkContain(PArrayStack *s, void *p) {
    return paContain(s, p);
}

/*
 * 打印栈。
 */
void paStkPrint(PArrayStack *s) {
    paPrint(s);
}




//-------- Queue ----------------------------

/*
typedef struct {
    void **pBuf;
    int capacity;
    int size;
    int head;
    int tail;
} PArrayQueue;
*/

/*
 * 采用默认容量初始化队列。
 */
PArrayQueue *paQueInitDef(PArrayQueue *q) {
    return paQueInit(q, DEFAULT_CAPACITY);
}

/*
 * 根据指定的容量初始化队列。
 */
PArrayQueue *paQueInit(PArrayQueue *q, int initCap) {
    if (q->pBuf) {
        // 已经初始化过
        paErr("Multi init");
    }
    if (initCap <= 0) {
        printf("Illegal arg initCap:%d\n", initCap);
        exit(1);
    }
    // 分配内存
    q->pBuf = (void **)malloc(initCap * sizeof(void *));
    // 分配失败
    if (!q->pBuf) return NULL;
    q->capacity = initCap;
    q->size = 0;
    q->head = 0;
    q->tail = 0;

    return q;
}

/*
 * 清除元素，释放内存。
 */
void paQueDestroy(PArrayQueue *q) {
    free(q->pBuf);
    q->pBuf = NULL;
    q->capacity = 0;
    q->size = 0;
    q->head = 0;
    q->tail = 0;
}

/*
 * 清除所有元素。
 */
void paQueClear(PArrayQueue *q) {
    q->size = 0;
    q->head = 0;
    q->tail = 0;
}

/*
 * 返回队列中元素个数。
 */
int paQueSize(PArrayQueue *q) {
    return q->size;
}

/*
 * 判断队列是否为空。
 */
int paQueEmpty(PArrayQueue *q) {
    return q->size == 0;
}

/*
 * 获取队列容量。
 */
int paQueCapacity(PArrayQueue *q) {
    return q->capacity;
}

/*
 * 查看队列第一个元素。
 */
void *paQuePeek(PArrayQueue *q) {
    if (q->size == 0) {
        printf("Empty queue, can't peek first elem.\n");
        exit(1);
    }
    return q->pBuf[q->head];
}

/*
 * 弹出队列第一个元素。
 */
void *paQuePop(PArrayQueue *q) {
    if (q->size == 0) {
        printf("Empty queue, can't pop first elem.\n");
        exit(1);
    }
    void *value = q->pBuf[q->head];
    // head位置后移一位
    q->head++;
    // 当head超出数组末尾，回到数组开头
    q->head %= q->capacity;
    
    q->size--;
    return value;
}

int paQueGrow(PArrayQueue *q, int minCapacity) {
    // 扩容1.5倍
    int newCapacity = q->capacity + (q->capacity >> 1); 
    // 至少满足要求的容量
    newCapacity = newCapacity < minCapacity ? minCapacity : newCapacity;

    void **ppNew = (void **) malloc(newCapacity * sizeof(void *));
    if (ppNew) {
        // 复制队列中元素。
        for (int i = 0, oldIndex; i < q->size; i++) {
            oldIndex = (q->head + i) % q->capacity;
            ppNew[i] = q->pBuf[oldIndex];
        }
        free(q->pBuf);
        q->pBuf = ppNew;
        q->capacity = newCapacity;
        q->head = 0;
        // 重置队列头尾
        q->tail = q->size;
        return PA_SUCCESS;
    } else {
        return PA_FAILED;
    }   
}

/*
 * 确保minCapacity容量。
 *
 * 返回：#PA_SUCCESS表示成功，#PA_FAILED表示失败
 */
int paQueEnsureCapacity(PArrayQueue *q, int minCapacity) {
    if (q->capacity >= minCapacity) {
        // 现有容量已经满足
        return PA_SUCCESS;
    }   
    // 至少扩容到DEFAULT_CAPACITY
    minCapacity = minCapacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : minCapacity;

    return paQueGrow(q, minCapacity);
}

/*
 * 将元素放入队列末尾。
 */
void paQuePush(PArrayQueue *q, void *p) {
    paQueEnsureCapacity(q, q->size + 1);

    q->pBuf[q->tail] = p;
    // tail位置后移一位
    q->tail++;
    // 当tail超出数组末尾，回到数组开头
    q->tail %= q->capacity;

    q->size++;
}

/*
 * 用于队列遍历。
 */
void *paQueGet(PArrayQueue *q, int index) {
    if (q->size == 0) {
        printf("Empty queue, can't get specific position elem.\n");
        exit(1);
    }
    index = (q->head + index) % q->capacity;
    return q->pBuf[index];
}

/*
 * 判断队列是否包含指定元素。
 */
int paQueContain(PArrayQueue *q, void *p) {
    for (int i = 0, index; i < q->size; i++) {
        index = (q->head + i) % q->capacity;
        if (q->pBuf[index] == p) {
            return 1;
        }
    }
    return 0;
}

/*
 * 打印队列。
 */
void paQuePrint(PArrayQueue *q) {
    printf("[");
    for (int i = 0, index; i < q->size; i++) {
        index = (q->head + i) % q->capacity;
        printf(i == 0 ? "%p" : " %p", q->pBuf[index]);
    }
    printf("]\n");
}


// ------ test code ------------------------------------------------------

void paTest() {
    printf("---- test PArray -----------------------------\n");
    PArray array = {0};
    PArray *p = paInitDef(&array);
    printf("initDef() capacity:%d\n", paCapacity(p));
    paDestroy(p);

    p = paInit(p, 3);
    printf("init(3) capacity:%d\n", paCapacity(p));

    char n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5, n6 = 6;
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




    printf("\n---- test Stack -----------------------------\n");

    PArrayStack *s = paStkInitDef(p);
    printf("initDef() capacity:%d\n", paStkCapacity(s));
    paStkDestroy(s);

    s = paStkInit(s, 2);
    printf("init(2) capacity:%d\n", paStkCapacity(s));

    printf("n1:%p, n2:%p, n3:%p, n4:%p, n5:%p, n6:%p\n", &n1, &n2, &n3, &n4, &n5, &n6);

    paStkPush(s, &n1);
    printf("stackpush(&n1) size:%d\n", paStkSize(s));

    paStkPush(s, &n2);
    printf("stackpush(&n2) size:%d\n", paStkSize(s));

    paStkPush(s, &n3);
    printf("stackpush(&n3) size:%d\n", paStkSize(s));

    paStkPrint(s);

    pNum = paStkPop(s);
    printf("stackpop()=%p, size:%d\n", pNum, paStkSize(s));
    paStkPrint(s);

    printf("stackpeek()=%p, size:%d\n", paStkPeek(s), paStkSize(s));

    paStkEnsureCapacity(s, 20);
    printf("ensureCapacity(20) capacity:%d\n", paStkCapacity(s));

    paStkPush(s, &n5);
    printf("stackpush(&n5) size:%d\n", paStkSize(s));
    paStkPrint(s);



    printf("\n---- test Queue -----------------------------\n");

    PArrayQueue queue = {0};
    PArrayQueue *q = paQueInit(&queue, 3);
    printf("n1:%p, n2:%p, n3:%p, n4:%p, n5:%p, n6:%p\n", &n1, &n2, &n3, &n4, &n5, &n6);

    paQuePush(q, &n1);
    paQuePush(q, &n2);
    paQuePush(q, &n3);
    printf("paQuePush(&n1 &n2 &n3)\n");
    printf("size:%d, cap:%d, head:%d, tail:%d\n", q->size, q->capacity, q->head, q->tail);
    
    pNum = paQuePeek(q);
    printf("paQuePeek(q)=%p, size=%d\n", pNum, paQueSize(q));

    pNum = paQuePop(q);
    printf("paQuePop(q)=%p, empty=%d\n", pNum, paQueEmpty(q));
    printf("size:%d, cap:%d, head:%d, tail:%d\n", q->size, q->capacity, q->head, q->tail);

    paQuePush(q, &n4);
    printf("paQuePush(&n4), size=%d, cap=%d\n", paQueSize(q), paQueCapacity(q));
    printf("size:%d, cap:%d, head:%d, tail:%d\n", q->size, q->capacity, q->head, q->tail);
    printf("paQueGet(0)=%p, contain(&n2)=%d\n", paQueGet(q, 0), paQueContain(q, &n2));
    paQuePrint(q);

    paQuePush(q, &n5);
    printf("paQuePush(&n5), size=%d, cap=%d\n", paQueSize(q), paQueCapacity(q));
    printf("size:%d, cap:%d, head:%d, tail:%d\n", q->size, q->capacity, q->head, q->tail);
    paQuePrint(q);

    paQueEnsureCapacity(q, 20);
    printf("paQueEnsureCapacity(q,20) cap:%d\n", paQueCapacity(q));

    paQueClear(q);
    printf("clear(), size=%d, empty=%d, contain(&n2)=%d\n", paQueSize(q), paQueEmpty(q), paQueContain(q, &n2));

    paQueDestroy(q);
    printf("destroy() buf:%p, capacity:%d, size:%d\n", q->pBuf, q->capacity, q->size);

}

/*
int main() {
    paTest();
    return 0;
}
*/
