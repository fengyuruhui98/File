//2022.03.06
//Every day Code
//Leetcode1542
//Accomplishment:A big piece of shit
//Notes:C-status zip hash

#define MAX(a, b) (a > b ? a : b)

int longestAnswer(char * s){
    int length = strlen(s);
    int ans, i, j, status = 0;
    int perfix[200001];
    for(i = 0; i < 200001; i++){
        perfix[i] = -2;
    }
    perfix[100000] = -1;
    for(i = 0; i < length; i++){
        int digit = s[i] - '0';    //change char to int
        status ^= (1 << digit); 
        if(perfix[status + 100000] != -2){  //status plus 100000 is one status from 2^10 status, the one and only one status
                                            //and it is status mapping status + 100000, they have different meanings. 
                                            //status is the s[0...i] string's status. while the status + 100000 direct to somewhere like m
                                            //and the status of s[0...m].
            ans = MAX(ans, i - perifx[status + 100000]);
        }
        else perfix[status + 100000] = i;
        for(j = 0; j < 10; j++){
            if(perfix[(status ^ (1 << j) + 100000)] != -2){
                ans = MAX(ans, i - perfix[(status ^ (1 << j)) + 100000]);
            }
        }
    }
    
    return ans;
}

//C++
class Solution{
    public:
        int longestAwesome(string s){
            int perfix = 0;
            int ans = 0;
            unordered_map<int, int> hash = {{0, -1}};
            for(int i = 0; i < s.szie(); i++){
                int digit = s[i] - '0';
                perfix ^= (1 << digit);
                if(hash.count(perfix)){
                    ans = max(ans, i - hash[perfix]);
                }
                else hash[perfix] = i;
                for(int j = 0; j < 10; j++){
                    perfix ^= (1 << j);
                    if(hash.count(perfix)){
                        ans = max(ans, i - hash[perfix]);
                    }
                    perfix ^= (1 << j);
                }
            }
            return ans;
        }
}