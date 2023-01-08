# 调侃C中的define

>目录
* [二狗子](#二狗子)
* [虚构一下](#虚构一下)
* [基本用法](#基本用法)
* [作用域](#作用域)
* [条件编译](#条件编译)

## 二狗子
二狗子这个名字，在大街小巷，在电视剧中几乎都能听到。我也不知道老一辈的父母为什么这么喜欢给自己的孩子取这样的名字，唯一能让我信服的理由是：顺口！大叔大伯们之所以叫这个名字还有一个理由，之前孩子多，希望孩子像小狗儿一样好喂养。

正好我们村有个孩子也叫二狗子，大名叫张力万，无论是叫二狗子还是叫张力万，都指的是同一个人。我们大多数情况下还是叫他二狗子，他也习惯了倒也觉得亲切，叫张力万一般都是在正式场合。

有些企业文化中规定：“不允许在公司直接喊同事的名字，每个人必须有个英文名”。比如二狗子入职到这样的企业，大家不允许喊他二狗子或者张力万，于是二狗子又有了一个英文名：“Jack”，你看 “Jack” 这个名字既符合企业文化又听起来高大尚。

无论是 “二狗子“ 还是 “Jack“ 都是张力万的别名。

在 C 语言中，关键字 define 和 typedef 就可以用来取别名，但是二者又有不同点，今天主要分享一下 define 的用法。

下面是使用 define 来模拟别名，示例如下：
```c
#define Zhangliwan1 "二狗子"
#define Zhangliwan2 "Jack"

#define Zhangliwan "张力万"

int main(int argc, const char *argv[])
{
    printf("我们村的%s可以叫他%s也可以叫他%s\n", Zhangliwan, Zhangliwan1, Zhangliwan2);
    
    return 0;
}
```
输出结果：
>我们村的张力万可以叫他二狗子也可以叫他Jack

## 虚构一下
丹尼斯·里奇 一个伟大而低调的牛人，是Unix之父、C语言之父。

丹尼斯·里奇还将Unix的设计原则定为 KISS 原则 即 Keep it simple stupid，保持简单和直接，所以Unix一直都是经典中的经典。这也说明丹尼斯·里奇不仅是一个优秀的工程师，还是一个优秀的产品经理。

我在想，当初丹尼斯·里奇和肯·汤普逊在实验室里没事也会讨论C语言的事情。

丹尼斯·里奇：“老兄，你看我们现在的语言是不是过于复杂了？”

肯·汤普逊：“的确有点复杂，我有个大胆的想法，不知当讲不当讲？”

丹尼斯·里奇：“你还跟我墨迹啥，有话直接说呗，呵呵！”

肯·汤普逊：“嗯，我们可以开发一门新的语言，他要足够的简单、高效。”

丹尼斯·里奇：“想法是挺好的，那就开干吧！”

于是C语言诞生了。

肯·汤普逊：“你有没有觉得我们在定义常量的时候不太方便？”

丹尼斯·里奇：“是呀，这样你看行不行，弄个预处理器可以让我们任意定义常量，暂时称他为 ‘宏’ 吧！”

肯·汤普逊：“我觉得完全没有问题，来，徒手写一个。”

丹尼斯·里奇：“哈哈，给力！”

于是 define 就有了。

以上纯属个人猜想，并不是冒犯两位大师，本故事纯属虚构，如有雷同，纯属巧合。Unix和C语言的大道至简，对后代科学的发展奠定了不可磨灭的贡献和影响。

## 基本用法
关键字 define 是 C 语言中的预处理命令，它用于宏定义，在大多数定义下可以提高代码的可读性，为编程提供方便。

在 C 语言中预处理命令以 “#” 号开头，如 #include、#ifdef、#endif 和宏定义命令 #define 等。

关键字 define 的用法如下：
```
#define 新类型名 原类型名

```
```c
#define INTEGER int
INTEGER a = 100;

#define PI 3.1415927

#define UserName "user_name"

#define MAX(x, y)	 (x)>(y)?(x):(y);
```
在 C 语言中，关键字 define 的定义的常量都会在预处理阶段将用到的别名直接被原样替换掉。例如在编写源程序时，所有用到 3.1415927 的地方都可用 PI 代替，而编译时，将先由预处理程序进行宏代换即用 3.1415927 去置换所有的宏名 PI，然后再进行编译。

关键字 define 还可以结合 “#”、“##”、“#@” 使用。

- 符号 “#”，表示将其字符串化。

- 符号 “##”，表示连接变量。

- 符号 “#@”，表示将其字符化。

```c
#define M(x) x##x
#define L(x) #x

int main(int argc, const char *argv[])
{
	// M(1): 11, L(1): "1"
	printf("M(1): %i, L(1): %s\n", M(1), L(1));
}
```
我使用 “#@” 定义，无论是GCC编译器还是Clang编译器都无法通过编译，错误信息：“ ‘#’ is not followed by a macro parameter ”，但是在 Visual Studio 中编译就没有问题。示例如下：

>#define K(x) #@x

关键字 define 给我们写代码带来了一定的便利，但是如果过多的乱用它也会代码不小的麻烦，比如下面的例子：
```c
#define square(x) x*x

int main()
{
	int i;
	i = 64/square(4);
	printf("i = %d\n", i);
	
	return 0;
}
```
定义宏 square(x) 本来是求某个数的平方，按理说 64/16 结果应该是 4，但是运行程序你会发现结果是 64.

我们把上面的例子展开，因为 define 是直接原样替换，如下：
```
i = 64/4*4;
i = 16*4;
i = 64;
```
修改一下程序中 define 的定义，结果就对了。
```c
#define square(x) (x*x)

int main()
{
	int i;
	i = 64/square(4);
	// 4
	printf("i = %d\n", i);
	
	return 0;
}
```
其实最安全的做法是这样定义，如下：
```c
#define square(x) ({    \
typeof(x) y = (x);  \
y*y;                \
})
```
## 作用域
可以在C文件的开头，也可以在方法体里面，还可以在方法的声明前都可以使用 define 关键字。

定义在文件开头：
```c
#define NAME "name"

int main()
{
	printf("Define NAME: %s\n", NAME);
    return 0;
}
```
定义在方法中：
```c
void play()
{
	#define NAME "name"
	printf("Define NAME: %s\n", NAME);
}
```
这里要注意，定义在方法中，并不是指该宏定义 NAME 只能用在该方法里面，其他地方照样可以使用。
```c
void play()
{
	#define NAME "name"
	printf("Define NAME: %s\n", NAME);
}

/* 注意：该方法一定是在define定义之后才能使用NAME */
void eat()
{
	printf("Define NAME: %s\n", NAME);
}
```
## 条件编译
在C语言中或者在类C语言中如Objective-C和C++中，我们会经常用到条件编译语句，如下：
```c
#ifdef NAME

#else

#endif
```
大家在做一些跨平台开发工作的时候，也会用到条件编译语句。
```c
#ifdef ANDROID
#define PLAYFORM 1
#else
#define PLAYFORM 2
#endif
```
还有就是类似防止重复包含（重复定义）头文件，也会用到条件编译，如下：
```c
#ifndef __Header_Person_H__
#define __Header_Person_H__
#endif
```
下面是来自Linux Kernel里面的代码片段：
```c
#if defined(CONFIG_ALPHA_GENERIC)
#define GAMMA_BIAS		alpha_mv.sys.t2.gamma_bias
#elif defined(CONFIG_ALPHA_GAMMA)
#define GAMMA_BIAS		_GAMMA_BIAS
#else
#define GAMMA_BIAS		0
#endif
```
既然我们可以定义宏，那么是否可以取消宏定义呢？答案是当然可以。
```c
void play()
{
	#define NAME "name"
	printf("Define NAME: %s\n", NAME);
}

#ifdef NAME
// 取消宏定义
#undef NAME
#endif

void eat()
{
	// Compile errror: Use of undeclared identifier 'NAME'
	printf("Define NAME: %s\n", NAME);
}
```
这份 [GNU Macors] 在线文档介绍了很多关于宏定义的知识，可以点击前往学习。