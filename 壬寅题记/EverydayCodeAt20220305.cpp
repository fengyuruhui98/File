//2022.03.05
//Every day Code
//Leetcode673
//Accomplishment:piece of shit
//Notes:DP-LIS

int findNumberOfLIS(int* nums, int numsSize){
    int i = 0, j = 0, ans = 1, count = 0;
    int * dp = (int *)malloc(sizeof(int) * (numsSize + 1));
    int * cnt = (int *)malloc(sizeof(int) * (numsSize + 1));
    dp[0] = 1;
    cnt[0] = 1;
    for(i = 1; i < numsSize; i++){
        dp[i] = 1;
        cnt[i] = 1;
        for(j; j < i; j++){//so fucking close !! fuck you, piece of shit, motherfucker!!!   j = 1;every i change j does not change
            if(nums[j] < nums[i]){        
                if(dp[j] + 1 > dp[i]){
                    dp[i] = dp[j] + 1;
                    cnt[i] = cnt[j];
                }
                else if(dp[j] + 1 == dp[i])
                    cnt[i] += cnt[j];
            }ans = (ans > dp[i] ? ans : dp[i]); 
        }
    }
    for(i = 0;i < numsSize; i++){
        if(dp[i] == ans){
            count += cnt[i];
        }
    }
    return count;
}