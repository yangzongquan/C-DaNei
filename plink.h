/*
 * 双向环链表。
 * head是一个无值节点，不存放数据，head->next指向链表头，head->prev指向链表尾。
 *     head->next -- Node1 -- Node2 -- ... -- NodeN -- prev<-head
 *
 * 可当做List、Stack、Queue使用。
 *
 */

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

/*
 * 初始化链表。
 * 返回：传参的链表指针。
 */
PLink *plInit(PLink *link);

/*
 * 销毁链表，并释放所有相关内存。
 * 除非重新初始化，否则销毁后不能使用。
 */
void plDestroy(PLink *l);

/*
 * 删除所有元素，并释放相关内存。
 */
void plClear(PLink *l);

/*
 * 获取index指定位置的元素。
 */
void *plGet(PLink *l, int index);

/*
 * 获取第一个元素。
 */
void *plGetFirst(PLink *l);

/*
 * 获取最后一个元素。
 */
void *plGetLast(PLink *l);

/*
 * 添加元素到队列开头
 */
void plAddFirst(PLink *l, void *pValue);

/*
 * 添加元素到队列末尾。
 */
void plAddLast(PLink *l, void *pValue);

/*
 * 添加元素到队列末尾。
 */
void plAdd(PLink *l, void *pValue);

/*
 * 添加元素到index下标处
 */
void plInsert(PLink *l, void *pValue, int index);

/*
 * 查找元素第一次出现的位置。
 * 返回：找到返回位置，没找到返回-1。
 */
int plIndexOf(PLink *l, void *pValue);

/*
 * 删除index位置元素。
 * 返回：被删除的元素。
 */
void *plRemove(PLink *l, int index);

/*
 * 删除第一个元素。
 * 返回：被删除的元素。
 */
void *plRemoveFirst(PLink *l);

/*
 * 删除末尾元素。
 * 返回：被删除的元素。
 */
void *plRemoveLast(PLink *l);

/*
 * 删除第一个值为p的元素。
 * 返回：找到并删除则返回被删除元素的位置，未找到则返回-1。
 */
int plRemoveElem(PLink *l, void *p);

/*
 * 返回：被替换的元素值。
 */
void *plSet(PLink *l, int index, void *p);

/*
 * 元素入栈。
 */
void plStackPush(PLink *l, void *p);

/*
 * 查看栈顶元素。
 */
void *plStackPeek(PLink *l);

/*
 * 弹出栈顶元素。
 */
void *plStackPop(PLink *l);

/*
 * 将元素放入队列。
 */
void plQueuePush(PLink *l, void *p);

/*
 * 查看队列第一个元素。
 */
void *plQueuePeek(PLink *l);

/*
 * 弹出队列第一个元素。
 */
void *plQueuePop(PLink *l);

/*
 * 返回元素的个数。
 */
int plSize(PLink *l);

/*
 * 判断link是否为空。
 */
int plEmpty(PLink *l);

/*
 * 判断是否包含指定元素
 */
int plContain(PLink *l, void *p);

/*
 * 遍历前，重置迭代器位置到-1位置。
 */
void plResetIterator(PLink *l);

/*
 * 遍历时，确定当前迭代器位置后面是否包含元素。
 */
int plHasNext(PLink *l);

/*
 * 遍历时，取下一个元素。
 */
void *plNext(PLink *l);

/*
 * 打印所有元素
 */
void plPrint(PLink *l);

