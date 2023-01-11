# DFS枚举

>目录

* [一、组合型枚举](#一组合型枚举)
* [二、排列组合型枚举模板](#二排列组合型枚举模板)
    * [不可重复排列](#枚举不可重复排列)
    * [可重复排列](#枚举可重复排列)
    * [组合](#枚举组合)
* [三、求和型枚举](#三求和)

# 一、排列组合型枚举
从 1∼n 这 n 个整数中随机选出 m个，输出所有可能的选择方案。

## 输入格式
两个整数 n,m,在同一行用空格隔开。

## 输出格式
按照从小到大的顺序输出所有方案，每行 1个。首先，同一行内的数升序排列，相邻两个数用一个空格隔开。其次，对于两个不同的行，对应下标的数一一比较，字典序较小的排在前面（例如 1 3 5 7 排在 1 3 6 8 前面）。

## 数据范围
n > 0  
0 ≤ m ≤ n   
n + (n − m) ≤ 25

## 输入样例：
5 3
## 输出样例：
1 2 3   
1 2 4   
1 2 5   
1 3 4   
1 3 5   
1 4 5   
2 3 4   
2 3 5   
2 4 5   
3 4 5   

n = 5, m = 3
```c
#include<bits/stdc++.h>
using namespace std;
const int N = 50;
int m, n;
int arr[N];
void dfs(int pos, int stx,int tar) {//当前第几个位置需要填，当前枚举到第几位，从第几位开始选
	if (pos == tar+1) {
		for (int i = 1;i <= tar;i++) {
			cout << arr[i] << ' ';
		}
		cout << endl;
		return;
	}
	for (int i = stx; i <= n;i++) {
		arr[pos] = i;
		dfs(pos + 1, i + 1, tar);
	}
}
int main() {
	cin >> n >> m;
	dfs(1,1,m);
}
```

# 二、排列组合型枚举模板

## 枚举可重复排列
```c
#include<iostream>
using namespace std;
int n,m,ans[15];
int a[15];//待排列的数存储在此
bool vis[15];
void dfs(int cnt)//按字典序输出n个数选m个数的所有排列
{
    if(cnt==m)
    {
        for(int i=0;i<m;i++) cout<<ans[i]<<" ";
        cout<<endl;
        return ;
    }
    for(int i=0;i<n;i++)
    {
        ans[cnt]=a[i];
        dfs(cnt+1);
    }
}
int main()
{
    while(cin>>n>>m)
    {
        fill(vis,vis+15,0);
        for(int i=0;i<n;i++) cin>>a[i];
        dfs(0);
    }
    return 0;
}
```
## 枚举不可重复排列

加标记数组即可
```c
#include<iostream>
using namespace std;
int n,m,ans[15];
int a[15];//待排列的数存储在此
bool vis[15];
void dfs(int d,int cnt)//按字典序输出n个数选m个数的所有排列
{
    if(cnt==m)
    {
        for(int i=0;i<m;i++) cout<<ans[i]<<" ";
        cout<<endl;
        return ;
    }
    for(int i=0;i<n;i++)
    {
        if(!vis[i])
        {
            ans[cnt]=a[i];
            vis[i]=1;
            dfs(i+1,cnt+1);
            vis[i]=0;
        }
    }
}
int main()
{
    while(cin>>n>>m)
    {
        fill(vis,vis+15,0);
        for(int i=0;i<n;i++) cin>>a[i];
        dfs(0,0);
    }
    return 0;
}
```

## 枚举组合
```c
#include<iostream>
using namespace std;
int n,m,ans[15];
int a[15];//待排列的数存储在此
bool vis[15];
void dfs(int d,int cnt)//按字典序输出n个数选m个数的所有组合
{
    if(cnt==m)
    {
        for(int i=0;i<m;i++) cout<<ans[i]<<" ";
        cout<<endl;
        return ;
    }
    for(int i=d;i<n;i++)
    {
        ans[cnt]=a[i];
        dfs(i+1,cnt+1);
    }
}
int main()
{
    while(cin>>n>>m)
    {
        fill(vis,vis+15,0);
        for(int i=0;i<n;i++) cin>>a[i];
        dfs(0,0);
    }
    return 0;
}
```

# 三、求和型枚举

深度优先搜索(DFS,Depth-First Search)是搜索手段之一。它从某个状态开始，不断的转移状态知道无法转移，然后退回到前一步的状态，继续转移到其他状态，如此不断重复，直到找到最终的解。

问题给定整数a1,a2...an,判断是否可以从中选出若干数，使它们的和恰好为k。

 1 <= n <= 20

-1e8 <= ai <= 10e8

-1e8 <= k <= 1e8

输入：

 n = 4 k = 13

a={1 2 4 7}

输出：

Yes (13 = 2 + 4 + 7)

输入：

n=4 k=15

a={1 2 4 7}

输出：

No

枚举每一种情况复杂度为O(2^n)。利用dfs搜索可以枚举每一种情况代码如下：
```c
#include <iostream>
#include<stack>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include <math.h>
using namespace std;
#define pi acos(-1)
const int N=1e5;
int n,a[N],ans[N],k;
int h=0;
bool dfs(int i,int sum)//ｉ代表第ｉ个数（从０开始）
{
    if(i==n)  return sum==k;
    if(dfs(i+1,sum+a[i]))　//加入a[i]往下递归
    {
        ans[h++]=a[i];
        return true;
    }
    if(dfs(i+1,sum))//不加a[i]往下递归　　可以看成是加不加a[i]的问题（包括了所有状态）
    {
        return true;
    }
    return false;
}
int main()
{
    int i,j;
    while(scanf("%d%d",&n,&k)!=EOF)
    {
        h=0;
        for(i=0; i<n; i++)
            scanf("%d",&a[i]);
        if(dfs(0,0))
        {
            printf("Yes (%d =",k);
            for(i=0; i<h; i++)
            {
                printf(" %d",ans[i]);
                if(i!=h-1)
                    printf(" +");
            }
            printf(")\n");
        }
        else
            printf("No\n");
    }
    return 0;
}
```c