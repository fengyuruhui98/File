# 1.若乘上一个2的倍数的数值，可改用左移运算符

例1：

x=x*2 或 x*=2 

改成：

x=x<<1 或 x<<=1

($\log_22=1$)

例2：

x=x*32 或 x*=32

改成：

x=x<<5 或 x<<=5

($\log_232=5$)

# 2.若除以一个2的倍数的数值，可改用右移运算符

例1：

x=x/2 或 x/=2

改成：

x= x>>1 或 x>>=1

(log22=1)

例2：

x=x/32 或 x/=32

改成：

x=x>>5 或 x>>=5

(log232=5)

# 3.交换两个数值，使用异或（xor）运算符

例：
inline void swap(int &a,int &b) {
    int t=a;
    a=b;
    b=t;
}

改成：

inline void swap(int &a,int &b) {
    a=a^b;
    b=a^b;
    a=a^b;
}

# 4.for循环卡常

例：
for(int i=1; i<=n; i++) {}

改成：

for(register int i(1); i<=n; ++i) {}

# 5.变量类型前加register

例：
int a,b;

改成：

register int a,b;

原理：把变量放到寄存器里，满了会自动溢出的。

# 6.函数类型前加inline

例：
int plus(int a,int b) {
	return a+b;
}

改成：

inline int plus(int a,int b) {
	return a+b;
}

（像dfs、bfs这种复杂的函数就没必要inline了）

# 7.读入输出优化

输入优化 [点击查看](./C%2B%2B%E8%BE%93%E5%85%A5%E4%BC%98%E5%8C%96.md)
输出优化 [点击查看](./C%2B%2B%E8%BE%93%E5%87%BA%E4%BC%98%E5%8C%96.md)

# 8.判断奇偶用与（and）运算符

例：

if(a%2==0) {}

改成：

if((a&1)==0) {}

注意：a&1 一定要加括号。

# 9.取余数用与（and）运算符（除数是2的倍数）

例1：

x=667%4;

改成：

x=667&(4-1);

例2：
x=667%32;

改成：

x=667&(32-1);

# 10.循环展开（语句没有关联）

例：

for(int i=1; i<=n; i++) a[i]=1,b[i]=2;

改成：

for(int i=1; i<=n; i++) a[i]=1;  
for(int i=1; i<=n; i++) b[i]=2;

# 11.尽量不要用中的自带函数（除sort等）

# 12.正负号转换用位运算

例：

i=-i;

改成

i=~i+1;   
或  
i=(i^-1)+1; 

# 13.取绝对值（abs）用位运算

例：

k=abs(x);

改成：

k= x<0 ? -x : x;  //较快  
k=(x^(x>>31))-(x >> 31);  //很快

# 14.访问a[i]后访问a[i+1]、a[i+2]等会快很多
