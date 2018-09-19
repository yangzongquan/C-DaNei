/*
 * 单向链表。
 *
 * 可当做Stack、Queue使用。
 * 
 * 性能分析：
 *     1. 多一个指针，内存加倍，相比与数组1.5倍扩容相差不多。
 *     2. 当作Stack、Queue使用时，均头尾直接操作，无遍历，性能很好。
 */
#include <stdio.h>
#include <stdlib.h>

#include "plink.h"

/*
typedef struct PNNode {
    void *value; // 数据值
    struct PNNode *next; // 下一个节点指针
} PNNode;

typedef struct {
    PNNode *head; // 链表头
    PNNode *tail; // 链表尾
    int size;
    PNNode *iterator; // 用于遍历的指针
} PLink, PLinkStack, PLinkQueue;
*/


PNNode *plNewNode(PNNode *next, void *p) {
    PNNode *node = (PNNode *) malloc(sizeof(PNNode));
    if (!node) {
        printf("Can't alloc mem for new node.\n");
        exit(1);
    }
    node->value = p;
    node->next = next;
    return node;
}

/*
 * 初始化链表。
 * 返回：传参的链表指针。
 */
PLink *plInit(PLink *link) {
    link->head = NULL;
    link->tail = NULL;
    link->size = 0;
    link->iterator = NULL;
    return link;
}

/*
 * 删除所有元素，并释放相关内存。
 */
void plClear(PLink *l) {
    for (PNNode *n = l->head; n != NULL;) {
        PNNode *next = n->next;
        free(n);
        n = next;
    }
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    l->iterator = NULL;
}

/*
 * 销毁链表，并释放所有相关内存。
 * 除非重新初始化，否则销毁后不能使用。
 */
void plDestroy(PLink *l) {
    plClear(l);
}

/*
 * 获取第一个元素。
 */
void *plGetFirst(PLink *l) {
    if (l->size == 0) {
        printf("Empty link, can't get first elem.\n");
        exit(1);
    }

    return l->head->value;
}

/*
 * 获取最后一个元素。
 */
void *plGetLast(PLink *l) {
    if (l->size == 0) {
        printf("Empty link, can't get last elem.\n");
        exit(1);
    }

    return l->tail->value;
}

/*
 * 添加元素到队列开头
 */
void plAddFirst(PLink *l, void *p) {
    PNNode *n = plNewNode(NULL, p);
    if (l->size == 0) {
        l->head = n;
        l->tail = n;
    } else {
        n->next = l->head;
        l->head = n;
    }
    l->size++;
}

/*
 * 添加元素到队列末尾。
 */
void plAddLast(PLink *l, void *p) {
    PNNode *n = plNewNode(NULL, p);
    if (l->size == 0) {
        l->head = n;
        l->tail = n;
    } else {
        l->tail = l->tail->next = n;
    }
    l->size++;
    
}

/*
 * 添加元素到队列末尾。
 */
void plAdd(PLink *l, void *p) {
    plAddLast(l, p);
}

/*
 * 删除第一个元素。
 * 返回：被删除的元素。
 */
void *plRemoveFirst(PLink *l) {
    if (l->size == 0) {
        printf("Empty link, can't remove first elem.\n");
        exit(1);
    }

    PNNode *first = l->head;
    l->head = l->head->next;
    l->size--;

    void *value = first->value;
    free(first);

    if (l->size == 0) {
        l->tail = NULL;
    }

    return value;
}

/*
 * 元素入栈。
 */
void plStackPush(PLink *l, void *p) {
    plAddFirst(l, p);
}

/*
 * 查看栈顶元素。
 */
void *plStackPeek(PLink *l) {
    return plGetFirst(l);
}

/*
 * 弹出栈顶元素。
 */
void *plStackPop(PLink *l) {
    return plRemoveFirst(l);
}

/*
 * 将元素放入队列。
 */
void plQueuePush(PLink *l, void *p) {
    return plAddLast(l, p);
}

/*
 * 查看队列第一个元素。
 */
void *plQueuePeek(PLink *l) {
    return plGetFirst(l);
}

/*
 * 弹出队列第一个元素。
 */
void *plQueuePop(PLink *l) {
    return plRemoveFirst(l);
}

/*
 * 返回元素的个数。
 */
int plSize(PLink *l) {
    return l->size;
}

/*
 * 判断link是否为空。
 */
int plEmpty(PLink *l) {
    return l->size == 0;
}

/*
 * 判断是否包含指定元素
 */
int plContain(PLink *l, void *p) {
    PNNode *n = l->head;
    while (n != NULL) {
        if (n->value == p) return 1;
        n = n->next;
    }
    return 0;
}

