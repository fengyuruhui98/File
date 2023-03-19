int cmp(const void * a, const void * b){
    int *a = (int *)a;
    int *b = (int *)b;
    return *a - *b;
}

bool containsDuplicate(int* nums, int numsSize){
    bool ret;
    int i, temp = nums[0];
    qsort(nums, numsSize, sizeof(int), cmp);
    for(i = 1; i < numsSize; i++){
        if(nums[i] == nums[i - 1]) return false;
    }
    return true;
}


//#########################################################################################################################################################################
//official C qsort
int cmp(const void * _a, const void * _b){
    int a = *(int *)_a, b = *(int *)_b;
    return a - b;
}

bool containsDuplicate(int* nums, int numsSize){
    qsort(nums, numsSize, sizeof(int), cmp);
    for(int i = 0; i < numsSize - 1; i++){
        if(nums[i] == nums[i + 1]){                 //one way to optimize is change == into ^ and judge the result
            return true;
        }
    }
    return false;
}


//#########################################################################################################################################################################
//Hash table C
struct hashTable{
    int key;
    UT_hash_handle hh;
};

bool containsDuplicate(int* nums, int numsSize){
    struct hashTable * set = NULL;
    for(int i = 0; i < numsSize; i++){
        struct hashTable* tmp;
        HASH_FIND_INT(set, nums + i; tmp);
        if(tmp == NULL){
            tmp = malloc(sizeof(struct hashTable));
            tmp->key = nums[i];
            HASH_ADD_INT(set, key, tmp);
        }else{
            return true;
        }
    }
    return false;
}


//#########################################################################################################################################################################
//hash table C++
class Solution{
    public:
        bool containsDuplicate(vector<int>& nums){
            unordered_set<int> s;
            for(int x: nums){
                if(s.find(x) != s.send()){
                    return true;
                }
                s.insert(x);
            }
            return false;
        }
};


//#########################################################################################################################################################################
//so easy java & python
class Solution{
    public boolean containsDuplicate(int[] nums){
        return IntStream.of(nums).distinct().count() != nums.length;
    }
}

//python
class Solution:
    def containsDuplicate(self, nums: List[int]) -> bool:
        return len(set(nums)) != len(nums)