//2022.03.21
//Every day Code
//Leetcode653
//Accomplishment：Wrong way's wrong code
//Notes：C-hash and binary tree


//wrong way's wrong code
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

#define MAXN 100000

bool findo(struct TreeNode* root, int k){
    if(root -> val == k)return true;
    else if(root -> left && root -> val > k) findo(root -> left, k);
    else if(root -> right && root -> val < k) findo(root -> right, k);
    return false;
}

bool findTarget(struct TreeNode* root, int k){
    int temp[MAXN] = {0};
    int left = 0, right = 0;
    temp[0] = root -> val;
    right++; 
    while(root -> left || root -> right || left != right){
        if(findo(root, k - root -> val)) return true;
        left++;
        if(root -> left){
            temp[right] = root -> left ->val;
            right++;
        }
        if(root -> right){
            temp[right] = root -> right -> val;
            right++;
        }
    }
    return false;
}

//dfs and hash table----------------------------------1.C

typedef struct {
    int key;
    UT_hash_handle hh;
} HashItem;

bool helper(const struct TreeNode* root, int k, HashItem ** hashTable) {
    if (root == NULL) {
        return false;
    }
    int key = k - root->val;
    HashItem * pEntry = NULL;
    HASH_FIND_INT(*hashTable, &key, pEntry);
    if (pEntry != NULL) {
        return true;
    }
    pEntry = (HashItem *)malloc(sizeof(HashItem));
    pEntry->key = root->val;
    HASH_ADD_INT(*hashTable, key, pEntry);
    return helper(root->left, k, hashTable) || helper(root->right, k, hashTable);
}

bool findTarget(struct TreeNode* root, int k){
    HashItem * hashTable = NULL;
    return helper(root, k, &hashTable);
}

//C++ version----------------------------------1.Cpp
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
//C++ version
class Solution {
public:
    unordered_set<int> hashTable;

    bool findTarget(TreeNode *root, int k) {
        if (root == nullptr) {
            return false;
        }
        if (hashTable.count(k - root->val)) {
            return true;
        }
        hashTable.insert(root->val);
        return findTarget(root->left, k) || findTarget(root->right, k);
    }
};

//python version----------------------------------1.py
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right
//python version
class Solution:
    def __init__(self):
        self.s = set()

    def findTarget(self, root: Optional[TreeNode], k: int) -> bool:
        if root is None:
            return False
        if k - root.val in self.s:
            return True
        self.s.add(root.val)
        return self.findTarget(root.left, k) or self.findTarget(root.right, k)

//bfs and hash table----------------------------------2.C

#define MAX_NODE_SIZE 1e4

typedef struct {
    int key;
    UT_hash_handle hh;
} HashItem;

bool findTarget(struct TreeNode* root, int k){
    HashItem * hashTable = NULL;
    struct TreeNode ** que = (struct TreeNode **)malloc(sizeof(struct TreeNode *) * MAX_NODE_SIZE);
    int head = 0, tail = 0;
    que[tail++] = root;
    while (head != tail) {
        struct TreeNode *node = que[head++];
        int key = k - node->val;
        HashItem * pEntry = NULL;
        HASH_FIND_INT(hashTable, &key, pEntry);
        if (pEntry != NULL) {
            return true;
        }
        pEntry = (HashItem *)malloc(sizeof(HashItem));
        pEntry->key = node->val;
        HASH_ADD_INT(hashTable, key, pEntry);
        if (node->left != NULL) {
            que[tail++] = node->left;
        }
        if (node->right != NULL) {
            que[tail++] = node->right;
        }
    }
    HashItem * curr = NULL, * next = NULL;
    HASH_ITER(hh, hashTable, curr, next) {
        HASH_DEL(hashTable, curr); 
        free(curr);           
    }
    return false;
}

//C++----------------------------------2.cpp
class Solution {
public:
    bool findTarget(TreeNode *root, int k) {
        unordered_set<int> hashTable;
        queue<TreeNode *> que;
        que.push(root);
        while (!que.empty()) {
            TreeNode *node = que.front();
            que.pop();
            if (hashTable.count(k - node->val)) {
                return true;
            }
            hashTable.insert(node->val);
            if (node->left != nullptr) {
                que.push(node->left);
            }
            if (node->right != nullptr) {
                que.push(node->right);
            }
        }
        return false;
    }
};

//python----------------------------------2.py
class Solution:
    def findTarget(self, root: Optional[TreeNode], k: int) -> bool:
        s = set()
        q = deque([root])
        while q:
            node = q.popleft()
            if k - node.val in s:
                return True
            s.add(node.val)
            if node.left:
                q.append(node.left)
            if node.right:
                q.append(node.right)
        return False

//dfs + iterate in the middle order + double pointer----------------------------------3.c
#define MAX_NODE_SIZE 1e4

void inorderTraversal(const struct TreeNode* node, int* vec, int* pos) {
    if (node == NULL) {
        return;
    }
    inorderTraversal(node->left, vec, pos);
    vec[(*pos)++] = node->val;
    inorderTraversal(node->right, vec, pos);
}

bool findTarget(struct TreeNode* root, int k) {
    int * vec = (int *)malloc(sizeof(int) * MAX_NODE_SIZE);
    int pos = 0;
    inorderTraversal(root, vec, &pos);
    int left = 0, right = pos - 1;
    while (left < right) {
        if (vec[left] + vec[right] == k) {
            return true;
        }
        if (vec[left] + vec[right] < k) {
            left++;
        } else {
            right--;
        }
    }
    free(vec);
    return false;
}

