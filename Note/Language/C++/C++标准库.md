1. Vector: 动态数组：存放变量的容器（每个元素都是一个变量，且每个元素类型相同）
```C++
#include <vector>
using namespace std;
vector<int> arr;
int c = arr.size();//数组arr调用自己的成员函数， 因此用成员访问操作符*点*来访问
bool b = arr.empty();
arr.push_back(25);
int i = arr[0];
for(size_t i = 0; i < arr.size(); ++i);
```
2. string: 变长字符串
与`vector<char>`几乎
但string增加了许多针对字符串的操作：加法操作符，查找子串，截取子串
```C++
#include <string>
using namespace std;

string s("I love you");
s.size() == 11;
s += " too";
```
3. 迭代器
3.1 auto关键字：在变量定义时，用别人的类型作为自己的类型的一种变量定义方式， 也叫**类型自动推断**

auto & a = b表示a是b的引用， 类型与b相同

```c++
int a = 1;
auto b = a;

const int a = 1;
int b1 = a;
auto b2 = a;

int a = 1;
int & b1 = a;
auto & b2 = a;

const int a = 1;
auto & b = a;

if(auto itr = a.begin(); itr != a.end(); itr++);

//无auto
for(vector<int>::iterator itr = a.begin(); itr != a.end(); ++itr);
```
3.2迭代器：专门用来遍历容器  

假设v为一个容器，如：vector<int> v;

迭代器类型

`vector<int>::iterator`

定义一个迭代器：

`vector<int>::iterator itr;`

迭代器能力：
函数|描述|备注
-|-|-
$v.begin()$|返回指向第一个元素的迭代器|
$v.end()$|指向最后一个元素的下一个位置的迭代器|迭代器到达这里表示结束，专门用来结束迭代
$*itr$|解引用,获得所指向元素的引用|
$++itr$|向后移动一次，指向下一个元素|

迭代器遍历与下标遍历差别

功能|下标访问|迭代器访问
-|-|-
访问第N个元素|$a[i]$|$*itr$
访问第一个元素|$a[0]$|$v.begin()$
访问最后一个元素|$a[a.size() - 1]$|$*(a.begin() + a.size()-1)$
结束循环|$i < a.size()$|$itr != a.end()$
移向下一个元素|$++i$|$++itr$

```c++
#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<int> a{ 1,3,5,7,8,9 };
	//rbegin获取逆序迭代器的第一个迭代器；
	//rend获得逆序迭代器的最后一个迭代器的下一个位置（哨兵）
	for (auto itr = a.rbegin(); itr != a.rend(); ++itr)
	{
		cout << *itr << " ";
	}
	return 0;
}
```