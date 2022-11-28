int findMaxConsecutiveOnes(int* nums, int numsSize){
    int num = 0, temp = 0, i;
    for(i = 0; i < numsSize; i++){
        if(nums[i] == 0){
            if(num < temp){
                num = temp;
            }
            temp = 0;
        }
        else temp++;
    }
    if(num < temp) num = temp;
    return num;
}