//C++----------------------------------3.cpp
class Solution {
public:
    vector<int> vec;

    void inorderTraversal(TreeNode *node) {
        if (node == nullptr) {
            return;
        }
        inorderTraversal(node->left);
        vec.push_back(node->val);
        inorderTraversal(node->right);
    }

    bool findTarget(TreeNode *root, int k) {
        inorderTraversal(root);
        int left = 0, right = vec.size() - 1;
        while (left < right) {
            if (vec[left] + vec[right] == k) {
                return true;
            }
            if (vec[left] + vec[right] < k) {
                left++;
            } else {
                right--;
            }
        }
        return false;
    }
};

//python----------------------------------3.py
class Solution:
    def findTarget(self, root: Optional[TreeNode], k: int) -> bool:
        arr = []
        def inorderTraversal(node: Optional[TreeNode]) -> None:
            if node:
                inorderTraversal(node.left)
                arr.append(node.val)
                inorderTraversal(node.right)
        inorderTraversal(root)

        left, right = 0, len(arr) - 1
        while left < right:
            sum = arr[left] + arr[right]
            if sum == k:
                return True
            if sum < k:
                left += 1
            else:
                right -= 1
        return False

//iterate + iterate in the middle order + double pointer----------------------------------4.c
#define MAX_NODE_SIZE 1e4 

typedef struct {
    struct TreeNode ** stBuf;
    int stTop;
    int stSize;
} Stack;

void init(Stack* obj, int stSize) {
    obj->stBuf = (struct TreeNode **)malloc(sizeof(struct TreeNode *) * stSize);
    obj->stTop = 0;
    obj->stSize = stSize;
}

bool isEmpty(const Stack* obj) {
    return obj->stTop == 0;
}

struct TreeNode * top(const Stack* obj) {
    return obj->stBuf[obj->stTop - 1];
}

bool push(Stack * obj, struct TreeNode* val) {
    if(obj->stTop == obj->stSize) {
        return false;
    }
    obj->stBuf[obj->stTop++] = val;
    return true;
}

void freeStack(Stack * obj) {
    free(obj->stBuf);
}

struct TreeNode * pop(Stack* obj) {
    if(obj->stTop == 0) {
        return NULL;
    }
    struct TreeNode *res = obj->stBuf[obj->stTop - 1];
    obj->stTop--;
    return res;
}

struct TreeNode *getLeft(Stack* stk) {
    struct TreeNode *root = pop(stk);
    struct TreeNode *node = root->right;
    while (node != NULL) {
        push(stk, node);
        node = node->left;
    }
    return root;
}

struct TreeNode *getRight(Stack* stk) {
    struct TreeNode *root = pop(stk);
    struct TreeNode *node = root->left;
    while (node != NULL) {
        push(stk, node);
        node = node->right;
    }
    return root;
}

bool findTarget(struct TreeNode* root, int k){
    struct TreeNode *left = root, *right = root;
    Stack leftStack, rightStack;
    init(&leftStack, MAX_NODE_SIZE);
    init(&rightStack, MAX_NODE_SIZE);
    push(&leftStack, left);
    while (left->left != NULL) {
        push(&leftStack, left->left);
        left = left->left;
    }
    push(&rightStack, right);
    while (right->right != NULL) {
        push(&rightStack, right->right);
        right = right->right;
    }
    while (left != right) {
        if (left->val + right->val == k) {
            freeStack(&leftStack);
            freeStack(&rightStack);
            return true;
        }
        if (left->val + right->val < k) {
            left = getLeft(&leftStack);
        } else {
            right = getRight(&rightStack);
        }
    }
    freeStack(&leftStack);
    freeStack(&rightStack);
    return false;
}

//C++----------------------------------4.cpp
class Solution {
public:
    TreeNode *getLeft(stack<TreeNode *> &stk) {
        TreeNode *root = stk.top();
        stk.pop();
        TreeNode *node = root->right;
        while (node != nullptr) {
            stk.push(node);
            node = node->left;
        }
        return root;
    }

    TreeNode *getRight(stack<TreeNode *> &stk) {
        TreeNode *root = stk.top();
        stk.pop();
        TreeNode *node = root->left;
        while (node != nullptr) {
            stk.push(node);
            node = node->right;
        }
        return root;
    }

    bool findTarget(TreeNode *root, int k) {
        TreeNode *left = root, *right = root;
        stack<TreeNode *> leftStack, rightStack;
        leftStack.push(left);
        while (left->left != nullptr) {
            leftStack.push(left->left);
            left = left->left;
        }
        rightStack.push(right);
        while (right->right != nullptr) {
            rightStack.push(right->right);
            right = right->right;
        }
        while (left != right) {
            if (left->val + right->val == k) {
                return true;
            }
            if (left->val + right->val < k) {
                left = getLeft(leftStack);
            } else {
                right = getRight(rightStack);
            }
        }
        return false;
    }
};

//python----------------------------------4.py
class Solution:
    def findTarget(self, root: Optional[TreeNode], k: int) -> bool:
        left, right = root, root
        leftStk, rightStk = [left], [right]
        while left.left:
            left = left.left
            leftStk.append(left)
        while right.right:
            right = right.right
            rightStk.append(right)
        while left != right:
            sum = left.val + right.val
            if sum == k:
                return True
            if sum < k:
                left = leftStk.pop()
                node = left.right
                while node:
                    leftStk.append(node)
                    node = node.left
            else:
                right = rightStk.pop()
                node = right.left
                while node:
                    rightStk.append(node)
                    node = node.right
        return False
