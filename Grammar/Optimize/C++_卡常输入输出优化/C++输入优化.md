快速读入，简称快读，比cin()、printf()等都快

# void类型的快读
```c
#include <cctype>
//isdigit()在<cctype>头文件中，用于判断字符是否为阿拉伯数字（0-9）.
inline void read(int &x){
	x=0; 
	char c=0; 
	int w=0;  
	while (!isdigit(c)) w=-1,c=getchar();  
	while (isdigit(c)) x=x*10+(c^48),c=getchar();  
	if(w) x=-x;  
}

int main() {
	read(a);
	read(b);
	return 0;
}
```
# int类型的快读
```c
#include <cctype>

inline int read(){
	int s=0,w=1;
	char c=getchar();
	while(!isdigit(c)) w=-1,c=getchar();
	while(isdigit(c)) s=s*10+(c^48),c=getchar();
	return s*w;
}

int main() {
	int n=read();
	int m=read();
	return 0;
}
```
# int类型的快读（易懂）
```c
inline int read(){
	int s=0,w=1;
	char ch=getchar();
	while(ch<'0' || ch>'9') {
		if(ch=='-') w=-1;
		ch=getchar();
	}
	while(ch>='0' && ch<='9') {
		s=s*10+ch-'0'；
		ch=getchar();
	}
	return s*w;
}

int main() {
	int n=read();
	int m=read();
	return 0;
}
```
# 不分整数类型的快读
```c
#include <cctype>

template <typename T>
inline void read(T &x){
	x=0; 
	char c=0; 
	T w=0;  
	while (!isdigit(c)) w=-1,c=getchar();  
	while (isdigit(c)) x=x*10+(c^48),c=getchar();  
	if(w) x=-x;  
}
```
# fread快读（超快）
```c
#include <cctype>
#include <cstdio>

char buf[1<<20],*p1,*p2;

#define gc() (p1 == p2 && (p2 = (p1 = buf) + fread(buf,1,1<<20,stdin), p1 == p2) ? 0 : *p1++)

template <typename T>
inline void read(T &x){
	x=0; 
	char c=0; 
	T w=0;  
	while (!isdigit(c)) w=-1,c=gc();  
	while (isdigit(c)) x=(x<<3)+(x<<1)+(c^48),c=gc();  
	if(w) x=-x;  
}

int main() {
	int n,m;
	read(n);
	read(m);
	return 0;
}
```
注意：使用fread快读后，getchar()、scanf()等都不能用了，它们都存入buf里了。所以不是特殊情况不要使用fread快读。
