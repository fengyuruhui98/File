//2022.03.03
//Every day Code
//Leetcode409
//Accomplishment：no AC today,hai
//Notes：C-string operation

//you even do not see the problem clear
#define MAX(a, b) a > b ? a : b

int longestPalindrome(char * s){
    int i, ans = 0, temp = 0;
    int flag[52];
    for(i = 0; i < 52; i++){
        flag[i] = -1;
    }
    for(i = 0; i < strlen(s); i++){
        flag[(int)(s[i] - 'A')] ^= 1;
    }
    for(i = 0; i < 52; i++){
        printf("%d", flag[i]);
    }
    printf("\n");
    for(i = 0; i < 51; i++){
               printf("%d", flag[i]);
        if(flag[i] == 0 && flag[i + 1] == 0) temp++;
        else {
            ans = MAX(ans, temp);
            temp = 0;
        }
    }
    return ans * 2 + 1;
}

//C
int longestPalindrome(char * s){
    int c[124] = {0}, num = 0, i;
    for(i = 0; i < strlen(s); i++) c[s[i]]++;  //flag can be longer than 52
    for(i = 0; i < 124; i++) num += c[i] - c[i] % 2;    //different from another problem
                                                    //and c[i] - c[i]%2
    return num + (num != strlen(s));
}