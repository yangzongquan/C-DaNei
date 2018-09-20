
/*
 * 双向环链表。
 * head是一个无值节点，不存放数据，head->next指向链表头，head->prev指向链表尾。
 *     head->next -- Node1 -- Node2 -- ... -- NodeN -- prev<-head
 *
 * 可当做List、Stack、Queue使用。
 * 使用前需要初始化分配内存，使用完需要调用销毁函数释放内存。
 *
 * 当作List使用时：
 *      适合变长、随机访问少、删除插入多时使用。
 *      ArrayList适合定长、随机访问多、删除插入少时使用。
 *
 * 当作Stack使用时：
 *      一般选择单链表或数组实现的栈：PLinkStack、PArrayStack。
 *
 * 当做Queue使用时:
 *      一般选择单链表或数组实现的队列：PLinkQueue、PArrayQueue。
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pllink.h"

/*
typedef struct PPNNode {
    void *value; // 数据值
    struct PPNNode *prev; // 上一个节点指针
    struct PPNNode *next; // 下一个节点指针
} PPNNode;

typedef struct {
    PPNNode *head; // 链表头/尾
    int size;
    PPNNode *iterator; // 用于遍历的指针
} PLLink, PLLinkList, PLLinkStack, PLLinkQueue;
*/

PPNNode *pllNewNode(PPNNode *prev, PPNNode *next, void *p) {
    PPNNode *n = (PPNNode *) malloc(sizeof(PPNNode));
    if (!n) {
        printf("Can't alloc mem for new node.\n");
        exit(1);
    }
    n->value = p;
    n->prev = prev;
    n->next = next;
    return n;
}

/*
 * 初始化链表。
 * 返回：传参的链表指针。
 */
