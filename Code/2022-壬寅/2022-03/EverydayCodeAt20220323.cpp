//2022.03.23
//Every day Code
//Leetcode440
//Accomplishment: find the problem: 1. langue fucking use bad 
                                  //2. lake of awareness of fucking automatic
//Notes：C dfs and bfs

//stupid C
int getlongsize(int n){
    int size = 1;
    while(n >= 10){
        n /= 10;
        size++;
    }
    return size;
}

int gethead(int n, int headlong, int size){
    int ret;
    int temp = pow(10 , (size - headlong));
    ret = n / temp;
    //printf("\ntemp:%d-ret:%d", temp, ret);
    return ret;
}

int getkfrombef(int headsoild, int k, int size){
    int ansbef = k / headsoild + 1;
    int ansaft = k % headsoild;

}

int findKthNumber(int n, int k){
    int size = getlongsize(n);
    long i, bef = 0, headsoild = 0, ans = 0;
    for(i = 1; i <= size; i++){
        //printf("\ni:%d\nsize:%d", i, size);
        int head = gethead(n, i, size);
        int base = pow(10, (i - 1));
        headsoild += base;
        //printf("\nbase:%d", head - base);
        bef += head - base + 1;
    }
    if(k <= bef) ans = getkfrombef(headsoild, k, size);
    else ans = gerkfromaft(n, k);
}

//fucking awsome py code
class Solution:
    def findKthNumber(self, n: int, k: int) -> int:
        # 小于等于n的以1开头的数有多少个?
        # 1 10-19 100-199 1000-1999 = 1111
        def dfs(l, r):
            # 当前层有 r - l + 1 个节点可取，递归到下一层。
            # l * 10： 从10变成100， r * 10 + 9: 从19变成199
            return 0 if l > n else min(n, r) - l + 1 + dfs(l * 10, r * 10 + 9)
        
        cur = 1
        k -= 1
        while k:
            cnts = dfs(cur, cur)
            # 当前节点中总数都小于需要的数，可以全部取走，bfs到同层下一点 (比如 1 -> 2)
            if cnts <= k:
                k -= cnts
                cur += 1
            # 答案在当前节点的子节点中，取走当前根节点，dfs向下 (比如 1 -> 10)
            else:
                k -= 1
                cur *= 10
        return cur

//fucking C++ code 
class Solution {
public:
    int getSteps(int curr, long n) {
        int steps = 0;
        long first = curr;
        long last = curr;
        while (first <= n) {
            steps += min(last, n) - first + 1;
            first = first * 10;
            last = last * 10 + 9;
        }
        return steps;
    }

    int findKthNumber(int n, int k) {
        int curr = 1;
        k--;
        while (k > 0) {
            int steps = getSteps(curr, n);
            if (steps <= k) {
                k -= steps;
                curr++;
            } else {
                curr = curr*10;
                k--;
            }
        }
        return curr;
    }
};
