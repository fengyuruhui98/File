# 最短路算法

Floyd 算法又称为插点法，是一种用于寻找给定的加权图中多源点之间最短路径的算法。

其最大特点是可以计算出现负边权时的最短路，实际应用中，很多题目不是问如何用 Floyd 求最短路，而是用 Floyd 的动态规划思想来解决类似 Floyd 的问题。

其时间复杂度是 O(N*N*N)，N是顶点数。

## 【极大值的选择】
设置无穷大时，0x7fffffff 是 32-bit int 的最大值，如果这个无穷大只用于一般的比较，那么 0x7fffffff 是一个完美的选择，但在更多情况下，其并不是一个好的选择。

在最短路的松弛操作时，如果 u、v 间无边，那么 w[u][v]=INF，此时若 INF 取 0x7fffffff，那么 dis[u]+w[u][v] 会溢出而变成负数，此时松弛操作便会出错，准确来说，0x7fffffff 不能满足无穷大加一个有穷的数依然是无穷大，而是变成了一个很小的负数。
```c
if(dis[u]+w[u][v]<dis[v]) 
    dis[v]=dis[u]+w[u][v];
```
由于要找一个能够满足无穷大加无穷大依然是无穷大的数，因此，可以选用 0x3f3f3f3f

0x3f3f3f3f 的十进制是 1061109567，是 $10^9$ 级别的，与 0x7fffffff 一个数量级，而一般场合下的数据都是小于$10^9$的，所以它可以作为无穷大使用而不致出现数据大于无穷大的情形。 

另一方面，由于一般的数据都不会大于 $10^9$，所以当我们把无穷大加上一个数据时，它并不会溢出，事实上 0x3f3f3f3f + 0x3f3f3f3f = 2122219134，这个数虽然非常大但却没有超过 32-bit int 的表示范围，因此 0x3f3f3f3f 还满足了无穷大加无穷大还是无穷大的需求。

此外，当想将某个数组清零或全部赋值为 -1，通常会使用 memset() 函数，但是当想将某个数组全部赋值为无穷大时，就不能使用memset 函数而是写循环了，因为 memset 是按字节操作的，它能够对数组清零是因为 0 的每个字节都是 0。但如果将无穷大设为 0x3f3f3f3f，由于其每个字节都是 0x3f，因此可以直接使用 memset() 函数来操作。

## 【算法核心】
1.初始化：

设 dis[i][j] 为 i、j 两点的距离，w[i][j] 为 i、j 两点的权值。

若点 u、v 有边连接，则：dis[u][v] = w[u][v]，即：初始化两点最短距离为两点权值。

若点 u、v 无边连接，则：dis[u][v] = 0x3f3f3f3f，即：初始化为一极大值。

2.算法主体
```c
for(int k=1;k<=n;k++)//第一重循环为i→j的中间点k
    for(int i=1;i<=n;i++)//第二重循环为起点i
        for(int j=1;j<=n;j++)//第三重循环为终点j
            if(dis[i][j]>dis[i][k]+dis[k][j])//如果i→k的距离加上k→j的距离小于i→j的距离
                dis[i][j]=dis[i][k]+dis[k][j];//更新最短路径
```
3.算法结束：dis[i][j] 即为 i→j 的最短路径。

## 【模版】
```c
int G[N][N];
int path[N][N];//path[i][j]=x表示i到j的路径上除i外的第一个点是x
void init(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == j)
                G[i][j] = 0;
            else
                G[i][j] = INF;
            path[i][j] = j;
        }
    }
}
void floyd(int n) {
    for (int k = 1; k <= n; k++) {         //枚举中间点
        for (int i = 1; i <= n; i++) {     //枚举起点
            for (int j = 1; j <= n; j++) { //枚举终点
                if (G[i][k] < INF && G[k][j] < INF) {
                    if (G[i][j] > G[i][k] + G[k][j]) { //松弛操作
                        G[i][j] = G[i][k] + G[k][j];
                        path[i][j] = path[i][k]; //更新路径
                    } 
                    else if (
                        G[i][j] == G[i][k] + G[k][j] && path[i][j] > path[i][k]) { //在最短路相同的情况下，更新字典序最小的路径
                        path[i][j] = path[i][k]; //最终path中存的是字典序最小的路径
                    }
                }
            }
        }
    }
}
int main() {
    int n, m;
    while (scanf("%d%d", &n, &m) != EOF) {
        init(n);
        for (int i = 1; i <= m; i++) {
            int x, y, dis;
            scanf("%d%d%d", &x, &y, &dis);
            //无向图添边一次,有向图添边两次
            G[x][y] = dis;
            G[y][x] = dis;
        }
 
        floyd(n);
 
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++)
                printf("%d ", G[i][j]);
            printf("\n");
        }
    }
    return 0;
}
```

# 连通性算法（与计算传递闭包相同）
Floyd 可以用来判断图中两点是否连通。

对于一个没有边权的图，可将相邻两点距离设为 dis[i][j]=true，不相邻的两点距离设为 dis[i][j]=false，而后进行 Floyd 算法即可。 