/*
 * 遍历前，重置迭代器位置到-1位置。
 */
void plMoveToBeforeFirst(PLink *l) {
    l->iterator = NULL;
}

/*
 * 遍历时，确定当前迭代器位置后面是否包含元素。
 */
int plHasNext(PLink *l) {
    if (l->iterator == NULL) {
        return l->head != NULL;
    } else {
        return l->iterator->next != NULL;
    }
}

/*
 * 遍历时，取下一个元素。
 * 一定要通过plHasNext(PLink)或size确定范围，否则可能会无限循环。
 */
void *plNext(PLink *l) {
    if (l->size == 0) {
        printf("Empty link, no next elem.\n");
        exit(1);
    }

    if (l->iterator == NULL) {
        l->iterator = l->head;
    } else {
        l->iterator = l->iterator->next;
    }
    return l->iterator->value;
}

/*
 * 打印所有元素
 */
void plPrint(PLink *l) {
    if (l->size == 0) {
        printf("[]\n");
        return;
    }
    PNNode *n = l->head;
    printf("[");
    while (n != NULL) {
        printf(n == l->head ? "%p" : " %p", n->value);
        n = n->next;
    }
    printf("]\n");
}


PLink *plInit(PLink *link);
void plDestroy(PLink *l);
void plClear(PLink *l);
void *plGetFirst(PLink *l);
void *plGetLast(PLink *l);
void plAddFirst(PLink *l, void *p);
void plAddLast(PLink *l, void *p);
void plAdd(PLink *l, void *p);
void *plRemoveFirst(PLink *l);
void plStackPush(PLink *l, void *p);
void *plStackPeek(PLink *l);
void *plStackPop(PLink *l);
void plQueuePush(PLink *l, void *p);
void *plQueuePeek(PLink *l);
void *plQueuePop(PLink *l);
int plSize(PLink *l);
int plEmpty(PLink *l);
int plContain(PLink *l, void *p);
void plMoveToBeforeFirst(PLink *l);
int plHasNext(PLink *l);
void *plNext(PLink *l);
void plPrint(PLink *l);

void test() {
    PLink plink = {0};
    PLink *l = plInit(&plink);

    char s[6]={0};
    printf("%p, %p, %p, %p, %p, %p\n", s, s+1, s+2, s+3, s+4, s+5);
    plAdd(l, s);
    printf("plAdd(l, s) size=%d, empty=%d\n", plSize(l), plEmpty(l));
    plAddLast(l, s+1);
    printf("plAddLast(l, s+1) contain(l, s)=%d\n", plContain(l, s));
    plAddFirst(l, s+2);
    printf("plAddFirst(l, s+2) contain(l, s+5)=%d\n", plContain(l, s+5));

    plMoveToBeforeFirst(l);
    while (plHasNext(l)) {
        printf("foreach:%p\n", plNext(l));
    }
    printf("plGetFirst(l)=%p, plGetLast(l)=%p\n", plGetFirst(l), plGetLast(l));
    void *p = plRemoveFirst(l);
    printf("plRemoveFirst(l)=%p\n", p);
    plPrint(l);

    plClear(l);
    printf("plClear(l) size=%d, empty=%d, contain(s+1)=%d, print:", plSize(l), plEmpty(l), plContain(l, s+1));
    plPrint(l);
    
    plDestroy(l);
    printf("plDestroy(l)\n\n\n");

    // ----------- Stack and Queue --------------
    plInit(l);
    printf("plInit(l)\n");

    plStackPush(l, s);
    printf("plStackPush(l, s)\n");
    plStackPush(l, s+1);
    printf("plStackPush(l, s+1)\n");
    plStackPush(l, s+2);
    printf("plStackPush(l, s+2)\n");
    plPrint(l);

    printf("plStackPeek(l)=%p\n", plStackPeek(l));
    printf("plStackPop(l)=%p\n", plStackPop(l));
    plPrint(l);

    plClear(l);
    printf("plClear(l)\n");

    plQueuePush(l, s);
    printf("plQueuePush(l, s)\n");
    plQueuePush(l, s+1);
    printf("plQueuePush(l, s+1)\n");
    plQueuePush(l, s+2);
    printf("plQueuePush(l, s+2)\n");

    printf("plQueuePeek(l)=%p\n", plQueuePeek(l));
    printf("plQueuePop(l)=%p\n", plQueuePop(l));
    plPrint(l);

    plDestroy(l);
    printf("plDestroy(l)\n");

}

/*
int main() {
    test();
    return 0;
}
*/

