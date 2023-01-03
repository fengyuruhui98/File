# Brent判圈算法

这是一个倍增算法，让乌龟保持不动，兔子走$2^i$步，看这个过程中龟兔有没有相遇，没有的话，让乌龟的位置变成兔子的位置（如果乌龟位置一直不变，它可能不会进入环中），让兔子走$2^{i+1}$步，看看会不会相遇，如此循环。这个算法也是O(n)的，但是它会比Floyd表现的更好，且Floyd是这个算法最差时的表现。

求环的长度

因为乌龟一直处在兔子更改步长上限时的位置，所以更改步长后，兔子走了几步与乌龟相遇，环的长度就是几步（就是代码中的steps_taken变量）

求环的起点

Floyd判圈算法利用了乌龟和兔子的距离是环长整数倍的性质来求出起点，所以可以让乌龟回到起点，兔子回到距离起点c（c指环的长度）处，然后接下来的步骤和Floyd一样。

代码
```c
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

int n, k;
LL bits[11];

int nxt(LL k) {
  k  = k * k;
  while (k >= bits[n]) k /= 10;
  return k;
}

int main() {
  bits[0] = 1;
  for (int i = 1; i < 11; ++i) bits[i] = bits[i - 1] * 10;
  int T;
  scanf("%d", &T);
  while (T--) {
    scanf("%d%d", &n, &k);
    int ans = k, k1 = k, k2 = k;
    int steps_taken = 0, step_limit = 2;
    while (true) {
      k2 = nxt(k2);
      if (k2 > ans) ans = k2;
      if (k1 == k2) break;
      ++steps_taken;
      if (steps_taken == step_limit) {
        steps_taken = 0;
        step_limit *= 2;
        k1 = k2;
      }
    }
    printf("%d\n", ans);
  }
}
```
-----------------------------------------------
┆ 凉 ┆ 暖 ┆ 降 ┆ 等 ┆ 幸 ┆ 我 ┆ 我 ┆ 里 ┆ 将 ┆ 　 ┆ 可 ┆ 有 ┆ 谦 ┆ 戮 ┆ 那 ┆ 　 ┆ 大 ┆ 　 ┆ 始 ┆ 然 ┆  
┆ 薄 ┆ 一 ┆ 临 ┆ 你 ┆ 的 ┆ 还 ┆ 没 ┆ 　 ┆ 来 ┆ 　 ┆ 是 ┆ 来 ┆ 逊 ┆ 没 ┆ 些 ┆ 　 ┆ 雁 ┆ 　 ┆ 终 ┆ 而 ┆  
┆ 　 ┆ 暖 ┆ 　 ┆ 如 ┆ 地 ┆ 站 ┆ 有 ┆ 　 ┆ 也 ┆ 　 ┆ 我 ┆ 　 ┆ 的 ┆ 有 ┆ 精 ┆ 　 ┆ 也 ┆ 　 ┆ 没 ┆ 你 ┆  
┆ 　 ┆ 这 ┆ 　 ┆ 试 ┆ 方 ┆ 在 ┆ 逃 ┆ 　 ┆ 会 ┆ 　 ┆ 在 ┆ 　 ┆ 清 ┆ 来 ┆ 准 ┆ 　 ┆ 没 ┆ 　 ┆ 有 ┆ 没 ┆  
┆ 　 ┆ 生 ┆ 　 ┆ 探 ┆ 　 ┆ 最 ┆ 避 ┆ 　 ┆ 在 ┆ 　 ┆ 这 ┆ 　 ┆ 晨 ┆ 　 ┆ 的 ┆ 　 ┆ 有 ┆ 　 ┆ 来 ┆ 有 ┆  
┆ 　 ┆ 之 ┆ 　 ┆ 般 ┆ 　 ┆ 不 ┆ 　 ┆ 　 ┆ 这 ┆ 　 ┆ 里 ┆ 　 ┆ 没 ┆ 　 ┆ 杀 ┆ 　 ┆ 来 ┆ 　 ┆ 　 ┆ 来 ┆  
