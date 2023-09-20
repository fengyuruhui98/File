//目录
/*
1.定义结构
2.先中后序递归遍与非递归遍历
3.层序遍历
4.线索树
5.树与森林
6.并查集
*/

//1.定义结构-链式结构
typedef struct BiTNode{
	ElemType data;//数据域
	struct BiTNode * lchild, *rchild;//左、右孩子指针
}BiTNode, * BiTree;

//2.先中后序遍历
//先序递归
void PreOrder(BiTree T){
	if(T != NULL){
		visit(T);//访问根结点
		PreOrder(T -> lchild);//递归遍历左子树
		PreOrder(T -> rchild);//递归遍历右子树
	}
}
//先序非递归
void PreOrder2(BiTree T){
	InitStack(S);//初始化栈
	BiTree p = T;//p是遍历指针
	while(p || !IsEmpty(S)){//栈不空或p不空时循环
		if(p){//一路向左
			visit(p);//访问当前结点
			push(S, p);//入栈
			p = p -> lchild;//左孩子不空，一直向左走
		}else{//出栈，并转向出栈结点的右子树
			pop(S, p);//栈顶元素出栈
			p = p -> rchild;//向右子树走，p赋值为当前结点的右孩子
		}//返回while循环继续进入if-else语句
	}
}

//中序递归
void InOrder(BiTree T){
	if(T != NULL){
		InOrder(T -> lchild);//递归遍历左子树
		visit(T);//访问根结点
		InOrder(T -> rchild);//递归遍历右子树
	}
}
//中序非递归
void InOrder2(BiTree T){
	InitStack(S);//初始化栈S
	BiTree p = T;//p是遍历指针
	while(p || !IsEmpty(S)){//栈不空或者p不空时循环
		if(p){//一路向左
			push(S, p);//当前结点入栈
			p = p -> lchild;//左孩子不空，一直向左走
		}else{//出栈，并转向出栈结点的右子树
			pop(S, p);//栈顶元素出栈
			visit(p);//访问出栈结点
			p = p -> rchild;//向右孩子走，p赋值为当前结点的右孩子
		}//返回while循环继续进入if-else语句
	}
}

//后序遍历
void PostOrder(BiTree T){
	if(T != NULL){
		PostOrder(T -> lchild);//递归遍历左子树
		PostOrder(T -> rchild);//递归遍历右子树
		visit(T);//访问根结点
	}
}
//后序非递归
void PostOrder2(BiTree T){
	InitStack(S);
	BiTree p = T;
	r = NULL;
	while(p || !IsEmpty(S)){
		if(p){//走到最左边
			push(S, p);
			p = p -> lchild;
		}
		else{//向右
			GetTop(S, p);//读栈顶结点（非出栈）
			if(p -> rchild && p -> rchild != r)//若右子树存在，且未被访问过
				p = p -> rchild;//转向右
			else{//否则弹出结点并访问
				pop(S, p);//将结点弹出
				visit(p);//访问该结点
				r = p;//记录最近访问过的结点
				p = NULL;//结点访问完后，重置p指针。
			}
		}//else
	}//while
}

//3.层次遍历
void LevelOrder(BiTree T){
	InitQueue(Q);//初始化辅助队列
	BiTree p;
	EnQueue(Q, T);//根结点入队
	while(!IsEmpty(Q)){//队列不空则循环
		DeQueue(Q, p);//队头结点出队
		visit(p);//访问出队结点
		if(p -> lchild != NULL){
			EnQueue(Q, p -> lchild);//左子树不空，则左子树根结点入队
		}
		if(p -> rchild != NULL){
			DeQueue(Q, p -> rchild);//右子树不空，则右子树根结点入队
		}
	}
}

//4.线索树
//线索二叉树存储结构
typedef struct ThreadNode{
	ElemType data;//数据元素
	struct ThreadNode *lchild, *rchild;//左、右子树指针
	int ltag, rtag;//左、右线索标志
}ThreadNode, *ThreadTree;
//通过中序遍历对二叉树线索化的递归算法：
void InThread(ThreadTree &p, ThreadTree &pre){
	if(p != NULL){
		InThread(p -> lchild, pre);//递归线索化左子树
		if(p -> rchild == NULL){//左子树为空，建立前驱线索
			p -> lchild = pre;
			p -> ltag = 1;
		}
		if(pre != NULL && pre -> rchild == NULL){
			pre -> rchild = p;//建立前驱结点的后继线索
			pre -> rtag = 1;
		}
		pre = p;//标记当前结点成为刚刚访问过的结点
		InThread(p -> rchild, pre);//递归，线索化右子树
	}//if(p != NULL)
}
//通过中序遍历建立中序线索二叉树的主过程算法：
void CreateInThread(ThreadTree T){
	ThreadTree pre = NULL;
	if(T != NULL){//非空二叉树，线索化
		InThread(T, pre);//线索化二叉树
		pre -> rchild = NULL;//处理遍历的最后一个结点
		pre -> rtag = 1;
	}
}
//不含头结点的线索二叉树的遍历算法：
//（1）求中序线索二叉树中中序序列下的第一个结点
ThreadNode *Firstnode(ThreadNode *p){
	while(p -> ltag == 0) p = p -> lchild;//最左下结点（不一定是叶结点）
	return p;
}
//##ltag和lchild换成rtag和rchild，将Firstnode替换为Lastnode，即为求中序线索二叉树的最后一个结点
//（2）求中序线索二叉树中结点p在中序序列下的后继
ThreadNode *Nextnode(ThreadNode *p){
	if(p -> rtag == 0) return Firstnode(p -> rchild);
	else return p -> rchild;//直接返回后继线索
}
//##rtag和rchild换成ltag和lchild，即为求中序线索二叉树中结点p的前驱
//（3）利用（1）（2）不含头结点的中序线索二叉树的中序遍历算法
void InOrder(ThreadNode *T){
	for(ThreadNode *p = Firstnode(T); p != NULL; p = Nextnode(p))
		visit(p);
}

//5.树与森林
//双亲表示法存储结构存储树
#define MAX_TREE_SIZE 100//树中最多结点数
typedef struct{//树的结点定义
	ElemType data;//数据元素
	int parent;//双亲位置域
}PTNode;
typedef struct{//树的类型定义
	PTNode nodes[MAX_TREE_SIZE];//双亲表示
	int n;//结点数
}PTree;

//孩子兄弟表示法
typedef struct CSNode{
	ElemType data;//数据域
	struct CSNode *firstchild, *nextsibling;//第一个孩子和右兄弟指针
}CSNode, *CSTree;

//6.并查集
//结构定义
#define SIZE 100
int UFSets[SIZE];//集合元素数组（双亲指针数组）

//初始化操作（S即为并查集）
void Initial(int S[]){
	for(int i = 0; i < size; i++)//每个自成单元素集合
		S[i] = -1;
}

//Find操作（函数在并查集S中查找并返回包含元素x的树的根）
int Find(int S[], int x){
	while(S[x] >= 0)//循环寻找x的根
		x = S[x];
	return x;//根的S[]小于0
}

//判断两个元素是否属于同一个集合，只需要分别找到它们的根结点，比较根结点是否相同即可
//Union操作（函数求两个不相交子集合的并集）
void Union(int S[], int Root1, int Root2){
	//要求Root1与Root2是不同的，且表示子集合的名字
	S[Root2] = Root1;//将根Root2连接到另一根Root1下面。
}