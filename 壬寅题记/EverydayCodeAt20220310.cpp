//2022.03.03
//Every day Code
//Leetcode1752
//Accomplishment：AC easy problem
//Notes：nothing to say about

bool check(int* nums, int numsSize){
    int i = 0, flag, ans = 0;
    flag = nums[0];
    while(i < numsSize && (flag < nums[i] || ans != 2)){
        if(flag > nums[i]) ans ++;
        flag = nums[i++];
    } 
    if(ans == 0) return true;
    else if(ans == 2 || nums[0] < nums[numsSize - 1]) return false;
    else return true;
}

//so fucking awsome code
bool check(int* nums, int numsSize){
    int i;
    int cnt = 0;
    /* 遍历整个数组, 后一个数小于前一个数的次数不能超过1 */
    for (i = 1; i <= numsSize; i++) {
        if (nums[i % numsSize] < nums[i - 1] && ++cnt > 1) {    //firstly see the  && before if true then cnt+1 and see if cnt > 1
            return false;                                       //when get to the end it get more than numsSIze to see if the last 
                                                                //is smaller than the first.
        }
    }
    return true;
}
