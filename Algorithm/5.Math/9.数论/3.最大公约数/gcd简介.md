# gcd函数简介
最大公因数（英语：highest common factor，hcf）也称最大公约数（英语：greatest common divisor，gcd）是数学词汇，指能够整除多个整数的最大正整数。而多个整数不能都为零。例如8和12的最大公因数为4。

求两个整数最大公约数主要的方法：  
- 1.穷举法：分别列出两整数的所有约数，并找出最大的公约数。
- 2.素因数分解：分别列出两数的素因数分解式，并计算共同项的乘积。
- 3.短除法：两数除以其共同素因数，直到两数互素时，所有除数的乘积即为最大公约数。
- 4.辗转相除法：两数相除，取余数重复进行相除，直到余数为0时，前一个除数即为最大公约数。
[相关介绍](./%E6%9C%80%E5%A4%A7%E5%85%AC%E7%BA%A6%E6%95%B0.md)

## gcd函数写法
C++写gcd函数有几种写法，下面介绍几种。  
这些代码我都对拍过，请大家放心使用。

- while循环  

此段代码a、b可以为0
```c
inline int gcd(int a,int b) {
    int r;
    while(b>0) {
        r=a%b;
        a=b;
        b=r;
    }
    return a;
}
```
- 三目运算符
  
此段代码a、b可以为0
```c
inline int gcd(int a, int b){
    return b > 0 ? gcd(b, a % b) : a;
}
```
- 位运算

此段代码a、b不能为0
```c
inline int gcd(int a,int b) {
    while(b^=a^=b^=a%=b);
    return a;
}
```
原理
`(b^=a^=b^=a%=b)`相当于`(b^=(a^=(b^=(a%=b))))`相当于`a%=b,b^=a,a^=b,b^=a`
其中`b^=a,a^=b,b^=a`相当于`swap(a,b)`，详见[卡常技巧](../../Skill/C%2B%2B%E5%8D%A1%E5%B8%B8%E6%8A%80%E5%B7%A7.md)第3条。
所以`(b^=a^=b^=a%=b)`等价于`a%=b,swap(a,b)`，这就是gcd函数的一般写法。

- if+while

此段代码a、b可以为0
```c
inline int gcd(int a,int b) {
	if(b) while((a%=b) && (b%=a));
	return a+b;
}
```
- 辗转相除法

此段代码a、b不能为0
```c
inline int gcd(int a,int b) {
	if(a%b==0) return b;
		else return (gcd(b,a%b));
}
```
- gcd库函数
此段代码a、b可以为0
```c
#include <algorithm>
inline int gcd(int a,int b) {
	return __gcd(a,b);
}
```
