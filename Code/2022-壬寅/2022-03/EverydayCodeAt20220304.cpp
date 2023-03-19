//2022.03.04
//Every day Code
//Leetcode382
//Accomplishment:After think whit the answer, get my answer
//Notes:C-struct operation C-random algoritm

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

typedef struct {
    int * nums;
    int length;
} Solution;

Solution* solutionCreate(struct ListNode* head) {
    Solution * obj = (Solution *)malloc(sizeof(Solution));
    if(!obj) exit(0);
    int length = 0;
    struct ListNode * p = head;
    while(p){
        p = p -> next;
        length++;
    }
    obj -> length = length;
    obj -> nums = (int *)malloc(sizeof(int) * obj -> length);
    if(!obj -> nums) exit(0);
    p = head;
    int i = 0; 
    while(p){
        obj -> nums[i] = p -> val;
        i++;
        p = p -> next;
    }
    return obj;
}

int solutionGetRandom(Solution* obj) {
    return obj -> nums[rand() % obj -> length];
}

void solutionFree(Solution* obj) {
    free(obj -> nums);
    free(obj);
}

/**
 * Your Solution struct will be instantiated and called as such:
 * Solution* obj = solutionCreate(head);
 * int param_1 = solutionGetRandom(obj);
 
 * solutionFree(obj);
*/

//C-answer
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

typedef struct{
    int * arr;
    int length;
}Solution;

Solution * solutionCreate(struct ListNode *head){
    Solution * obj = (Solution *)malloc(sizeof(Solution));
    assert(obj != NULL);
    obj -> length = 0;
    struct ListNode * node = head;

    while(node){
        node = node -> next;
        obj -> length++;
    }
    obj -> arr = (int *)malloc(sizeof(int) * obj -> length);
    assert(obj -> != NULL);
    node = head;
    for(int i = 0; i < obj -> length; i++){
        obj -> arr[i] = node -> val;
        node = node -> next;
    }
    return obj;
}

int solutionGetRandom(Solution * obj){
    return obj -> arr[rand() % obj -> length];
}

void solutionFree(Solution * obj){
    free(obj -> arr);
    free(obj);
}

//C++
