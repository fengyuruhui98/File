int removeDuplicates(int* nums, int numsSize){
    int i, j;
    for(i = 1; i < numsSize; i++){
        if(nums[i] == nums[i-1]){
            for(j = i, j < numsSize; j++){
                nums[i-1] = nums[i];
            }
            numsSize--;
            i--;
        }
    }
    return numsSize;
}