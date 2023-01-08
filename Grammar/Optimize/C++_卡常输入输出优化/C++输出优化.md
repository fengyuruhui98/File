原理  
众所周知， putchar() 是用来输出单个字符的函数。  
因此将数字的每一位转化为字符输出以加速。  
要注意的是，负号要单独判断输出，并且每次 %（mod）取出的是数字末位，因此要倒序输出。

代码实现  
一般的快写：
```c
inline void write(int x) {
	if(x<0) {
		x=-x;
		putchar('-');
	}
	if(x>9) write(x/10);  
	putchar(x%10+'0'); 
}
```
但是递归实现常数是较大的，我们可以写一个栈来实现这个过程：
```c
inline void write(int x) {
	static int sta[35];
	int top=0;
	do {
		sta[top++]=x%10, x/=10;
	} while(x);
	while(top) putchar(sta[--top]+'0');
}
```
不分整数类型的快写：
```c
template <typename T>
inline void write(T x) {
    if(x<0) putchar('-'),x=-x;
    if(x<10) putchar(x+'0');
        else write(x/10),putchar(x%10+'0');
}
```
# fwrite
```c
#include <cctype>
#include <cstdio>

typedef long long LL;

#define bsiz 1000000

int sta[30];
char buf[bsiz], pbuf[bsiz], *p = pbuf, *s = buf, *t = buf;

#define mputc(ch) (p - pbuf == bsiz ? fwrite(pbuf, 1, bsiz, stdout), p = pbuf, *p++ = ch : *p++ = ch)

inline void putll(LL x) {
    register int top = 0;
    if (x<0) mputc('-'), x = -x;
        do sta[top++] = x % 10, x /= 10;
    while (x);
    while (top) mputc(sta[--top] + '0');
}

int main() {
    LL ans=123;
	putll(ans);
    fwrite(pbuf, 1, p - pbuf, stdout);
    printf("\n");
	return 0;
}
```