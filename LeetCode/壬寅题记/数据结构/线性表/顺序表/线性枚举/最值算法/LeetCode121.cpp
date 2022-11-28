int maxProfit(int* prices, int pricesSize){
    int max = 0, temp, i, j;
    for(i = 0; i< pricesSize; i++){
        for(j = i; j < pricesSize; j++){
            temp = prices[j] - prices[i];
            if(temp > max) max = temp;
        }
    }
    return max;
}


//#########################################################################################################################################################################
//C++ monotone pripority stack
class Solution{
    public :
        int maxProfit(vector<int>& prices){
            int ans = 0;
            vector<int> St;
            prices.emplace_back(-1);     //last place as sentry
            for(int i = 0; i < prices.size(); i++){
                while(!St.empty() && St.back() > prices[i]){      //maintenance monotone priority stack
                    ans = std::max(ans, St.back() - St.front());   //maintenance maximum value
                    St.pop_back();
                }
                St.emplace_back(prices[i]);
            }
            return ans;
        }
};


//#########################################################################################################################################################################
//C monotone priority stack
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//stack implement start
typedef struct Stack{
    int *array;
    int capacity;
    int top;
}Stack;

Stack *newStack(int capacity){
    Stack *stack = NULL;
    stack = (Stack *)calloc(sizeof(stack[0]), 1);
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
//end of stack implement

int maxProfit( int *prices, int pricesSize ) {
    int max = 0, i = 0, temp = 0;
    Stack *stack = NULL;

    if( !prices || pricesSize < 1 ) {
        return 0;
    }
    stack = newStack( 123456 );
    for( i = 0; i < pricesSize; ++i ) {
        while( !emptyStack( stack ) && peekStack( stack ) > prices[i] ) {
            temp = peekStack( stack ) - peekBottomStack( stack );
            max = MAX( max, temp );
            popStack( stack );
        }
        pushStack( stack, prices[i] );
    }
    while( sizeStack( stack ) > 1 ) {
        temp = peekStack( stack ) - peekBottomStack( stack );
        max = MAX( max, temp );
        popStack( stack );
    }
    delStack( &stack );

    return max;
}

int maxProfit(int *prices, int pricesSize){
    int ans, i;
    Stack *stack = NULL;
    stack = newStack(100010);
    for(i = 0; i < pricesSize; i++){
        if(emptyStack(stack)) pushStack(stack, prices[i]);
        else if(prices[i] < peekStack(stack)){
            while(prices[i] < peekStack(stack)) {
            ans = MAX(ans, popStack(stack) - peekHeadStack(stack));
            pushStack(stack, prices[i]);
            }
        }
        else if(prices[i] > peekStack(stack)) pushStack(stack, prices[i]);
    }
    return ans;
}


//#########################################################################################################################################################################
//monotone priority stack optimize
int max(int x, int y) {return x > y ? x : y;}

int maxProfit(int *prices, int pricesSize){
    int top = -1, ans = 0, i;
    for(i = 0; i < pricesSize; i++){
        while(top >= 0 && prices[top] > prices[i]){
            ans = max(ans, prices[top] - prices[0]);
            --top;
        }
        prices[++top] = prices[i];
    }
    //pop the reminder element
    while(top >= 0 && prices[top] > -1){
        ans = max(ans, peices[top] - prices[0]);
        --top;
    }
    return ans;
}


//#########################################################################################################################################################################
//DP solove              very motherfucker classic!!! holly shit!!!!!!! so fucking classic!!
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (a) : (b))

int maxProfit(int *prices, int pricesSize){
    int i = 0;
    int max = INT_MIN, min = INT_MAX;

    if(!prices || pricesSize < 1){
        return 0
    }
    for(i = 0; i < pricesSize; i++){
        min = MIN(min, prices[i]);//from begin to end, by order of date, find the lowest price before today which was selected at min. 
        max = MAX(max, prices[i] - min);//and every max corresponding to that day's max profit.
    }
    return max;
}


//#########################################################################################################################################################################
//java mononotion priority stack
public int maxProfit(int[] prices){
    int[] prices2 = new int[prices.length + 1];
    for(int i = 0; i < prices.length; i++){
        prices2[i] = prices[i];
    }
    prices2[prices.length] = -1;           //last place as sentry
    ArrayDeque<Integer> stack = new ArrayDeque<>();
    int ans = 0;
    for(int i = 0; i < prices2.length; i++){
        while(!stack.isEmpty() && stack.peek() >= prices2[i]){
            int top = stack.pop();
            if(stack.isEmpty()){
                continue;
            }
            ans = Math.max(ans, top - stack.peekLast());  //when the bigest element in stack pop out, subtract the top stack element
        }
        stack.push(price2[i]);
    }
    return ans;
}


//#########################################################################################################################################################################
