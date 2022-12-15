# Tarjan与有向图

- [Tarjan与有向图](#tarjan与有向图)
  - [一、强连通定义](#一强连通定义)
  - [二、Tarjan算法求强连通分量](#二tarjan算法求强连通分量)
    - [1.tarjan的构成要素(DFS 生成树)](#1tarjan的构成要素dfs-生成树)
    - [2.算法的分析](#2算法的分析)
    - [3.算法的实现](#3算法的实现)
    - [4.算法的代码实物](#4算法的代码实物)
  - [三、缩点](#三缩点)
  - [四、实际应用](#四实际应用)
  - [证明，略…（超复杂的）](#证明略超复杂的)
- [Tarjan和无向图](#tarjan和无向图)
  - [一、定义和性质](#一定义和性质)
  - [二、割边（桥）和 E-DCC](#二割边桥和-e-dcc)
    - [1.模板](#1模板)
    - [2.实际应用](#2实际应用)
  - [三、割点](#三割点)
    - [1.概况](#1概况)
    - [2.实现](#2实现)
  - [四、V-DCC（点双联通分量）](#四v-dcc点双联通分量)
    - [1.求v-dcc](#1求v-dcc)
    - [2.v-dcc特异性缩点](#2v-dcc特异性缩点)
  - [例题](#例题)


## 一、强连通定义

- 有向图的任意两个节点可以相互到达则称这两个点强连通（你能到我，我也能到你）

- 如果图G 的任意两个节点强连通，那么G 就是一个强连通图
  
- 有向图的极大强连通子图，称为强连通分量
  
- PS : 一个点也可以是强连通的图哦
  
## 二、Tarjan算法求强连通分量

### 1.tarjan的构成要素(DFS 生成树)

![image1][image1]

由一个节点（如上图 1 号节点）开始 DFS，根据遍历到的边得到一棵 DFS 生成树。

在 DFS 生成树中，边分为 4 种：

- 树边（上图中黑色的边）：DFS 时搜到一个未搜过的点时形成树边；
  
- 返祖边/回边（上图中粉色的边）：指向祖先的边；
  
- 横叉边（上图中蓝色的边）：指向非祖先和非儿子节点的边；
  
- 前向边（上图中绿色的边）：指向儿子的边。
  
考虑四种边与强连通分量的联系，我们发现：对于一个强连通分量中被 DFS 到的第一个节点 u，强连通分量中的其他点一定在其子树内，这个强连通分量称为以 u 为根的。

这是基于DFS 的算法所以这些东西很重要

>1. int dfn[MAXN] (dfs的时间戳，就是dfn[i]即为i点第几次被搜到的)  
>2. int low[MAXN]（表示 i号点 它能走到的dfn最小的点的编号，注意：不是dfn号）  
>3. stack\<int>s  一个栈，用来收集和回溯  
>4. int cnt 全局计数器，记录dfn的初始变换，可持续保持数值，不受递归影响  
>5. bool st[MAXN] (记录是否在栈里)

### 2.算法的分析

设一个点 U 为联通子图 G 在 dfs 搜索树里的根，那么他就是在 G 中 dfn 的 min 对应的点（根都是最小的，因为先找到了它啊）

- 就从dfn和low的定义上讲，一个ssc的（假定我们不知道）某个节点必然会dfn == low（low是追溯的最早点，点自己和自己没边，自己能找到自己肯定是ssc）
- 初始化： 对所有的节点 dfn[u] = cnt++; low[u] = dfn[u];
  
在没有扩展之前，每个点自己都是一个联通子图。

### 3.算法的实现
在搜索过程中，对于结点 u 和它指向的节点 v 考虑 2 种情况：

1. **未被访问**：
   
    **判断标准**：dfn 没有被定义，为0，压入到栈里st变为true

    **几何意义**：传参前找到了**前向边或者树枝边**，回溯时**后向边**造成有效 low 修改

    **处理方法**：

   - 继续对 v 进行深度优先搜索。在回溯过程中，用 low[v] 更新 low[u]
   - 因为存在从 u 到 v 的直接路径，所以 v 能够回溯到的已经在栈中的结点, u 也一定能够回溯到。

2. **被访问过，已经在栈中**：
   
    **判断标准**：st [v]

    **几何意义**：横叉边

    **处理方法**：v 即已经被访问过，根据 low 值的定义，使用min函数，可以取到与之相连的栈里最早被dfs的点，low[u]则用dfn[v]尝试更新

### 4.算法的代码实物
```c
void tarjan(int x)
{
    low[x]=dfn[x]=++cnt;
    st[x]=1;
    s.push(x);

    for(int i=h[x];~i;i=nxt[i])
    {
        int j=e[i];
        if(!dfn[j])tarjan(j),low[x]=min(low[x],low[j]);
        else if(st[j])low[x]=min(low[x],dfn[j]);
    }

    if(dfn[x]==low[x])
    {
        int y;
        ++scc_cnt;
        do
        {
            y=s.top();  s.pop();  st[y]=0;
            id[y]=scc_cnt;
        }while (y!=x);
    }
}
```
以下是引用别的巨佬的伪代~
```c
tarjan(u)
{
　　DFN[u]=Low[u]=++Index // 为节点u设定次序编号和Low初值
　　Stack.push(u)   // 将节点u压入栈中
　　for each (u, v) in E // 枚举每一条边
　　　　if (v is not visted) // 如果节点v未被访问过
　　　　　　　　tarjan(v) // 继续向下找
　　　　　　　　Low[u] = min(Low[u], Low[v])
　　　　else if (v in S) // 如果节点u还在栈内
　　　　　　　Low[u] = min(Low[u], DFN[v])
　　if (DFN[u] == Low[u]) // 如果节点u是强连通分量的根
　　repeat v = S.pop  // 将v退栈，为该强连通分量中一个顶点
　　print v
　　until (u== v)
}
```

## 三、缩点

- 思想：对同一联通分量的点进行染色，同一色为一个大点，大点内部的边吞掉，指向外部的边转换成一个大点指向另一个的点的边

- 手段
```c
int id[MAXN]  (一个小点所属的大点序号)
vector<int>new_g[MAXN]  (建立一个缩点之后的新图)
核心：
for(each e in color(x)){
	if(e.to is not in subgraph(x))   new_g[x].push_back(color[e.to]);   }

for(int k=1;k<=n;k++)
    {
        for(int i=h[k];~i;i=nxt[i])
        {
            int j=e[i];
            if(id[k]!=id[j])addx(id[k],id[j]),cout<<id[k]<<" --> "<<id[j]<<endl;
        }
    }
```
## 四、实际应用

- 一个拓补图最少加入多少条边构造一个新的scc？

- ans = max（出度为0的点，入度为0的点）
证明，略…（超复杂的）
--------------------------------
# Tarjan和无向图

## 一、定义和性质

- 无向图的dfs搜索树中没有横叉边
  
割边：无向图中删去此边，全图不再联通（图的极大连通分量数增加了）  
割点：无向图中删去此点及其相连的边，全图不再联通（图的极大连通分量数增加了）

点双联通分量（V - DCC）：不含割点的极大联通子图  
边双联通分量（E - DCC）：不含割边的极大联通子图

e-dcc性质：

- 1，任意两点之间存在至少两条不相交（边不重复，点可重）路径
- 2，删去e-dcc内任意一个单点，e-dcc内部依然联通
  
v-dcc性质：

- 1，任意两点之间存在至少两条不相交（点不重复，边可重）路径
- 2，任意v-dcc内部包含一个割点
- 3，每一个割点至少属于两个v-dcc
（这种缩点很不一般）
e-dcc不一定是v-dcc，v-dcc不一定是e-dcc

割边相连的两点不一定是割点
两个割点的连边不一定是割边

## 二、割边（桥）和 E-DCC
### 1.模板

延续tarjan（有向版的框架）

- 思考：只讨论边（x ，j），如果没有这条边，子节点回不去他的父亲或者祖先，那么联通 x，j 的唯一方法就是这条边，删去立刻不联通
- 实现：对于搜索完回溯的时候，判断一条无向边（x ，j）若有d f n [ x ] < l o w [ j ] dfn[x] <low[j]dfn[x]<low[j]（严格大于），判定为割边
- edcc 的记录类似于scc
  
小细节：无向图的边是双向的，记录下来边可以防止远地不动
```c
void tarjan_edcc(int x，int from)
{
    low[x]=dfn[x]=++cnt;
    s.push(x);

    for(int i=h[x];~i;i=nxt[i])
    {
        int j=e[i];
        if(!dfn[j])
        {
            tarjan_edcc(j,i);
            low[x]=min(low[x],low[j]);
            if(low[j]>dfn[x])cut_edge[i]=cut_edge[i^1]=1;
        }
        else if(i!=(from^1) )low[x]=min(low[x],dfn[j]);
    }

    if(low[x]==dfn[x])
    {
        int y;
        ++edcc_cnt;
        do{
            y=s.top(); s.pop(); 
            id[y]=edcc_cnt;
        }while (y!=x);
    } 
}
```

### 2.实际应用
- 问题描述：将求完edcc的全图构造为一个大的edcc

- 做法：统计所有度为1的节点数 cnt ，易证明：$ans\geq{} (cnt+1)/2$

- 几何状态：edcc之后是只有桥的树，度为1的点均为叶子节点，考虑这些叶子节点互联，从底层构造环，抵消桥特性

- 具体方法：首先把两个lca最远的两个叶节点之间连接一条边，这样可以把这两个点到祖先的路径上所有点收缩到一起，因为一个形成的环一定是双连通的。然后再找两个lca最远的两个叶节点，这样一对一对找完，恰好是(cnt+1)/2次，把所有点收缩到了一起。

## 三、割点
### 1.概况
- 1，对于搜索树根结点（找到dfn=0 的递归入口），只有根有大于等于2个的子节点时，他才可以是割点（详见图一和二的比较）
- 2，对于子节点，若在无向图G中，当且仅当点u存在一个可遍历到的后代v，且点v无法走回点u的前辈时，点u就为割点。
（详见图三的v1和v2对比）

![image2][image2]

### 2.实现
- 1，根节点设为全局变量，对于根结点，统计可以搜索到的子节点（son_num为子树个数）由于是根节点，所以一定存在low==dfn，若是 son_num>1很好，是个割点了

- 2，子节点判断方法

    - 对于子节点，找到某点如果不过他的父亲，就去不了他的祖先那里，那么这就是一个割点。
    - tarjan裸法里加一个小手段，tarjan裸算法中，搜索回溯tarjan(v); 这一步后，此时v的low一定已经确定，在循环里，介入与dfn的比较，**如果v最早只能到x或者更晚的点，那就是不靠x回不到祖先了**，那么就是割点了
- 3，注意，对于割点的判断可能会有很多次，如果把割点计数嵌入tarjan里，就会很多次重复（经验细节谈）

**下面的代码嵌入了删除该点之后可在该点所在联通块内可以形成新的联通块的个数**
```c
bool cut[N];
void tarjan_cutdot(int x)
{
	low[x]=dfn[x]=++cnt;
	int s=0;
	
	for(int i=h[x];~i;i=nxt[i])
	{
		int j=e[i];
		if(!dfn[j])
		{
			tarjan_cutdot(j);
			low[x]=min(low[x],low[j]);
			if(low[j]>=dfn[x])s++;
		}
		else low[x]=min(low[x],dfn[j]);
	}
	
	if(s>0&&x!=root)cut[x]=1,s++,num=max(num,s);   不是根节点且删除可以产生至少一个子树
	else if(x==root&&s>1)cut[x]=1,num=max(num,s);   是根节点且删除产生大于1的子树
}
```
例题：ac wing 1183

## 四、V-DCC（点双联通分量）
### 1.求v-dcc
- 判断出现割点的时候，把割点以下的栈内子树全弹出
- 注意弹出栈不是弹到x（tarjan传参节点）为止 而是弹到 j为止(x仍保留在栈中)
  
u一定和新分支 j 组成一个dcc 也和旧连通块组成dcc  
那么当前最高点u还要被用在更高的包含u的旧连通块  
所以如果这个时候出栈了 回溯到比u高的点的时候 u就加不进旧连通块里
```c
void tarjan_vdcc(int x)
{
	low[x]=dfn[x]=++cnt;
	int num=0;
	s.push(x);
	
	if(x==root&&h[x]=-1)
	{
		vdcc_cnt++;
		dcc[vdcc_cnt].push_back(x);
		return ;
	}
	
	for(int i=h[x];~i;i=nxt[i])
	{
		int j=e[i];
		if(!dfn[j])
		{
			trajan_vdcc(j);
			low[x]=min(low[x],low[j]);
			if(low[j]>=dfn[x])
			{
				num++;
				if(num>1||x!=root) cut[x]=1;
				++vdcc_cnt;
				int y;
				do{
					y=s.top(); s.pop();
					dcc[vdcc_cnt].push_back(y);
				}while (y!=j);
					
				dcc[vdcc+cnt].push_back(x);
			}
		}
		else low[x]=min(low[x],dfn[j]); 
	}
}
```
### 2.v-dcc特异性缩点
- 1，每个割点自成一点
- 2，从每个 vdcc 缩成点向该 vdcc 内的割点连一条边
## 例题
[星 梦 的 b l o g](https://blog.csdn.net/qq_36288976/article/details/79382878)  
[OI Wiki 的 解 释 这个很棒棒哎，宝藏推荐](https://oi-wiki.org/graph/scc/)（原理讲解很好）  
[算 法 的 手 动 模 拟 走 这 边 ！](https://blog.csdn.net/cax1165/article/details/51864105?utm_medium=distribute.pc_relevant.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-4.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~BlogCommendFromMachineLearnPai2~default-4.control)  
[割 点 的 素 材 来 源](https://blog.csdn.net/zsyz_ZZY/article/details/79907335) 图一上，就很清晰唉  








[image1]:data:image/png;base64,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