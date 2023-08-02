//2022.06.13
//Every day Code
//Leetcode5
//Accomplishment:A big piece of shit,so fucking shit

char * longestPalindrome(char * s){
    int ansnum = 0, left, right, i;
    int length = strlen(s);
    int ansleft;
    char * ans = (char *)malloc(sizeof(char) * length);
    for(i = 0; i < length; i++){
        left = i;
        right = i;
        int temp = 1;
        while((--left >= 0) && (++right < length) && (s[left] == s[right])) {
            temp += 2;
            ansleft = left;
        }
        if(temp > ansnum){
            ansnum = temp;
            memset(ans, 0, length);
            memcpy(ans, &s[ansleft], right - left);
        }
    } 
    return ans;
}

char * longestPalindrome(char *s){
    int i, j;
    int length = strlen(s);
    if(length < 2) return s;
    char * ans = (char *)malloc(sizeof(char) * length);
    int ** dp = (int **)malloc(sizeof(int *) * (length + 1));
    for(i = 0; i < length + 1; i++) dp[i] = (int *)malloc(sizeof(int) * (length + 1));
    for(i = 0; i < length + 1; i++)
        for(j = 0; j < length + 1; i++) dp[i][j] = 0;
    
}