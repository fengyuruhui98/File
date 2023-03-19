//2022.03.03
//Every day Code
//Leetcode1750
//Accomplishment：AC in one time
//Notes：C-string operation

int minimumLength(char * s){
    if(*(s + 1) == '\0' || (*(s + 2) == '\0' && (s[0] == s[1]))){
        return 1;
    }
    int i = 0, length = 0;
    while(s[i] != '\0'){    //son use the strlen can be much easier;
        length++;
        i++;
    }
    int left = 0, right = length - 1;
    while(s[left] == s[right]){
        while(s[left + 1] == s[left]){
            left++;
        }
        left++;
        if(left < right){
            while(s[right] == s[right - 1]){
                right--;
            }
            right--;
            if(left > right){
                return 0;
            }
            else if(left == right) return 1;
        }
        else return 0;
    }
    printf("\n%d--%d\n", left, right);
    return (right - left + 1);
}

//python-Greedy algorithms
class Solution:
    def minimumLength(self, s: str) -> int:
        L,R = 0, len(s) - 1
        while L < R:
            if s[L] != s[R]:
                break
            c = s[L]
            while L <= R and s[L] == c: #pointer point to nothing
                L += 1
            while L <= R and s[R] == c: #pointer point to nothing
                R -= 1
        return R - L + 1

//C two pointer
int minimumLength(char * s){
    int left = 0, right = strlen(s) - 1; //so fucking close!! so fucking close!!!!
    while( left < right ){
        if(s[left] != s[right]) break;
        while(s[left]==s[right] && left < right) left++;  //different way he/she is compareing it to a sentry, but I compare it with its next ones
        while(s[left-1] == s[right] && left <= right) right--;
    }
    return right-left+1;
}
