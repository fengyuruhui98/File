//Ŀ¼
/*
1.����ṹ
2.���к���ݹ����ǵݹ����
3.�������
4.������
5.����ɭ��
6.���鼯
*/

//1.����ṹ-��ʽ�ṹ
typedef struct BiTNode{
	ElemType data;//������
	struct BiTNode * lchild, *rchild;//���Һ���ָ��
}BiTNode, * BiTree;

//2.���к������
//����ݹ�
void PreOrder(BiTree T){
	if(T != NULL){
		visit(T);//���ʸ����
		PreOrder(T -> lchild);//�ݹ����������
		PreOrder(T -> rchild);//�ݹ����������
	}
}
//����ǵݹ�
void PreOrder2(BiTree T){
	InitStack(S);//��ʼ��ջ
	BiTree p = T;//p�Ǳ���ָ��
	while(p || !IsEmpty(S)){//ջ���ջ�p����ʱѭ��
		if(p){//һ·����
			visit(p);//���ʵ�ǰ���
			push(S, p);//��ջ
			p = p -> lchild;//���Ӳ��գ�һֱ������
		}else{//��ջ����ת���ջ����������
			pop(S, p);//ջ��Ԫ�س�ջ
			p = p -> rchild;//���������ߣ�p��ֵΪ��ǰ�����Һ���
		}//����whileѭ����������if-else���
	}
}

//����ݹ�
void InOrder(BiTree T){
	if(T != NULL){
		InOrder(T -> lchild);//�ݹ����������
		visit(T);//���ʸ����
		InOrder(T -> rchild);//�ݹ����������
	}
}
//����ǵݹ�
void InOrder2(BiTree T){
	InitStack(S);//��ʼ��ջS
	BiTree p = T;//p�Ǳ���ָ��
	while(p || !IsEmpty(S)){//ջ���ջ���p����ʱѭ��
		if(p){//һ·����
			push(S, p);//��ǰ�����ջ
			p = p -> lchild;//���Ӳ��գ�һֱ������
		}else{//��ջ����ת���ջ����������
			pop(S, p);//ջ��Ԫ�س�ջ
			visit(p);//���ʳ�ջ���
			p = p -> rchild;//���Һ����ߣ�p��ֵΪ��ǰ�����Һ���
		}//����whileѭ����������if-else���
	}
}

//�������
void PostOrder(BiTree T){
	if(T != NULL){
		PostOrder(T -> lchild);//�ݹ����������
		PostOrder(T -> rchild);//�ݹ����������
		visit(T);//���ʸ����
	}
}
//����ǵݹ�
void PostOrder2(BiTree T){
	InitStack(S);
	BiTree p = T;
	r = NULL;
	while(p || !IsEmpty(S)){
		if(p){//�ߵ������
			push(S, p);
			p = p -> lchild;
		}
		else{//����
			GetTop(S, p);//��ջ����㣨�ǳ�ջ��
			if(p -> rchild && p -> rchild != r)//�����������ڣ���δ�����ʹ�
				p = p -> rchild;//ת����
			else{//���򵯳���㲢����
				pop(S, p);//����㵯��
				visit(p);//���ʸý��
				r = p;//��¼������ʹ��Ľ��
				p = NULL;//�������������pָ�롣
			}
		}//else
	}//while
}

//3.��α���
void LevelOrder(BiTree T){
	InitQueue(Q);//��ʼ����������
	BiTree p;
	EnQueue(Q, T);//��������
	while(!IsEmpty(Q)){//���в�����ѭ��
		DeQueue(Q, p);//��ͷ������
		visit(p);//���ʳ��ӽ��
		if(p -> lchild != NULL){
			EnQueue(Q, p -> lchild);//���������գ�����������������
		}
		if(p -> rchild != NULL){
			DeQueue(Q, p -> rchild);//���������գ�����������������
		}
	}
}