此外，可以在求连通性的同时，进行传递闭包计算：对于一个节点 i，如果 j 能到 i，i 能到 k，那么 j 就能到 k，求传递闭包，就是把图中所有满足这样传递性的节点计算出来，计算完成后，就知道任意两个节点之间是否相连。 
```c
for(int k=1;k<=n;k++)//第一重循环为i→j的中间点k
    for(int i=1;i<=n;i++)//第二重循环为起点i
        for(int j=1;j<=n;j++)//第三重循环为终点j
            dis[i][j] |= dis[i][k] && dis[k][j];
```


# 求最小环
Floyd 算法还可以用来解决最小环问题，所谓最小环问题，最小环就是指在一张图中找出一个环，使得这个环上的各条边的权值之和最小。

记两点间的最短路为 dis[i][j]，w[i][j] 为边 < i,j > 的权值，res 为图的最小环

一个环中最大的节点为 k，与它相连的节点为 i、j，这个环的最短长度为 w[i][k] + w[k][j]+(i 到 j 的路径中所有节点编号都小于 k 的最短路径长度)

根据 Floyed 原理，在最外层进行 k-1 次循环之后 dis[i][j] 代表了 i 到 j 的路径中，所有结点编号都小于 k 的最短路径，因此该算法一定能找到图中的最小环。
```c
int res = INF;
for(int k = 1; k <= n; k++){//第一重循环为i→j的中间点k
    for(int i = 1; i <= n; i++)//第二重循环为起点i
        for(int j = 1; j <= n; j++)//第三重循环为终点j
            res = min(res, dis[i][j] + w[j][k] + w[k][i]);//环的最短长度
    
    for(int i = 1; i <= n; i++)//第二重循环为起点i
        for(int j = 1; j <= n; j++)//第三重循环为终点j
            dis[i][j] = min(dis[i][j], w[i][k] + w[k][j]);//最短路径
}
```
一个环中最大的节点为k，与它相连的节点为i，j，这个环的最短长度为g[i][k]+g[k][j]+(i到j的路径中，所有节点编号都小于k的最短路径长度)。
根据floyed原理，在最外层进行k-1次循环之后dis[i][j]则代表了i到j的路径中，所有结点编号都小于k的最短路径。
综上所述，该算法一定能找到图中的最小环。


## 【模板】
我们可以发现如果图里面存在环的话，我们假设i，j间有一条简单路径，如果有环我们可以通过另一个点k到达，是不是跟Floyd算法很相似？？？所以我们最外层的这个循环，我们用来枚举前k个点，找前k个点中的最小环，后面的Floyd照常进行就可以了，下面的代码还有记录路径，pre[i][j],表示从i到j离j最近的一个点。  
POJ1734
```c
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cstdlib>
#include<cmath>
#include<algorithm>

using namespace std;

#define REP(i,a,b) for(register int i = (a), i##_end_ = (b); i <= i##_end_; ++i)

inline int read()
{
    char c = getchar();register int fg = 1, sum = 0;
    while(c < '0' || c > '9')
    {
        if(c == '-')fg = -1;
        c = getchar();
    }
    while(c >= '0' && c <= '9')
    {
        sum = sum * 10 + c - '0';
        c = getchar();
    }
    return fg * sum;
} 

int n, m;
int a[200][200],dis[200][200],pre[200][200],path[100000];
#define inf 0x7ffffff

int main()
{
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(a,0,sizeof(a));
        memset(pre,0,sizeof(pre));
        memset(dis,0,sizeof(dis));
        memset(path,0,sizeof(path));
        REP(i,1,n)REP(j,1,n)a[i][j] = inf,pre[i][j] = i;
        REP(i,1,m)
        {
            int x,y,z;
            x = read(), y = read(), z = read();
            a[x][y] = a[y][x] = min(a[y][x],z);
        }
        memcpy(dis,a,sizeof(dis));
        int ans = inf,num = 0;
        REP(k,1,n)
        {
            for(int i = 1; i < k; i++)
            {
                for(int j = i+1; j < k; j++)
                {
                    int tmp = dis[i][j] + a[i][k] + a[k][j];
                    if(tmp < ans)
                    {
                        ans = tmp;
                        num = 0;
                        int now = j;
                        while(now != i)
                        {
                            path[num++] = now;
                            now = pre[i][now];
                        }
                        path[num++] = i,path[num++] = k;
                    }
                }
            }
            REP(i,1,n)
            {
                REP(j,1,n)
                {
                    if(dis[i][j] > dis[i][k]+dis[k][j])
                    {
                        dis[i][j] = dis[i][k] + dis[k][j];
                        pre[i][j] = pre[k][j];
                    }
                }
            }
        }
        if(ans == inf){printf("No solution.\n");continue;}
        int i;
        for(i = 0; i < num-1; i++)printf("%d ",path[i]);
        printf("%d\n",path[i]);
    }
    return 0;
}
```

