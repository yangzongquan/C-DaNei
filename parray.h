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
 *      只能调用paStk*方法。
 *
 * 当做Queue使用时:
 *      一般定长、省内存时使用，其他情况使用LinkQueue。
 *      （后面单独实现：数组+循环队列+自动扩容）
 *
 *      只能调用paQue*方法。
 * */

#define       PA_SUCCESS      1
#define       PA_FAILED       0


typedef struct {
    void **pBuf;
    int capacity;
    int size;
} PArray, PArrayList;

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


//-------- Stack ----------------------------

typedef PArray PArrayStack;

/*
 * 采用默认容量初始化栈。
 */
PArrayStack *paStkInitDef(PArrayStack *s);

/*
 * 根据指定的容量初始化栈。
 */
PArrayStack *paStkInit(PArrayStack *s, int initCap);
/*
 * 清除元素，释放内存。
 */
void paStkDestroy(PArrayStack *s);

/*
 * 清除所有元素。
 */
void paStkClear(PArrayStack *s);

/*
 * 返回栈中元素个数。
 */
int paStkSize(PArrayStack *s);

/*
 * 判断栈是否为空。
 */
int paStkEmpty(PArrayStack *s);

/*
 * 获取栈容量。
 */
int paStkCapacity(PArrayStack *s);

/*
 * 查看栈顶元素。
 */
void *paStkPeek(PArrayStack *s);

/*
 * 弹出栈顶元素。
 */
void *paStkPop(PArrayStack *s);

/*
 * 确保minCapacity容量。
 *
 * 返回：#PA_SUCCESS表示成功，#PA_FAILED表示失败
 */
int paStkEnsureCapacity(PArrayStack *s, int minCapacity);

/*
 * 将元素放入栈。
 */
void paStkPush(PArrayStack *s, void *p);

/*
 * 用于栈遍历。
 */
void *paStkGet(PArrayStack *s, int index);

/*
 * 判断栈是否包含指定元素。
 */
int paStkContain(PArrayStack *s, void *p);

/*
 * 打印栈。
 */
void paStkPrint(PArrayStack *s);




//-------- Queue ----------------------------

typedef struct {
    void **pBuf;
    int capacity;
    int size;
    int head;
    int tail;
} PArrayQueue;

/*
 * 采用默认容量初始化队列。
 */
PArrayQueue *paQueInitDef(PArrayQueue *q);

/*
 * 根据指定的容量初始化队列。
 */
PArrayQueue *paQueInit(PArrayQueue *q, int initCap);
/*
 * 清除元素，释放内存。
 */
void paQueDestroy(PArrayQueue *q);

/*
 * 清除所有元素。
 */
void paQueClear(PArrayQueue *q);

/*
 * 返回队列中元素个数。
 */
int paQueSize(PArrayQueue *q);

/*
 * 判断队列是否为空。
 */
int paQueEmpty(PArrayQueue *q);

/*
 * 获取队列容量。
 */
int paQueCapacity(PArrayQueue *q);

/*
 * 查看队列第一个元素。
 */
void *paQuePeek(PArrayQueue *q);

/*
 * 弹出队列第一个元素。
 */
void *paQuePop(PArrayQueue *q);

/*
 * 确保minCapacity容量。
 *
 * 返回：#PA_SUCCESS表示成功，#PA_FAILED表示失败
 */
int paQueEnsureCapacity(PArrayQueue *q, int minCapacity);

/*
 * 将元素放入队列末尾。
 */
void paQuePush(PArrayQueue *q, void *p);

/*
 * 用于队列遍历。
 */
void *paQueGet(PArrayQueue *q, int index);

/*
 * 判断队列是否包含指定元素。
 */
int paQueContain(PArrayQueue *q, void *p);

/*
 * 打印队列。
 */
void paQuePrint(PArrayQueue *q);


