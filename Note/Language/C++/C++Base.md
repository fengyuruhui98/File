1. 变量的地址是变量所占空间的起始内存地址。  
2. 不同类型变量对应不同操作（整形可加减乘除，字符型加减乘除无意义）  
3. C++中内置类型：
>int  
long long 
double
bool
char
4. 引用：给变量创建别名，并不会创建新变量
```C++
int a = 1;
int & b = a;
```
使用引用的场景：
- 向函数传递参数
- 操作符重载
5. 输出换行：
```c++
cout<<endl;
cin>>s;
```
6. 命名空间：主要防止重名
```c++
//定义一个名称空间F，并在里面添加两个函数start stop
namespace F {
    void start(){}
    void stop(){}
}
namespace G {
    void start(){}
    void stop(){}
}
//我们也可以在std名称空间中添加类，虽然一般不这么做，这里仅仅用于帮助我们理解什么是名称空间
namespace std {
    struct Student{};
}
int main()
{
    F::start();//通过名称空间名称F访问该名称空间中的函数start,下同
    F::stop();

    G::start();
    G::stop();

    std::Student my;

    using namespace F;//将名称空间F中的名称暴露出来，不需要使用F::也可以访问
    start();//再次执行名称空间F中的start函数

    return 0;
}
```
std: standard template library C++ 标准模板库，cin, cout都在里面

using：简化，否则每次使用都要：
std::cout
7. getline:读取一行内容，需要包含：
\#include<string>

```c++
string line;
getline(cin, line);
#include <iostream>
#include<string>
using namespace std;

int main()

{
	cout<<"please input 2 lines:"<<endl;
	string line;
	getline(cin, line);//这里程序等待你手动输入一行，按回车键结束这一行的输入
	cout<<"your first input line is:"<<line<<endl;
	getline(cin, line);//这里程序等待你手动输入一行，按回车键结束这一行的输入
	cout << "your second input line is:" << line << endl;
	return 0;
}
```
8. cin的回车键问题
cin在获得输入的时候，需要用户按回车键结束。但是cin并没有管这个回车键，而是把回车键留在了输入流里。

这样当接下来执行getline的时候，getline上来就遇到了cin留下来的回车键。而getline遇到回车就会结束。这导致getline直接退出了。

代码如下：
```c++
#include <iostream>
#include <string>
using namespace std;

int main()
{
    int a = 0;
    cin >> a;//第一行：这里输入了1234和回车（光标到下一行）
    cout << a << endl;//第二行：这里输出了1234和一个回车（光标到下一行）
    string line;
    /*下面这里会直接得到cin留下来的回车键而结束，
      导致line的内容为空，这不是我们想要的
    */
    getline(cin, line);//第三行
    cout << line << endl; //这里输出一个回车（光标到下一行）
    //第四行

    return 0;
}
```

可以看出getline吃到了cin留下的回车
可以看到输出和代码注释中的解释一样。

如何解决这种问题呢？

就是不要让cin留一个换行符在输入流里面。执行下面的语句把cin留下来的回车擦出掉（吃掉）。

cin.ignore();//让cin把用户为了输入a而输入的回车吃掉
正确的代码如下：
```c++
#include <iostream>
#include <string>
using namespace std;

int main()
{
    int a = 0;
    cin >> a;//第一行：这里输入了1234和回车（光标到下一行，第二行）
    cin.ignore();//把用户为了输入a而输入的回车吃掉
    cout << a << endl;//第二行：这里输出了1234和一个回车（光标到下一行，第三行）
    string line;
    /*等待用户输入一行内容*/
    getline(cin, line);//第三行，这里用户输入了abcdef和一个回车（光标到下一行，第四行）
    cout << line << endl; //这里输出abcdef和一个回车（光标到下一行，第5行）

    return 0;
}
```

9. 输出两位小数，控制格式

std::fixed 表示使用小数计数法，而不是科学计数法显示浮点数。

std::setprecision(n) 表示小数部分保留n位，最后一位使用四舍五入法显示浮点数。
```c++
#include <iostream>
#include <iomanip>//控制格式fixed setw setprecision
using namespace std;

int main()
{
	double i   = 100 / 3.0; //33.33333333333......
	double you = 200 / 3.0; //66.66666666666......
	//fixed使用小数计数法(而不是科学计数法)显示浮点数
	//setprecision(4) 小数部分保留4位，最后一位四舍五入
	cout << fixed << setprecision(4) 
		<< i  << endl
		<< you<< endl
		;
}
```
10. C++ 中不常使用传递指针，使用传值，传引用
11. 模板函数:同化对不同类型变量的操作
```c++
template<typename T>
bool bigger(const T& lhs, const T& rhs){
    return lhs > rhs;
}
#include <iostream>
using namespace std;

template<typename T>
bool biggerThan(const T& lhs, const T& rhs)
{
    return lhs > rhs;
}

int main()
{
    cout << "abc > def ? : " << biggerThan("abc", "def")<<endl;//使用ASCII码顺序来比较
    cout << "1.0 > 2.0 ? : " << biggerThan(1.0, 2.0) << endl;
}
```
12. 变量存储：
- 栈内存：不用new的都在栈上
- 堆内存：new的变量
13. 解引用
在 动态内存(指针变量) 中，我们创建了指针变量（地址变量：专门存储别的变量的地址的变量）。但是并没有去使用这种变量。

使用乘法符号*和地址变量结合（紧挨着写）来表示堆上的那个无名变量，这就是解引用。
```c
#include <iostream>
using namespace std;

int main()
{
    int* a = new int;//创建一个地址变量a存储了堆上一个无名变量的地址。
    *a = 2;//解引用：用乘法符号和地址变量结合得到无名整形变量。并给无名变量赋值2
    cout << *a << endl;//输出无名变量的值

    return 0;
}
```
14. 动态数组
int * a = new int[10];