/*
 * 采用数组实现的List<void *>、Stack<void *>数据结构。
 *
 * 自动扩容：
 *     当容量不够用时，自动扩充容量。扩充容量过程中需要重新分配
 *     内存，并复制原有数据。所以，在初始化或者使用过程中明确容
 *     量。
 *
 * 使用前需要初始化分配内存，使用完需要调用销毁函数释放内存。
 *
 * */

#define       SUCCESS      1
#define       FAILED       0


// ------ IList -----------------------------------------------

typedef struct {
    void **ppBuf;
    int capacity;
    int size;
} PList;

/*
 * 返回：初始化成功则返回传参pp，失败则返回NULL。
 */
PList *plInitDef(PList *pp);

/*
 * 返回：初始化成功则返回传参pp，失败则返回NULL。
 */
PList *plInit(PList *pp, int initCap);

void plDestroy(PList *pp);

void plClear(PList *pp);

void *plGet(PList *pp, int index);

/*
 * 返回：添加元素的下标。
 */
int plAdd(PList *pp, void *pValue);

/*
 * 添加元素到index下标处，index范围：index >=0 && index <= size。
 */
void plInsert(PList *pp, int index, void *pValue);

/*
 * 返回：被删除的指针
 */
void *plRemove(PList *pp, int index);

/*
 * 返回：原指针
 */
void *plSet(PList *pp, int index, void *pValue);

/*
 * 返回：首次找到指针的下标，没找到返回-1。
 */
int plIndex(PList *pp, void *pValue);

/*
 * 返回：#SUCCESS表示成功，#FAILED表示失败
 */
int plEnsureCapacity(PList *pp, int minCapacity);

int plSize(PList *pp);

int plCapacity(PList *pp);

int plEmpty(PList *pp);

/*
 * 打印所有元素
 */
void plPrint(PList *pp);




// ------ PStack ------------------------------------------------------

typedef PList PStack;

/*
 * 返回：初始化成功则返回传参pp，失败则返回NULL。
 */
PStack *psInitDef(PStack *pp);

/*
 * 返回：初始化成功则返回传参pp，失败则返回NULL。
 */
PStack *psInit(PStack *pp, int initCap);

/*
 * 清除数据，释放内存
 */
void psDestroy(PStack *pp);

/*
 * 清除栈中所有元素
 */
void psClear(PStack *pp);

void *psPeek(PStack *pp);

void *psPop(PStack *pp);

void psPush(PStack *pp, void *pValue);

/*
 * 确保最小容量
 * 返回：#SUCCESS表示成功，#FAILED表示失败
 */
int psEnsureCapacity(PStack *pp, int minCapacity);

/*
 * 返回：元素个数
 */
int psSize(PStack *pp);

/*
 * 返回：容量大小
 */
int psCapacity(PStack *pp);

/*
 * 返回：0表示空，非0表示非空
 */
int psEmpty(PStack *pp);

/*
 * 打印所有元素
 */
void psPrint(PStack *pp);