PLLink *pllInit(PLLink *link) {
    // 初始化头，前后指针都指向自己
    PPNNode *h = pllNewNode(NULL, NULL, NULL);
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
void pllDestroy(PLLink *l) {
    PPNNode *n = l->head, *tmp = NULL;
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
void pllClear(PLLink *l) {
    PPNNode *n = l->head, *tmp = NULL;
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

void rangeCheck(PLLink *l, int index) {
    if (index < 0 || index >= l->size) {
        printf("Out of bounds, index:%d\n", index);
        exit(1);
    }
}

PPNNode *pllInternalGet(PLLink *l, int index) {
    rangeCheck(l, index);

    PPNNode *n = l->head;
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
void *pllGet(PLLink *l, int index) {
    return pllInternalGet(l, index)->value;
}

/*
 * 获取第一个元素。
 */
void *pllGetFirst(PLLink *l) {
    if (l->size == 0) {
        printf("Empty link, do not get the first element.\n");
        exit(1);
    }
    return l->head->next->value;
}

/*
 * 获取最后一个元素。
 */
void *pllGetLast(PLLink *l) {
    if (l->size == 0) {
        printf("Empty link, do not get the last element.\n");
        exit(1);
    }
    return l->head->prev->value;
}

void pllAddBefore(PLLink *l, void *p, PPNNode *node) {
    PPNNode *newNode = pllNewNode(node->prev, node, p);
    newNode->prev->next = newNode;
    newNode->next->prev = newNode;
    l->size++;
}

/*
 * 添加元素到队列开头
 */
void pllAddFirst(PLLink *l, void *p) {
    pllAddBefore(l, p, l->head->next);
}

/*
 * 添加元素到队列末尾。
 */
void pllAddLast(PLLink *l, void *p) {
    pllAddBefore(l, p, l->head);
}

/*
 * 添加元素到队列末尾。
 */
void pllAdd(PLLink *l, void *p) {
    pllAddLast(l, p);
}

/*
 * 添加元素到index下标处
 */
void pllInsert(PLLink *l, void *p, int index) {
    pllAddBefore(l, p, (index == l->size ? l->head : pllGet(l, index)));
}

/*
 * 查找元素第一次出现的位置。
 * 返回：找到返回位置，没找到返回-1。
 */
int pllIndexOf(PLLink *l, void *p) {
    PPNNode *n = l->head;
    for (int i = 0; i < l->size; i++) {
        n = n->next;
        if (n->value == p) {
            return i;
        }
    }
    return -1;
}

/*
 * 删除index位置元素。
 * 返回：被删除的元素。
 */
void *pllRemove(PLLink *l, int index) {
    // 无需检查index越界，pllInternalGet会检查。
    PPNNode *n = pllInternalGet(l, index);
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->prev = NULL;
    n->next = NULL;

    void *value = n->value;
    free(n);
    n = NULL;
    l->size--;

    return value;
}

/*
 * 删除第一个元素。
 * 返回：被删除的元素。
 */
void *pllRemoveFirst(PLLink *l) {
    return pllRemove(l, 0);
}

/*
 * 删除末尾元素。
 * 返回：被删除的元素。
 */
void *pllRemoveLast(PLLink *l) {
    return pllRemove(l, l->size - 1);
}

/*
 * 删除第一个值为p的元素。
 * 返回：找到并删除则返回被删除元素的位置，未找到则返回-1。
 */
int pllRemoveElem(PLLink *l, void *p) {
    int index = pllIndexOf(l, p);
    if (index >= 0) {
        pllRemove(l, index);
    }
    return index;
}

/*
 * 返回：被替换的元素值。
 */
void *pllSet(PLLink *l, int index, void *p) {
    PPNNode *n = pllInternalGet(l, index);
    void *old = n->value;
    n->value = p;
    return old;
}

/*
 * 元素入栈。
 */
void pllStkPush(PLLink *l, void *p) {
    pllAddLast(l, p);
}

/*
 * 查看栈顶元素。
 */
void *pllStkPeek(PLLink *l) {
    return pllGetLast(l);
}

/*
 * 弹出栈顶元素。
 */
void *pllStkPop(PLLink *l) {
    return pllRemoveLast(l);
}

/*
 * 将元素放入队列。
 */
void pllQuePush(PLLink *l, void *p) {
    pllAddLast(l, p);
}

/*
 * 查看队列第一个元素。
 */
void *pllQuePeek(PLLink *l) {
    return pllGetFirst(l);
}

/*
 * 弹出队列第一个元素。
 */
void *pllQuePop(PLLink *l) {
    return pllRemoveFirst(l);
}

/*
 * 返回元素的个数。
 */
int pllSize(PLLink *l) {
    return l->size;
}

/*
 * 判断link是否为空。
 */
int pllEmpty(PLLink *l) {
    return l->size == 0;
}

/*
 * 判断是否包含指定元素
 */
int pllContain(PLLink *l, void *p) {
    return pllIndexOf(l, p) != -1;
}

/*
 * 遍历前，重置迭代器位置到-1位置。
 */
void pllMoveToBeforeFirst(PLLink *l) {
    l->iterator = l->head;
}

/*
 * 遍历时，确定当前迭代器位置后面是否包含元素。
 */
int pllHasNext(PLLink *l) {
    PPNNode *next = l->iterator->next;
    return next != l->head;
}

/*
 * 遍历时，取下一个元素。
 */
void *pllNext(PLLink *l) {
    PPNNode *next = l->iterator->next;
    if (next == l->head) {
        printf("No next element\n");
        exit(1);
    }
    l->iterator = next;
    return l->iterator->value;
}

/*
 * 遍历前，重置迭代器位置到size位置。
 */
void pllMoveToAfterLast(PLLink *l) {
    l->iterator = l->head;
}

/*
 * 遍历时，确定当前迭代器位置前面是否包含元素。
 */
int pllHasPrevious(PLLink *l) {
    PPNNode *prev = l->iterator->prev;
    return prev != l->head;
}

/*
 * 遍历时，取上一个元素。
 */
void *pllPrevious(PLLink *l) {
    PPNNode *prev = l->iterator->prev;
    if (prev == l->head) {
        printf("No previous element\n");
        exit(1);
    }
    l->iterator = prev;
    return l->iterator->value;
}

/*
 * 打印所有元素
 */
void pllPrint(PLLink *l) {
    printf("[");
    PPNNode *n = l->head;
    for (int i = 0; i < l->size; i++) {
        n = n->next;
        printf(i == 0 ? "%p" : ", %p", n->value);
    }
    printf("]\n");
}






/************** test code ***************************************/


void pllTest() {
    PLLink link;
    PLLink *l = pllInit(&link);
    char s[6] = {0};
    printf("char[6]:%p,%p,%p,%p,%p,%p\n", s, s+1, s+2, s+3, s+4, s+5);
    pllAddFirst(l, s);
    printf("pllAddFirst(l, s);\n");
    pllPrint(l);

    pllAdd(l, s+1);
    printf("pllAdd(l, s+1);\n");
    pllPrint(l);

    pllAddLast(l, s+2);
    printf("pllAddLast(l, s+2);\n");
    pllPrint(l);

    printf("pllGet(l, 2)=%p\n", pllGet(l, 2));

    printf("pllGetFirst(l)=%p\n", pllGetFirst(l));
    
    printf("pllGetLast(l)=%p\n", pllGetLast(l));

    pllInsert(l, s+3, 3);
    printf("pllInsert(l, s+3, 3)\n");

    pllMoveToBeforeFirst(l);
    printf("forward:");
    while (pllHasNext(l)) {
        printf("%p ", pllNext(l));
    }
    printf("\n");

    pllMoveToAfterLast(l);
    printf("backward:");
    while (pllHasPrevious(l)) {
        printf("%p ", pllPrevious(l));
    }
    printf("\n");

    printf("pllIndexOf(l, s)=%d\n", pllIndexOf(l, s));

    printf("pllSize(l)=%d\n", pllSize(l));

    printf("pllRemove(l, 0)=%p, pllSize(l)=%d\n", pllRemove(l, 0), pllSize(l));
    pllPrint(l);

    printf("pllRemoveFirst(l)=%p, pllSize(l)=%d\n", pllRemoveFirst(l), pllSize(l));
    pllPrint(l);
    
    printf("pllSet(l, 0, s+4)=%p, pllSize(l)=%d\n", pllSet(l, 0, s+4), pllSize(l));
    pllPrint(l);

    printf("pllRemoveLast(l)=%p, pllEmpty(l)=%d\n", pllRemoveLast(l), pllEmpty(l));
    pllPrint(l);
    
    printf("pllContain(l, s+4)=%d\n", pllContain(l, s+4));
    printf("pllRemoveElem(l, s+4)=%d\n", pllRemoveElem(l, s+4));
    pllPrint(l);

    printf("pllEmpty(l)=%d, pllSize(l)=%d, pllContain(s)=%d\n\n", pllEmpty(l), pllSize(l), pllContain(l, s));

    pllDestroy(l);
    l = pllInit(l);
    printf("destroy(l),init(l):");
    pllPrint(l);

    pllStkPush(l, s);
    printf("pllStkPush(l, s)\n");
    
    pllStkPush(l, s+1);
    printf("pllStkPush(l, s+1)\n");
    pllPrint(l);

    printf("pllStkPeek(l)=%p\n", pllStkPeek(l));
    
    printf("pllStkPop(l)=%p\n", pllStkPop(l));
    pllPrint(l);
    
    pllClear(l);
    printf("pllClear(l)\n\n");

    pllQuePush(l, s);
    printf("pllQuePush(l, s)\n");
    
    printf("pllQuePeek(l)=%p\n", pllQuePeek(l));

    pllQuePush(l, s + 1);
    printf("pllQuePush(l, s+1)\n");
    pllPrint(l);

    printf("pllQuePop(l)=%p\n", pllQuePop(l));

    pllDestroy(l);
    printf("pllDestroy(l)\n\n");
}

/*
int main() {
    pllTest();
    pllTest();
    return 0;
}
*/
