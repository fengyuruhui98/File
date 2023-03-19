int thirdMax(int* nums, int numsSize){
    if(numsSize < 3){
        if(numsSize == 2){
            if(nums[0] < nums[1]) return nums[1];
            return nums[0];
        }
        if(numsSize == 1) return nums[0];
    }
    long ma = -2147483649, mb = -2147483649, mc = -2147483649, i;
    for(i = 0; i < numsSize; i++){
        if(nums[i] == ma || nums[i] == mb || nums[i] == mc){
            continue;
        }
        if(nums[i] > ma){
            mc = mb;
            mb = ma;
            ma = nums[i];
        }
        else if(nums[i] > mb){
            mc = mb;
            mb = nums[i];
        }
        else if(nums[i] > mc) mc = nums[i];
    }
    if(mc == -2147483649) return ma;
    return mc;
}                       //motherfucker    [1,2,-2147483648]