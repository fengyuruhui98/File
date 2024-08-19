# LeetCode-2940. 找到 Alice 和 Bob 可以相遇的建筑
2024.08.10  
难度：困难    
给你一个下标从 0 开始的正整数数组 heights ，其中 heights[i] 表示第 i 栋建筑的高度。

如果一个人在建筑 i ，且存在 i < j 的建筑 j 满足 heights[i] < heights[j] ，那么这个人可以移动到建筑 j 。

给你另外一个数组 queries ，其中 queries[i] = [ai, bi] 。第 i 个查询中，Alice 在建筑 ai ，Bob 在建筑 bi 。

请你能返回一个数组 ans ，其中 ans[i] 是第 i 个查询中，Alice 和 Bob 可以相遇的 最左边的建筑 。如果对于查询 i ，Alice 和 Bob 不能相遇，令 ans[i] 为 -1 。

 

>示例 1：  
输入：heights = [6,4,8,5,2,7], queries =[[0,1],[0,3],[2,4],[3,4],[2,2]]
输出：[2,5,-1,5,2]
解释：第一个查询中，Alice 和 Bob 可以移动到建筑 2 ，因为 heights[0] < heights[2] 且 heights[1] < heights[2] 。
第二个查询中，Alice 和 Bob 可以移动到建筑 5 ，因为 heights[0] < heights[5] 且 heights[3] < heights[5] 。
第三个查询中，Alice 无法与 Bob 相遇，因为 Alice 不能移动到任何其他建筑。
第四个查询中，Alice 和 Bob 可以移动到建筑 5 ，因为 heights[3] < heights[5] 且 heights[4] < heights[5] 。
第五个查询中，Alice 和 Bob 已经在同一栋建筑中。
对于 ans[i] != -1 ，ans[i] 是 Alice 和 Bob 可以相遇的建筑中最左边建筑的下标。
对于 ans[i] == -1 ，不存在 Alice 和 Bob 可以相遇的建筑。 
 

>提示：  
1 <= heights.length <= 5 * 104
1 <= heights[i] <= 109
1 <= queries.length <= 5 * 104
queries[i] = [ai, bi]
0 <= ai, bi <= heights.length - 1 

# 解

方法一：线段树
思路与算法

由题意可知，人只能往右边移动，并且两人交换位置后不影响答案。对于每一次询问不妨设 $a_i<b_i$。如果 $heights[a_i]<heights[b_i]$，那么答案就是$b_i$，否则答案在$b_i$右边。
令 hights 的长度为 n，问题转化为在区间$[b_i+1,n]$中找到最左边的下标 x 满足$heights[x]>heights[b_i]$，对于此问题用线段树求解即可。

```c++
class Solution {
public:
    vector<int> d;//线段树
    //建树
    void build(int l, int r, int rt, vector<int> &heights){
        if(l == r){
            d[rt] = heights[l - 1];
            return;
        }
        int mid = (l + r) >> 1;
        build(l, mid, rt << 1, heights);
        build(mid + 1, r, (rt << 1) | 1, heights);
        d[rt] = max(d[rt << 1], d[rt << 1 | 1]);
    }
    //查询
    int query(int pos, int val, int l, int r, int rt){
        if(val >= d[rt]) return 0;
        if(l == r) return l;
        int mid = (l + r) >> 1;
        if(pos <= mid){
            int res = query(pos, val, l, mid, rt << 1);
            if(res != 0) return res;
        }
        return query(pos, val, mid + 1, r, rt << 1 |1);
    }
    //调用
    vector<int> leftmostBuildingQueries(vector<int>& heights, vector<vector<int>>& queries) {
        int n = heights.size();
        d.resize(n * 4);
        build(1, n, 1, heights);

        int m = queries.size();
        vector<int> ans(m);
        for(int i = 0; i < queries.size(); ++i){
            int a = queries[i][0], b = queries[i][1];
           if( a > b)swap(a, b);
           if(a == b || heights[a] < heights[b]){
            ans[i] = b;
            continue;
           }
           ans[i] = query(b + 1, heights[a], 1, n, 1) - 1;
        }
        return ans;
    }
};
```