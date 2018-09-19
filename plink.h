/*
 * 单向链表。
 *
 * 可当做Stack、Queue使用。
 * 
 * 性能分析：
 *     1. 多一个指针，内存加倍，相比与数组1.5倍扩容相差不多。
 *     2. 当作Stack、Queue使用时，均头尾直接操作，无遍历，性能很好。
 */

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
void plAddFirst(PLink *l, void *p);

/*
 * 添加元素到队列末尾。
 */
void plAddLast(PLink *l, void *p);

/*
 * 添加元素到队列末尾。
 */
void plAdd(PLink *l, void *p);

/*
 * 删除第一个元素。
 * 返回：被删除的元素。
 */
void *plRemoveFirst(PLink *l);

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
void plMoveToBeforeFirst(PLink *l);

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

