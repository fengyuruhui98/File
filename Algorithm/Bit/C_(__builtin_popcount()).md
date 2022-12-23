# C语言————builtin_popcount（）

这个函数功能：返回输入数据中，二进制中‘1’的个数。

对于不同的使用类型，可以采用采用以下函数：

__builtin_popcount = int  
__builtin_popcountl = long int  
__builtin_popcountll = long long  

## 1. 二分法，源码采用的方法
主要思路是：将相邻两位相加，可以实现用二进制来表示输入数据中‘1’的个数。然后依次将上半部分和下半部分相和并实现计数。
```c
unsigned popcount (unsigned u)
{
    u = (u & 0x55555555) + ((u >> 1) & 0x55555555);
    u = (u & 0x33333333) + ((u >> 2) & 0x33333333);
    u = (u & 0x0F0F0F0F) + ((u >> 4) & 0x0F0F0F0F);
    u = (u & 0x00FF00FF) + ((u >> 8) & 0x00FF00FF);
    u = (u & 0x0000FFFF) + ((u >> 16) & 0x0000FFFF);
    return u;
}
```

算法案例如下：   
这个二分法的原理：  
用8位二进制数来做示范好了，例如 u = 10110011。  
10110011  
00010001 //每两位取1位，即取偶数位, u & 01010101  
01010001 //取奇数位并右移一位, (u >> 1) &   01010101

---------------  

01100010 //上面两数相加，赋值给u，注意每两列相加的结果不会进位到第三列    
00100010 //每四位取低两位， u & 00110011    
00010000 //每四位取高两位并右移两位, (u >> 2) & 00110011  

---------------
00110010 //上面两数相加，赋值给u  
00000010 //每八位取低四位， u & 00001111  
00000011 //每八位取高四位并右移四位，(u >> 4) & 00001111  

---------------
00000101 //上面两数相加，赋值给u
最终结果 u = 5。


## 方法二：末位清零
这个方法还是很常见的，利用一个数字减1之后，原来数字最后一个‘1’的位置一定会变成0，然后利用与&操作来实现清零最后一位。  
```c
unsigned popcnt (unsigned u)
{
    unsigned ret = 0;
    while (u)
        {
        u = (u & (u - 1));    // 将 u 最右边的 1 清除
        ret ++;
        }
    return ret;
}
```
## 方法三：哈希法（查表法）
这种方法感觉是最快的一种，但是也是最笨的吧。（可是我也没想到这种做法），实现原理就是把1个字节（8位）,每个数字(0~255)所对于的个数记录下来，一直查表就可以了。
```c
char poptable [256] =
{
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};
unsigned __builtin_popcount (unsigned u)
{
    return poptable [u & 0xFF] + poptable [(u >> 8) & 0xFF] + poptable [(u >> 16) & 0xFF] + poptable [(u >> 24) & 0xFF];
}
```
反思：这种方法自己竟然没有想到，虽然看起来很笨，但是实际应用中，应该是最高效的实现。
思路上思考算法时，最好将时间最快，空间最小当作两个极端情况来思考。