//4.������
//�����������洢�ṹ
typedef struct ThreadNode{
	ElemType data;//����Ԫ��
	struct ThreadNode *lchild, *rchild;//��������ָ��
	int ltag, rtag;//����������־
}ThreadNode, *ThreadTree;
//ͨ����������Զ������������ĵݹ��㷨��
void InThread(ThreadTree &p, ThreadTree &pre){
	if(p != NULL){
		InThread(p -> lchild, pre);//�ݹ�������������
		if(p -> rchild == NULL){//������Ϊ�գ�����ǰ������
			p -> lchild = pre;
			p -> ltag = 1;
		}
		if(pre != NULL && pre -> rchild == NULL){
			pre -> rchild = p;//����ǰ�����ĺ������
			pre -> rtag = 1;
		}
		pre = p;//��ǵ�ǰ����Ϊ�ոշ��ʹ��Ľ��
		InThread(p -> rchild, pre);//�ݹ飬������������
	}//if(p != NULL)
}
//ͨ������������������������������������㷨��
void CreateInThread(ThreadTree T){
	ThreadTree pre = NULL;
	if(T != NULL){//�ǿն�������������
		InThread(T, pre);//������������
		pre -> rchild = NULL;//������������һ�����
		pre -> rtag = 1;
	}
}
//����ͷ���������������ı����㷨��
//��1�����������������������������µĵ�һ�����
ThreadNode *Firstnode(ThreadNode *p){
	while(p -> ltag == 0) p = p -> lchild;//�����½�㣨��һ����Ҷ��㣩
	return p;
}
//##ltag��lchild����rtag��rchild����Firstnode�滻ΪLastnode����Ϊ���������������������һ�����
//��2�������������������н��p�����������µĺ��
ThreadNode *Nextnode(ThreadNode *p){
	if(p -> rtag == 0) return Firstnode(p -> rchild);
	else return p -> rchild;//ֱ�ӷ��غ������
}
//##rtag��rchild����ltag��lchild����Ϊ�����������������н��p��ǰ��
//��3�����ã�1����2������ͷ����������������������������㷨
void InOrder(ThreadNode *T){
	for(ThreadNode *p = Firstnode(T); p != NULL; p = Nextnode(p))
		visit(p);
}

//5.����ɭ��
//˫�ױ�ʾ���洢�ṹ�洢��
#define MAX_TREE_SIZE 100//�����������
typedef struct{//���Ľ�㶨��
	ElemType data;//����Ԫ��
	int parent;//˫��λ����
}PTNode;
typedef struct{//�������Ͷ���
	PTNode nodes[MAX_TREE_SIZE];//˫�ױ�ʾ
	int n;//�����
}PTree;

//�����ֵܱ�ʾ��
typedef struct CSNode{
	ElemType data;//������
	struct CSNode *firstchild, *nextsibling;//��һ�����Ӻ����ֵ�ָ��
}CSNode, *CSTree;

//6.���鼯
//�ṹ����
#define SIZE 100
int UFSets[SIZE];//����Ԫ�����飨˫��ָ�����飩

//��ʼ��������S��Ϊ���鼯��
void Initial(int S[]){
	for(int i = 0; i < size; i++)//ÿ���Գɵ�Ԫ�ؼ���
		S[i] = -1;
}

//Find�����������ڲ��鼯S�в��Ҳ����ذ���Ԫ��x�����ĸ���
int Find(int S[], int x){
	while(S[x] >= 0)//ѭ��Ѱ��x�ĸ�
		x = S[x];
	return x;//����S[]С��0
}

//�ж�����Ԫ���Ƿ�����ͬһ�����ϣ�ֻ��Ҫ�ֱ��ҵ����ǵĸ���㣬�Ƚϸ�����Ƿ���ͬ����
//Union�������������������ཻ�Ӽ��ϵĲ�����
void Union(int S[], int Root1, int Root2){
	//Ҫ��Root1��Root2�ǲ�ͬ�ģ��ұ�ʾ�Ӽ��ϵ�����
	S[Root2] = Root1;//����Root2���ӵ���һ��Root1���档
}