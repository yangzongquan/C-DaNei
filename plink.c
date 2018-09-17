/*
 * 双向环链表。
 * head是一个无值节点，不存放数据，head->next指向链表头，head->prev指向链表尾。
 *     head->next -- Node1 -- Node2 -- ... -- NodeN -- prev<-head
 *
 * 可当做List、Stack、Queue使用。
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plink.h"

/*
typedef struct PNode {
    void *value; // 数据值
    struct PNode *prev; // 上一个节点指针
    struct PNode *next; // 下一个节点指针
} PNode;

typedef struct {
    PNode *head; // 链表头/尾
    int size;
    PNode *iterator; // 用于遍历的指针
} PLink;
*/

PNode *plNewNode(PNode *prev, PNode *next, void *pValue) {
    PNode *p = (PNode *) malloc(sizeof(PNode));
    p->value = pValue;
    p->prev = prev;
    p->next = next;
    return p;
}

/*
 * 初始化链表。
 * 返回：传参的链表指针。
 */
PLink *plInit(PLink *link) {
    // 初始化头，前后指针都指向自己
    PNode *h = plNewNode(NULL, NULL, NULL);
    h->prev = h;
    h->next = h;

    link->head = h;
    link->size = 0;
    link->iterator = h;

    return link;
}

/*
 * 销毁链表，并释放所有相关内存。
 * 除非重新初始化，否则销毁后不能使用。
 */
void plDestroy(PLink *l) {
    PNode *n = l->head, *tmp = NULL;
    for (int i = 0; i < l->size; i++) {
        // 释放每个元素
        n = n->next;
        tmp = n->next;
        free(n);
        n = tmp;
    }
    // 释放head
    free(l->head);
    l->head = NULL;
    l->size = 0;
    l->iterator = NULL;
}

/*
 * 删除所有元素，并释放相关内存。
 */
void plClear(PLink *l) {
    PNode *n = l->head, *tmp = NULL;
    for (int i = 0; i < l->size; i++) {
        // 释放每个元素
        n = n->next;
        tmp = n->next;
        free(n);
        n = tmp;
    }
    l->head->prev = l->head;
    l->head->next = l->head;
    l->size = 0;
    l->iterator = l->head;
}

void rangeCheck(PLink *l, int index) {
    if (index < 0 || index >= l->size) {
        printf("Out of bounds, index:%d\n", index);
        exit(1);
    }
}

PNode *plInternalGet(PLink *l, int index) {
    rangeCheck(l, index);

    PNode *n = l->head;
    if (index < (l->size >> 1)) {
        // 离头近，从头开始找
        for (int i = 0; i <= index; i++) {
            n = n->next;
        }
    } else {
        // 离尾近，从尾开始找
        for (int i = l->size; i > index; i--) {
            n = n->prev;
        }
    }

    return n;
}

/*
 * 获取index指定位置的元素。
 */
void *plGet(PLink *l, int index) {
    return plInternalGet(l, index)->value;
}

/*
 * 获取第一个元素。
 */
void *plGetFirst(PLink *l) {
    if (l->size == 0) {
        printf("Empty link, do not get the first element.\n");
        exit(1);
    }
    return l->head->next->value;
}

/*
 * 获取最后一个元素。
 */
void *plGetLast(PLink *l) {
    if (l->size == 0) {
        printf("Empty link, do not get the last element.\n");
        exit(1);
    }
    return l->head->prev->value;
}

void plAddBefore(PLink *l, void *pValue, PNode *node) {
    PNode *newNode = plNewNode(node->prev, node, pValue);
    newNode->prev->next = newNode;
    newNode->next->prev = newNode;
    l->size += 1;
}

/*
 * 添加元素到队列开头
 */
void plAddFirst(PLink *l, void *pValue) {
    plAddBefore(l, pValue, l->head->next);
}

/*
 * 添加元素到队列末尾。
 */
void plAddLast(PLink *l, void *pValue) {
    plAddBefore(l, pValue, l->head);
}

/*
 * 添加元素到队列末尾。
 */
void plAdd(PLink *l, void *pValue) {
    plAddLast(l, pValue);
}

/*
 * 添加元素到index下标处
 */
void plInsert(PLink *l, void *pValue, int index) {
    plAddBefore(l, pValue, (index == l->size ? l->head : plGet(l, index)));
}

/*
 * 查找元素第一次出现的位置。
 * 返回：找到返回位置，没找到返回-1。
 */
int plIndexOf(PLink *l, void *pValue) {
    PNode *n = l->head;
    for (int i = 0; i < l->size; i++) {
        n = n->next;
        if (n->value == pValue) {
            return i;
        }
    }
    return -1;
}

/*
 * 删除index位置元素。
 * 返回：被删除的元素。
 */
void *plRemove(PLink *l, int index) {
    // 无需检查index越界，plInternalGet会检查。
    PNode *n = plInternalGet(l, index);
    n->prev->next = n->next;
    n->next->prev = n->prev;

    void *value = n->value;
    free(n);
    l->size -= 1;

    return value;
}

/*
 * 删除第一个元素。
 * 返回：被删除的元素。
 */
void *plRemoveFirst(PLink *l) {
    return plRemove(l, 0);
}

/*
 * 删除末尾元素。
 * 返回：被删除的元素。
 */
