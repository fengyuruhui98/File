# C语言随机数生成教程，C语言rand和srand用法详解
在实际编程中，我们经常需要生成随机数，例如，贪吃蛇游戏中在随机的位置出现食物，扑克牌游戏中随机发牌。

在C语言中，我们一般使用 <stdlib.h> 头文件中的 rand() 函数来生成随机数，它的用法为：  

int rand (void);

void 表示不需要传递参数。
>C语言中还有一个 random() 函数可以获取随机数，但是 random() 不是标准函数，不能在 VC/VS 等编译器通过，所以比较少用。  

rand() 会随机生成一个位于 0 ~ RAND_MAX 之间的整数。

RAND_MAX 是 <stdlib.h> 头文件中的一个宏，它用来指明 rand() 所能返回的随机数的最大值。C语言标准并没有规定 RAND_MAX 的具体数值，只是规定它的值至少为 32767。在实际编程中，我们也不需要知道 RAND_MAX 的具体值，把它当做一个很大的数来对待即可。

下面是一个随机数生成的实例：
```c
#include <stdio.h>
#include <stdlib.h>
int main(){
    int a = rand();
    printf("%d\n",a);
    return 0;
}
```
运行结果举例：
193
## 随机数的本质
多次运行上面的代码，你会发现每次产生的随机数都一样，这是怎么回事呢？为什么随机数并不随机呢？

实际上，rand() 函数产生的随机数是伪随机数，是根据一个数值按照某个公式推算出来的，这个数值我们称之为“种子”。种子和随机数之间的关系是一种正态分布，如下图所示：
![image][image]
C语言随机数和种子之间呈正态分布

种子在每次启动计算机时是随机的，但是一旦计算机启动以后它就不再变化了；也就是说，每次启动计算机以后，种子就是定值了，所以根据公式推算出来的结果（也就是生成的随机数）就是固定的。
## 重新播种
我们可以通过 srand() 函数来重新“播种”，这样种子就会发生改变。srand() 的用法为：

void srand (unsigned int seed);

它需要一个 unsigned int 类型的参数。在实际开发中，我们可以用时间作为参数，只要每次播种的时间不同，那么生成的种子就不同，最终的随机数也就不同。

使用 <time.h> 头文件中的 time() 函数即可得到当前的时间（精确到秒），就像下面这样：
```c
srand((unsigned)time(NULL));
```

对上面的代码进行修改，生成随机数之前先进行播种：
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    int a;
    srand((unsigned)time(NULL));
    a = rand();
    printf("%d\n", a);
    return 0;
}
```
多次运行程序，会发现每次生成的随机数都不一样了。但是，这些随机数会有逐渐增大或者逐渐减小的趋势，这是因为我们以时间为种子，时间是逐渐增大的，结合上面的正态分布图，很容易推断出随机数也会逐渐增大或者减小。
## 生成一定范围内的随机数
在实际开发中，我们往往需要一定范围内的随机数，过大或者过小都不符合要求，那么，如何产生一定范围的随机数呢？我们可以利用取模的方法：

int a = rand() % 10;    //产生0~9的随机数，注意10会被整除

如果要规定上下限：

int a = rand() % 51 + 13;    //产生13~63的随机数

分析：取模即取余，rand()%51+13我们可以看成两部分：rand()%51是产生 0~50 的随机数，后面+13保证 a 最小只能是 13，最大就是 50+13=63。

最后给出产生 13~63 范围内随机数的完整代码：
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(){
    int a;
    srand((unsigned)time(NULL));
    a = rand() % 51 + 13;
    printf("%d\n",a);
    return 0;
}
```
## 连续生成随机数
有时候我们需要一组随机数（多个随机数），该怎么生成呢？很容易想到的一种解决方案是使用循环，每次循环都重新播种，请看下面的代码：
```C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    int a, i;
    //使用for循环生成10个随机数
    for (i = 0; i < 10; i++) {
        srand((unsigned)time(NULL));
        a = rand();
        printf("%d ", a);
    }
    return 0;
}
```
运行结果举例：
8 8 8 8 8 8 8 8 8 8

运行结果非常奇怪，每次循环我们都重新播种了呀，为什么生成的随机数都一样呢？

这是因为，for 循环运行速度非常快，在一秒之内就运行完成了，而 time() 函数得到的时间只能精确到秒，所以每次循环得到的时间都是一样的，这样一来，种子也就是一样的，随机数也就一样了。






[image]:data:image/gif;base64,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