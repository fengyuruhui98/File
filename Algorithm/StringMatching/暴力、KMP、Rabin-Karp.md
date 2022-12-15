# 不同模式匹配算法详解

## 1.概述
单模式匹配是处理字符串的经典问题，指在给定字符串中寻找是否含有某一给定的字串。比较形象的是CPP中的`strStr()`函数，Java的String类下的`indexOf()`函数都实现了这个功能，本文讨论几种实现单模式匹配的方法，包括暴力匹配方法、KMP方法、以及Rabin-Karp方法（虽然Rabin-Karp方法在单模式匹配中性能一般，单其多模式匹配效率较高，且采取非直接比较的方法也值得借鉴）。
<center>

算法|预处理时间|匹配时间
-|-|-
暴力匹配法	|	|O(mn)
KMP|	O(m)	|O(n)
Rabin-Karp	|O(m)	|O(mn)

</center>

## 2.暴力匹配

模式匹配类的问题做法都是类似使用一个匹配的滑动窗口，失配时改变移动匹配窗口，具体的暴力的做法是，两个指针分别指向长串的开始、短串的开始，依次比较字符是否相等，当不相等时，指向短串的指针移动，当短串指针已经指向末尾时，完成匹配返回结果。

以[leetcode28. 实现 strStr()](https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/)为例给出实现代码（下同）
```java
class Solution {
    public int strStr(String haystack, String needle) {
        int m = haystack.length(), n = needle.length();
        if (needle.length() == 0) return 0;
        for (int i = 0; i <= m - n; i++) {
            for (int j = 0; j < n; j++) {
                if (haystack.charAt(i + j) != needle.charAt(j))
                    break;
                if (j == n - 1)
                    return i;
            }
        }
        return -1;
    }
}
```
值得注意的是，Java中的`indexO()`方法即采用了暴力匹配方法，尽管其算法复杂度比起下面要谈到的KMP方法要高上许多。

一个可能的解释是，日常使用此方法过程中串的长度都比较短，而KMP方法预处理要生成next数组浪费时间。而一般规模较大的字符串可以由开发人员自行决定使用哪种匹配方法。

```java
public static int indexOf(byte[] value, int valueCount, byte[] str, int strCount, int formIndex){
    byte first = str[0];
    int max = valueCount - strCount;

    for(int i = formIndex; i <= max; ++i){
        if(value[i] != first){
            do{
                ++i
            }while(i <= max && value[i] != first);
        }

        if(i < max){
            int j = i + 1;
            int end = j + strCount -1;

            for(int k = 1; j < end && value[j] == str[k]; ++k){
                ++j;
            }

            if(j == end){
                return i;
            }
        }
    }

    return -1;
} 
```

## 3.KMP算法
这个算法由高德纳和沃恩·普拉特在1974年构思，同年詹姆斯·H·莫里斯也独立地设计出该算法，最终三人于1977年联合发表。

大体想法是，在暴力匹配的前提下，每次失配时，不再从待匹配串开头开始重新匹配，而是充分利用已经匹配到的部分，具体的就是使用一个部分匹配表（即在程序中经常讲的next数组），利用这一特性以避免重新检查先前匹配的字符。

比如对于待匹配串`abcabce`当我匹配到末尾最后一个e字母时，发现失配，一般的做法是，对于长串指针往后移动一位，然后从待匹配串开始重新匹配，但事实上，我们发现对于待匹配串失配位置以前的字符串`abcabc`来讲，存在着一个长度为3的相同的字串`abc`，我们可以把第一个叫做前缀，第二个叫做后缀，所以对于当在后缀下一个字符失配时，我们只需要回溯到前缀的下一个字符继续匹配即可，对于此串即待匹配串移动到第四个字符（数组下标为3）开始匹配。

所以对于KMP算法，核心就是构建待匹配串的部分匹配表。其作用是当模式串第`i`个位置失配，我不必从模式串开始再重新匹配，而是移动到前`i`个字符的某个位置，具体这个位置是前`i`个字符的最长公共前后缀的长度。

依旧以`abcabce`为例，假如匹配到第`i = 5`也就是第六个字母（第二个`c`）时，失配，那么我只需要退回到`i = 2`开始匹配即可，因为匹配到第六个字母时，我们已经确定`abcab`匹配成功，很明显发现`abcab`中出现了两次`ab`且分别是前后缀，那么此时只需要从`i = 2`接着匹配即可。所以计算部分匹配表本质上就是对模式串本身做了多次匹配，或者可以理解为模式串构建了一个失配的自动机。

所以对于`abcabce`很容易计算出部分失配表，特别的`i = 0`时令`next[0] = -1`。