## 【其他用途】
FLoyd算法用来解决其他的一些多源最短路的问题有想不到的妙处，比如我们所说的洛谷1119灾后重建为例：

>B地区在地震过后，所有村庄都造成了一定的损毁，而这场地震却没对公路造成什么影响。但是在村庄重建好之前，所有与未重建完成的村庄的公路均无法通车。换句话说，只有连接着两个重建完成的村庄的公路才能通车，只能到达重建完成的村庄。  
>题目描述  
>给出B地区的村庄数N，村庄编号从0到N-1，和所有M条公路的长度，公路是双向的。并给出第i个村庄重建完成的时间t[i]，你可以认为是同时开始重建并在第t[i]天重建完成，并且在当天即可通车。若t[i]为0则说明地震未对此地区造成损坏，一开始就可以通车。之后有Q个询问(x, y, t)，对于每个询问你要回答在第t天，从村庄x到村庄y的最短路径长度为多少。如果无法找到从x村庄到y村庄的路径，经过若干个已重建完成的村庄，或者村庄x或村庄y在第t天仍未重建完成 ，则需要返回-1。  
输入输出格式  
输入格式：  
输入文件rebuild.in的第一行包含两个正整数N，M，表示了村庄的数目与公路的数量。  
第二行包含N个非负整数t[0], t[1], …, t[N – 1]，表示了每个村庄重建完成的时间，数据保证了t[0] ≤ t[1] ≤ … ≤ t[N – 1]。  
接下来M行，每行3个非负整数i, j, w，w为不超过10000的正整数，表示了有一条连接村庄i与村庄j的道路，长度为w，保证i≠j，且对于任意一对村庄只会存在一条道路。  
接下来一行也就是M+3行包含一个正整数Q，表示Q个询问。  
接下来Q行，每行3个非负整数x, y, t，询问在第t天，从村庄x到村庄y的最短路径长度为多少，数据保证了t是不下降的。  
输出格式：  
输出文件rebuild.out包含Q行，对每一个询问(x, y, t)输出对应的答案，即在第t天，从村庄x到村庄y的最短路径长度为多少。如果在第t天无法找到从x村庄到y村庄的路径，经过若干个已重建完成的村庄，或者村庄x或村庄y在第t天仍未修复完成，则输出-1。  
输入输出样例  
输入样例#1：  
4 5  
1 2 3 4  
0 2 1  
2 3 1  
3 1 2  
2 1 4  
0 3 5  
4  
2 0 2  
0 1 2  
0 1 3  
0 1 4  
输出样例#1：  
-1  
-1  
5  
4  
说明  
对于30%的数据，有N≤50；  
对于30%的数据，有t[i] = 0，其中有20%的数据有t[i] = 0且N＞50；  
对于50%的数据，有Q≤100；  
对于100%的数据，有N≤200，M≤N*(N-1)/2，Q≤50000，所有输入数据涉及整数均不超过100000。

看数据范围我们很容易想到Floyed，但是怎么做呢？？？我们不妨假设我们所枚举的k点是指的前k个村庒，由于出题者良心地帮我们排好了顺序，于是我们在Floyd的时候枚举询问，由于你枚举到这个点k，经过k点之前的最短路是被更新出来了的，是不是很明了？？？
```c
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>

using namespace std;

#define REP(i,a,b) for(register int i = (a), i##_end_ = (b); i <= i##_end_; ++i)

inline int read()
{
    char c = getchar();register int fg = 1, sum = 0;
    while(c < '0' || c > '9')
    {
        if(c == '-')fg = -1;
        c = getchar();
    }
    while(c >= '0' && c <= '9')
    {
        sum = sum * 10 + c - '0';
        c = getchar();
    }
    return fg * sum;
}

const int maxn = 100000+10;
int n,m,t[500],qx[maxn],qy[maxn],qt[maxn],dis[500][500];

int main()
{
    n = read(), m = read();
    memset(dis,0x3f,sizeof(dis));
    REP(i,0,n-1)t[i] = read();
    REP(i,1,m)
    {
        int x,y,z;
        x = read(), y = read(), z = read();
        dis[x][y] = z,dis[y][x] = z;
    }
    int q = read();
    REP(i,1,q)
    {
        qx[i] = read(), qy[i] = read(), qt[i] = read();
    }
    int now = 1;
    t[n] = t[n-1] + 1;
    REP(k,0,n-1)
    {
        while(now <= q && t[k] > qt[now])
        {
            int ans = dis[qx[now]][qy[now]];
            if(t[qx[now]] >= t[k] || t[qy[now]] >= t[k])ans = -1;
            printf("%d\n",ans == dis[n][n] ? -1 : ans);
            now++;
        }
        REP(i,0,n-1)
        {
            REP(j,0,n-1)
            {
                if(dis[i][j] > dis[i][k] + dis[k][j])
                    dis[i][j] = dis[i][k] + dis[k][j];
            }
        }
    }
    while(now <= q)
    {
        int ans = dis[qx[now]][qy[now]];
        printf("%d\n",ans == dis[n][n] ? -1 : ans);
        now++;
    }
}
```