#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) > (b) ? (a) : (b))

int maximumProduct(int* nums, int numsSize){
	long max[3] = {INT_MIN, INT_MIN, INT_MIN}, i;
	long min[2] = {INT_MAX, INT_MAX};
	for(i = 0; i < numsSize; i++){
		if(nums[i] > max[0]){
			max[2] = max[1];
			max[1] = max[0];
			max[0] = nums[i];
		}
		else if(nums[i] > max[1]){
			max[2] = max[1];
			max[1] = nums[i];
		}
		else if(nums[i] > max[2]) max[2] = nums[i];

		if(nums[i] < min[0]){
			min[1] = min[0];
			min[0] = nums[i];
		}
		else if(nums[i] < min[1]){
			min[1] = nums[i];
		}
	} 
	return fmax(min[0]*min[1]*max[0], max[0]*max[1]*max[2]);
}





//#########################################################################################################################################################################
//motherfucker!!!!!!!!!
int maximumProduct(int* nums, int numsSize) {
    int min1 = INT_MAX, min2 = INT_MAX;
    int max1 = INT_MIN, max2 = INT_MIN, max3 = INT_MIN;

    for (int i = 0; i < numsSize; i++) {
        int x = nums[i];
        if (x < min1) {
            min2 = min1;
            min1 = x;
        } else if (x < min2) {
            min2 = x;
        }

        if (x > max1) {
            max3 = max2;
            max2 = max1;
            max1 = x;
        } else if (x > max2) {
            max3 = max2;
            max2 = x;
        } else if (x > max3) {
            max3 = x;
        }
    }

    return fmax(min1 * min2 * max1, max1 * max2 * max3);
}
