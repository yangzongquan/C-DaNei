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

/*
 * 初始化链表。
 * 返回：传参的链表指针。
 */
PLLink *pllInit(PLLink *link);

/*
 * 销毁链表，并释放所有相关内存。
 * 除非重新初始化，否则销毁后不能使用。
 */
void pllDestroy(PLLink *l);

/*
 * 删除所有元素，并释放相关内存。
 */
void pllClear(PLLink *l);

/*
 * 获取index指定位置的元素。
 */
void *pllGet(PLLink *l, int index);

/*
 * 获取第一个元素。
 */
void *pllGetFirst(PLLink *l);

/*
 * 获取最后一个元素。
 */
void *pllGetLast(PLLink *l);

/*
 * 添加元素到队列开头
 */
void pllAddFirst(PLLink *l, void *p);

/*
 * 添加元素到队列末尾。
 */
void pllAddLast(PLLink *l, void *p);

/*
 * 添加元素到队列末尾。
 */
void pllAdd(PLLink *l, void *p);

/*
 * 添加元素到index下标处
 */
void pllInsert(PLLink *l, void *p, int index);

/*
 * 查找元素第一次出现的位置。
 * 返回：找到返回位置，没找到返回-1。
 */
int pllIndexOf(PLLink *l, void *p);

/*
 * 删除index位置元素。
 * 返回：被删除的元素。
 */
void *pllRemove(PLLink *l, int index);

/*
 * 删除第一个元素。
 * 返回：被删除的元素。
 */
void *pllRemoveFirst(PLLink *l);

/*
 * 删除末尾元素。
 * 返回：被删除的元素。
 */
void *pllRemoveLast(PLLink *l);

/*
 * 删除第一个值为p的元素。
 * 返回：找到并删除则返回被删除元素的位置，未找到则返回-1。
 */
int pllRemoveElem(PLLink *l, void *p);

/*
 * 返回：被替换的元素值。
 */
void *pllSet(PLLink *l, int index, void *p);

/*
 * 元素入栈。
 */
void pllStkPush(PLLink *l, void *p);

/*
 * 查看栈顶元素。
 */
void *pllStkPeek(PLLink *l);

/*
 * 弹出栈顶元素。
 */
void *pllStkPop(PLLink *l);

/*
 * 将元素放入队列。
 */
void pllQuePush(PLLink *l, void *p);

/*
 * 查看队列第一个元素。
 */
void *pllQuePeek(PLLink *l);

/*
 * 弹出队列第一个元素。
 */
void *pllQuePop(PLLink *l);

/*
 * 返回元素的个数。
 */
int pllSize(PLLink *l);

/*
 * 判断link是否为空。
 */
int pllEmpty(PLLink *l);

/*
 * 判断是否包含指定元素
 */
int pllContain(PLLink *l, void *p);

/*
 * 遍历前，重置迭代器位置到-1位置。
 */
void pllMoveToBeforeFirst(PLLink *l);

/*
 * 遍历时，确定当前迭代器位置后面是否包含元素。
 */
int pllHasNext(PLLink *l);

/*
 * 遍历时，取下一个元素。
 */
void *pllNext(PLLink *l);

/*
 * 遍历前，重置迭代器位置到size位置。
 */
void pllMoveToAfterLast(PLLink *l);

/*
 * 遍历时，确定当前迭代器位置前面是否包含元素。
 */
int pllHasPrevious(PLLink *l);

/*
 * 遍历时，取上一个元素。
 */
void *pllPrevious(PLLink *l);

/*
 * 打印所有元素
 */
void pllPrint(PLLink *l);

