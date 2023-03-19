int findMin(int* nums, int numsSize){
    int min = nums[0], i;
    for(i = 1; i < numsSize; i++){
        if(nums[i] < nums[i - 1]) return nums[i];
    }
    return min;
}

int findMin(int* nums, int numsSize){
    int low = 0, high = numsSize - 1;
    while(low < high){
        int pivot = low + (low + high)/2;
        if(nums[pivote] < nums[high]) high = pivote;
        else if(nums[pivote] > nums[low]) low = pivote + 1;
        else high--;
    }
    return nums[pivote];
}