<center>

i	|0|	1|	2|	3|	4|	5|	6
-|-|-|-|-|-|-|-
模式串	|a|	b|	c|a|b|c|e
next[i]	|-1|0|0|0|1|2|3

</center>

给出算法Java实现
```java
class Solution {
    public int strStr(String haystack, String needle) {
        int i = 0, j = 0;
        int sLen = haystack.length();
        int pLen = needle.length();
        if (pLen == 0) {
            return 0;
        }

        int[] next = getNext(needle);
        while (i < sLen && j < pLen) {
            if (j == -1 || haystack.charAt(i) == needle.charAt(j)) {
                i++;
                j++;
            } else {
                j = next[j];
            }
        }
        return j == pLen ? (i - j) : -1;

    }

    public int[] getNext(String p) {
        int pLen = p.length();
        int[] next = new int[pLen];
        int k = -1;
        int j = 0;
        next[0] = -1;
        while (j < pLen -1) {
            if (k == -1 || p.charAt(j) == p.charAt(k)) {
                k++;
                j++;
                next[j] = k; 
            } else {
                k = next[k];
            }
        }

        return next;
    }
}
```
## 4.Rabin-Karp算法
Rabin–Karp算法由 Richard M. Karp 和 Michael O. Rabin 在 1987 年发表，用来解决模式匹配问题，在多模式匹配中其效率很高，常见的应用就是论文查重。

![image1][image1]

Rabin–Karp算法采用了计算字符串hash值是否相等的方法来比较字符串是否相等，当然hash算法肯定会出现冲突的可能，所以对于计算出hash相等后还需用朴素方法对比是否字符串真的相等。

但是即使计算哈希，也需要每次都计算一个长度为模式串的哈希值，真正巧妙的地方在于，RK算法采取了滚动哈希的方法，我们假设需要匹配的字符只有26个小写字母来展开讨论。

我们采取常见的多项式哈希算法来计算，底数取一个经验值31。(JDK对于String的hashCode()方法也是如此)

假设主串为`abcdefg`，模式串为`bcde`，首先计算模式串的hash值，基于上述假设的前提下，为了简化，我们将字母进一步做一个映射转换成整型（统一减去'a'），那么只需要计算[1,2,3,4]的哈希值即可，得到

$$hash=1×31^3+2×31^2+3×31^1+4×31^0$$

维护一个大小为模式串长度的滑动窗口，开始从主串开头计算窗口内的hash值，比如最开始窗口内字符串为`abcd`，此时有

$$h_0=0×31^3+1×31^2+2×31^1+3×31^0$$

然后此时发现$h_0$与模式串哈希值并不相等，则将窗口往后移动一个单位，此时窗口内的字符串是`bcde`，我们计算它的hash值

$$h_1=1×31^3+2×31^2+3×31^1+4×31^0$$

但此时显而易见的是，$h_1$可以由$h_0$计算得来，具体的

$$h_{i+1}=(h_i−c_0a^{L−1})a+c_{i+L}a^0$$

所以此时我们能够由前一个窗口的哈希值以$O(1)$的时间复杂度计算出下一个窗口的哈希值，以方便比较。

当然显然字符串过长时会存储hash值的变量会溢出，所以需要每次累加时进行一次取模运算，具体的可以选取一个大素数，素数的选择可以[参考这里](https://planetmath.org/goodhashtableprimes)。

下面给出java实现
```java
class Solution {
   public static int strStr(String haystack, String needle) {
        int sLen = haystack.length(), pLen = needle.length();
        if (pLen == 0) return 0;
        if (sLen == 0) return -1;
        

        int MOD = 997;
        int power = 1;
        for (int i = 0; i < pLen; i++) {
           power = (power * 31) % MOD;
        }
        int hash = 0;
        for (int i = 0; i < pLen; i++) {
            hash = (hash * 31 + (needle.charAt(i) -'a')) % MOD;
        }

        int h = 0;
        for (int i = 0; i < sLen; i++) {
            h = (h * 31 + (haystack.charAt(i) - 'a')) % MOD;
            if (i < pLen - 1) {
                continue;
            }

            if (i >= pLen) {
                h = (h - (haystack.charAt(i - pLen)-'a') * power) % MOD;

                if (h < 0) {
                    h += MOD;
                }
            }

            if (hash == h) {
                int start = i - pLen + 1;
                boolean equal = true;
                for(int j = start, k = 0; j <= i; j++,k++) {
                    if (haystack.charAt(j) != needle.charAt(k))
                        equal = false;
                }

                if (equal) return start;

            }
        }

        return -1;

    }
}
```


[image1]:data:image/png;base64,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