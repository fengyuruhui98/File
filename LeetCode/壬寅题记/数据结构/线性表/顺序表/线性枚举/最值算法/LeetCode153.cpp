int findMin(int* nums, int numsSize){
    int min = nums[0], i;
    for(i = 1; i < numsSize; i++){
        if(nums[i] < nums[i - 1]) return nums[i];
    }
    return min;
}