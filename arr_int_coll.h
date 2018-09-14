/*
 * 采用数组实现的List<int>、Stack<int>数据结构。
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
    int *pBuf;
    int capacity;
    int size;
} IList;

/*
 * 返回：初始化成功则返回传参p，失败则返回NULL。
 */
IList *ilInitDef(IList *p);

/*
 * 返回：初始化成功则返回传参p，失败则返回NULL。
 */
IList *ilInit(IList *p, int initCap);

void ilDestroy(IList *p);

void ilClear(IList *p);

int ilGet(IList *p, int index);

/*
 * 返回：添加元素的下标。
 */
int ilAdd(IList *p, int num);

/*
 * 添加元素到index下标处，index范围：index >=0 && index <= size。
 */
void ilInsert(IList *p, int index, int num);

/*
 * 返回：被删除的数
 */
int ilRemove(IList *p, int index);

/*
 * 返回：原数
 */
int ilSet(IList *p, int index, int num);

/*
 * 返回：首次找到num的下标，没找到返回-1。
 */
int ilIndex(IList *p, int num);

/*
 * 返回：#SUCCESS表示成功，#FAILED表示失败
 */
int ilEnsureCapacity(IList *p, int minCapacity);

int ilSize(IList *p);

int ilCapacity(IList *p);

int ilEmpty(IList *p);

int ilToStrLen(IList *p);

char *ilToString(IList *p, char *str, int len);

/*
 * 打印所有元素
 */
void ilPrint(IList *p);




// ------ IStack ------------------------------------------------------

typedef IList IStack;

/*
 * 返回：初始化成功则返回传参p，失败则返回NULL。
 */
IStack *isInitDef(IStack *p);

/*
 * 返回：初始化成功则返回传参p，失败则返回NULL。
 */
IStack *isInit(IStack *p, int initCap);

/*
 * 清除数据，释放内存
 */
void isDestroy(IStack *p);

/*
 * 清除栈中所有元素
 */
void isClear(IStack *p);

int isPeek(IStack *p);

int isPop(IStack *p);

void isPush(IStack *p, int num);

/*
 * 确保最小容量
 * 返回：#SUCCESS表示成功，#FAILED表示失败
 */
int isEnsureCapacity(IStack *p, int minCapacity);

/*
 * 返回：元素个数
 */
int isSize(IStack *p);

/*
 * 返回：容量大小
 */
int isCapacity(IStack *p);

/*
 * 返回：0表示空，非0表示非空
 */
int isEmpty(IStack *p);

/*
 * 返回：格式化所有元素用空格隔开的字符串
 */
int isToStrLen(IStack *p);

/*
 * 返回：传参str
 */
char *isToString(IStack *p, char *str, int len); 
/*
 * 打印所有元素
 */
void isPrint(IStack *p);
