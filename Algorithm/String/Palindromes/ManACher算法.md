# Manacher算法(https://www.cnblogs.com/Dfkuaid-210/p/manacher.html)
## 问题描述
给定一个字符串，求出其最长回文子串，如：    
- `s=“abcd”` ，最长回文长度为 1；  
- `s=“ababa”`，最长回文长度为 5；  
- `s=“abccb”`，最长回文长度为 4，即 bccb。  

## 1.朴素算法
注意回文串可能有奇偶两种情况，首先，对于奇回文：  

枚举每个字符作为回文串的中点，不断向左右两端扩展，直到到达边界或两边字符不同：

```c
Palindromes(char * c){
    int ans = 0;
    for(int i = 0; i < strlen(i); i++){
        int j = 0; 
        while()
    }
}
```