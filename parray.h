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

#define       PA_SUCCESS      1
#define       PA_FAILED       0


// ------ IList -----------------------------------------------

typedef struct {
    void **pBuf;
    int capacity;
    int size;
} PArray;

/*
 * 返回：初始化成功则返回传参a，失败则返回NULL。
 */
PArray *paInitDef(PArray *a);

/*
 * 返回：初始化成功则返回传参a，失败则返回NULL。
 */
PArray *paInit(PArray *a, int initCap);

/*
 * 清除所有元素，并是否内存。
 */
void paDestroy(PArray *a);

/*
 * 清除所有元素。
 */
void paClear(PArray *a);

/*
 * 获取指定位置的元素。
 */
void *paGet(PArray *a, int index);

/*
 * 返回：添加元素的下标。
 */
int paAdd(PArray *a, void *p);

/*
 * 添加元素到index下标处，index范围：index >=0 && index <= size。
 */
void paInsert(PArray *a, void *p, int index);

/*
 * 返回：被删除的元素。
 */
void *paRemove(PArray *a, int index);

/*
 * 首次找到指定元素，并删除。
 * 返回：找到并删除则返回被删除元素的位置，未找到则返回-1。
 */
int paRemoveElem(PArray *a, void *p);

/*
 * 返回：指定位置原来元素的值。
 */
void *paSet(PArray *a, int index, void *p);

/*
 * 返回：首次找到指定元素的下标，没找到返回-1。
 */
int paIndexOf(PArray *a, void *p);

/*
 * 判断是否包含某元素。
 */
int paContain(PArray *a, void *p);

/*
 * 返回：#SUCCESS表示成功，#FAILED表示失败
 */
int paEnsureCapacity(PArray *a, int minCapacity);

/*
 * 返回元素个数。
 */
int paSize(PArray *a);

/*
 * 返回数组容量。
 */
int paCapacity(PArray *a);

/*
 * 判断是否为空。
 */
int paEmpty(PArray *a);

/*
 * 打印所有元素
 */
void paPrint(PArray *a);

/*
 * 查看栈顶元素。
 */
void *paStackPeek(PArray *a);

/*
 * 弹出栈顶元素。
 */
void *paStackPop(PArray *a);

/*
 * 元素入栈。
 */
void paStackPush(PArray *a, void *p);

/*
 * 查看队列第一个元素。
 */
void *paQueuePeek(PArray *a);

/*
 * 弹出队列第一个元素。
 */
void *paQueuePop(PArray *a);

/*
 * 将元素放入队列末尾。
 */
void paQueuePush(PArray *a, void *p);