void *plRemoveLast(PLink *l) {
    return plRemove(l, l->size - 1);
}

/*
 * 删除第一个值为p的元素。
 * 返回：找到并删除则返回被删除元素的位置，未找到则返回-1。
 */
int plRemoveElem(PLink *l, void *p) {
    int index = plIndexOf(l, p);
    if (index >= 0) {
        plRemove(l, index);
    }
    return index;
}

/*
 * 返回：被替换的元素值。
 */
void *plSet(PLink *l, int index, void *p) {
    PNode *n = plInternalGet(l, index);
    void *old = n->value;
    n->value = p;
    return old;
}

/*
 * 元素入栈。
 */
void plStackPush(PLink *l, void *p) {
    plAddLast(l, p);
}

/*
 * 查看栈顶元素。
 */
void *plStackPeek(PLink *l) {
    return plGetLast(l);
}

/*
 * 弹出栈顶元素。
 */
void *plStackPop(PLink *l) {
    return plRemoveLast(l);
}

/*
 * 将元素放入队列。
 */
void plQueuePush(PLink *l, void *p) {
    plAddLast(l, p);
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
    return plIndexOf(l, p) != -1;
}

/*
 * 遍历前，重置迭代器位置到-1位置。
 */
void plResetIterator(PLink *l) {
    l->iterator = l->head;
}

/*
 * 遍历时，确定当前迭代器位置后面是否包含元素。
 */
int plHasNext(PLink *l) {
    PNode *next = l->iterator->next;
    return next != l->head;
}

/*
 * 遍历时，取下一个元素。
 */
void *plNext(PLink *l) {
    PNode *next = l->iterator->next;
    if (next == l->head) {
        printf("No next element\n");
        exit(1);
    }
    l->iterator = l->iterator->next;
    return l->iterator->value;
}

/*
 * 打印所有元素
 */
void plPrint(PLink *l) {
    printf("[");
    PNode *n = l->head;
    for (int i = 0; i < l->size; i++) {
        n = n->next;
        printf(i == 0 ? "%p" : ", %p", n->value);
    }
    printf("]\n");
}






/************** test code ***************************************/


void test() {
    PLink link;
    PLink *l = plInit(&link);
    char s[6] = {0};
    printf("char[6]:%p,%p,%p,%p,%p,%p\n", s, s+1, s+2, s+3, s+4, s+5);
    plAddFirst(l, s);
    printf("plAddFirst(l, s);\n");
    plPrint(l);

    plAdd(l, s+1);
    printf("plAdd(l, s+1);\n");
    plPrint(l);

    plAddLast(l, s+2);
    printf("plAddLast(l, s+2);\n");
    plPrint(l);

    printf("plGet(l, 2)=%p\n", plGet(l, 2));

    printf("plGetFirst(l)=%p\n", plGetFirst(l));
    
    printf("plGetLast(l)=%p\n", plGetLast(l));

    plInsert(l, s+3, 3);
    printf("plInsert(l, s+3, 3)\n");

    plResetIterator(l);
    printf("iterate:");
    while (plHasNext(l)) {
        printf("%p ", plNext(l));
    }
    printf("\n");

    printf("plIndexOf(l, s)=%d\n", plIndexOf(l, s));

    printf("plSize(l)=%d\n", plSize(l));

    printf("plRemove(l, 0)=%p, plSize(l)=%d\n", plRemove(l, 0), plSize(l));
    plPrint(l);

    printf("plRemoveFirst(l)=%p, plSize(l)=%d\n", plRemoveFirst(l), plSize(l));
    plPrint(l);
    
    printf("plSet(l, 0, s+4)=%p, plSize(l)=%d\n", plSet(l, 0, s+4), plSize(l));
    plPrint(l);

    printf("plRemoveLast(l)=%p, plEmpty(l)=%d\n", plRemoveLast(l), plEmpty(l));
    plPrint(l);
    
    printf("plContain(l, s+4)=%d\n", plContain(l, s+4));
    printf("plRemoveElem(l, s+4)=%d\n", plRemoveElem(l, s+4));
    plPrint(l);

    printf("plEmpty(l)=%d, plSize(l)=%d, plContain(s)=%d\n\n", plEmpty(l), plSize(l), plContain(l, s));

    plDestroy(l);
    l = plInit(l);
    printf("destroy(l),init(l):");
    plPrint(l);

    plStackPush(l, s);
    printf("plStackPush(l, s)\n");
    
    plStackPush(l, s+1);
    printf("plStackPush(l, s+1)\n");
    plPrint(l);

    printf("plStackPeek(l)=%p\n", plStackPeek(l));
    
    printf("plStackPop(l)=%p\n", plStackPop(l));
    plPrint(l);
    
    plClear(l);
    printf("plClear(l)\n\n");

    plQueuePush(l, s);
    printf("plQueuePush(l, s)\n");
    
    printf("plQueuePeek(l)=%p\n", plQueuePeek(l));

    plQueuePush(l, s + 1);
    printf("plQueuePush(l, s+1)\n");
    plPrint(l);

    printf("plQueuePop(l)=%p\n", plQueuePop(l));

    plDestroy(l);
    printf("plDestroy(l)\n");
}

/*
int main() {
    test();
    return 0;
}
*/
