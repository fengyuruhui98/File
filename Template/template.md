# Template 
>Capture

* [Use in monotone priority stack](#use-in-monotone-priority-stack)  
* [Easy implement of queue](#easy-implement-of-queue)  
* [Easy implement of stack](#easy-implement-of-stack)  
* [Qsort compare function](#qsort-compare-function)


## Use in monotone priority stack
```c
typedef struct Stack{
    int capacity;
    int top;
    int *array
}Stack;

Stack * newStack(int capacity){
    Stack * stack = NULL;
    stack = (Stack *)calloc(sizeof(stack[0]), 1);
    stack -> array = (int *)malloc(sizeof(stack -> array[0]) * capacity);
    stack -> capacity = capacity;
}

int pushStack(Stack * stack, int data){
    if(stack -> top >= stack -> capacity) exit(EXIT_FAILURE);
    /********************************************************/
    while(!stack.isEmpty() && stack -> array[stack -> top - 1] < data) top--;
    /********************************************************/
    while(!stack.isEmpty() && stack -> array[stack -> top - 1] > data) top--;
    /********************************************************/
    stack -> array[stack -> top++] = data;
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
```

## Easy implement of stack
```c
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
```
## Easy implement of queue
```c
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
```
## Qsort compare function
```c 
//qsort compare function
int cmp(const void * _a, const void * _b){
    int a = *(int *)_a, b = *(int *)_b;
    return a - b;
    //return (*(int *)_a) - (*(int *)_b);
}
```

## Two-Qsort compare function
```c 
//qsort compare function
static inline int cmp(const void *pa, const void *pb) {
    return (*(int **)pb)[1] - (*(int **)pa)[1];
}
```