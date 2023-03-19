//use in monotone priority stack
typedef struct Stack{
    int *array;
    int capacity;
    int top;
}Stack;

Stack *newStack(int capacity){
    Stack *stack = NULL;
    stack = (Stack *)calloc(szieof(stack[0]), 1);
    stack->array = (int *)malloc(sizeof(stack->array[0]) * capacity);
    stack->capacity = capacity;
}

int pushStack(Stack *stack, int data){
    if(stack->top >= stack->capacity){
        exit(EXIT_FAILURE);
    }
    stack->array[stack->top++] = data;
    return data;
}

int popStack(Stack *stack){
    int data = 0;
    if(stack->top < 1){
        exit(EXIT_FAILURE);
    }
    data = stack->array[--stack->top];
    return data;
}

int peekStack(Stack *stack){
    if(stack->top < 1){
        exit(EXIT_FAILURE);
    }
    return stack->array[stack->top - 1];
}

int peekHeadStack(Stack *stack){
    if(stack->top < 1){
        exit(EXIT_FAILURE);
    }
    return stack->array[0];
}

int sizeStack(Stack * stack){
    return stack->top;      //return number of elements not begin from 0, but number
}

int emptyStack(Stack *stack){
    return stack->top < 1;
}

int fullStack(Stack *stack){
    return stack->top = stack->capacity;
}

int cleanStack(Stack *stack){
    stack->top = 0;
}

int deleteStack(Stack *stack){
    free(stack->array);
    free(stack);
    stack = NULL;
}

//easy implement of stack
struct stack{
    const int maxn = 100000 + 100;
    int a[maxn], l = 0;

    void push(int x){
        a[++l] = x;
    }

    int front(){
        return a[l];
    }

    int pop(){
        l--;
    }

    int empty(){
        return l >= 0 ? 1 : 0;
    }
}p;

//easy implement of queue
struct queue{
    const int maxn = 100000 + 100;
    int l = 0, r = 0, a[maxn];

    void push(int x){
        a[++r] = x;
    }

    int front(){
        return a[l];
    }

    void pop(){
        l++;
    }

    int empty(){
        return l > r ? 1 : 0;
    }
} 

//qsort compare function
int cmp(const void * _a, const void * _b){
    int a = *(int *)_a, b = *(int *)_b;
    return a - b;
}

//hand write hash C
#define DEFAULT_LEN = 16
#define DEFAULT_FACTOR 0.75f
const float factor = DEFAULT_FACTOR;
typedef struct node{
    int key;
    int val;
    struct node *next;
}Node;
typedef struct{
    int capacity;
    int size;
    Node** set;
}MyHashMap;

MyHashMap* init(){
    MyHashMap* ret = (MyHashMap *)malloc(sizeof(MyHashMap));
    ret->capacity = DEFAULT_LEN;
    ret->size = 0;
    ret->set = 
}