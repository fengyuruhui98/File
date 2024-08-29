# 详解—— vector的详细用法和底层原理

## 一、介绍

- vector类似数组为长度可变容器，**为了实现动态数组而产生**
- vector为顺序容器，内存中**连续排列**，大小固定，超出后自动扩容。

## 二、使用

### 1.包含头文件
```c++
#include <vector>;
```
### 2.创建初始化
vector 本质是类模板，可以存储任何类型数据，数组声明时需要加上数据类型：
```c++
vector<int> arr1; // 空数组
vector<int> arr2{1,2,3}; //含三个元素数组
vector<int> arr3(4); //开辟四个空间，默认值为0
vector<int> arr4(4,1); //开辟四个空间，内容全为1
vector<int> arr5(arr4); //arr4复制进去，内容与arr4一样
vector<int> arr6(arr4.begin(),arr4.end()); //将arr4从头到尾复制进去
vector<int> arr7(arr4.rbegin(), arr4.rend()); //将arr4从尾到头复制进去
```
### 方法
**Iterators(迭代器)**

|名字|描述|
|-|-|
|begin|返回指向容器中第一个元素的迭代器|
|end|返回指向容器最后一个元素所在位置**后一个位置**的迭代器|
|rbegin|返回容器逆序的第一个元素的迭代器|
|rend|返回容器逆序的最后一个元素的**前一个位置**的迭代器|
|cbegin|和begin()功能相同，在其基础上增加了 const 属性，不能用于修改元素|
|cend|和end()功能相同，在其基础上增加了 const 属性，不能用于修改元素。|
|crbegin|和rbegin()功能相同，在其基础上增加了 const 属性，不能用于修改元素|
|crend|和rend()功能相同，在其基础上增加了 const 属性，不能用于修改元素。|

**Capacity(容量)**

|名字|描述|
|-|-|
|size|返回实际元素个数|
|capacity|返回容量|
|max_size|返回元素个数最大值，一般是$2^32 - 1$|
|empty|vector空返回true否则false|
|resize|改变**实际元素**的个数，对应size|
|reserve|增加容器容量，控制vector预留空间|
|shrink_to_fit|减少capacity到size大小，即减少到size，释放未使用容量|

**Element access(元素访问)**

|名字|描述|
|-|-|
|operator[]|可以使用[]下标访问，与数组一样|
|at|vector.at(i)与vector[i]一样|
|front|返回第一个元素|
|back|返回最后一个元素|
|data|返回指向容器中第一个元素的指针|

```c++
// C++ program to demonstrate the
// vector::date() function
#include <bits/stdc++.h>
using namespace std;
 
int main()
{
	// initialising vector
	vector<int> vec = { 10, 20, 30, 40, 50 };
 
	// memory pointer pointing to the
	// first element
	int* pos = vec.data();
 
	// prints the vector
	cout << "The vector elements are: ";
	for (int i = 0; i < vec.size(); ++i)
		cout << *pos++ << " ";
 
	return 0;
}
```

**Modifiers(修改器)**

|名字|描述|
|-|-|
push_back|在容器的尾部插入元素
pop_back|删除最后一个元素
insert|插入元素
erase|删除元素
clear|清除容器内容，size=0，存储空间不变
swap|交换两个元素的所有内容
assign|用新元素替换原有内容。
emplace|插入元素，和insert实现原理不同，速度更快
emplace_back|在容器的尾部插入元素，和push_back不同

## 三、具体用法
**1. 遍历vector**
```c++
//通过迭代器访问从begin()到end()，需要定义iterator，当然可以用auto替代。
//begin()表示第一个元素，而end()不是最后一个元素，end()是最后一个元素的后一个位置。
//迭代器：vector<int>::iterator
for (vector<int>::iterator it = arr.begin(); it != arr.end(); it++)
{
    cout << *it << endl;
}
//迭代器：vector<int>::reverse_iterator
for (vector<int>::reverse_iterator it = arr.rbegin(); it != arr.rend(); it++)
{
    cout << *it << endl;
}
//下标访问
//和数组类似，从下标0开始遍历，而不到size的大小。
for (int i = 0; i < arr.size(); i++)
{
	cout << arr[i] << endl;
}

//范围for循环
//C++11的特性，范围for，遍历元素十分方便。
for (auto num : arr)
{
	cout << num << endl;
}
```
**2. vector 容量和大小**
- 顾名思义，size表示当前有多少个元素，capacity是可容纳的大小。因为vector是顺序存储的，那么和数组一样，有一个初始容量，在vector里就是capacity。capacity必然大于等于size，每次扩容时会改变，具体大小和vector底层实现机制有关。

- max_size是可存储的最大容量，和实际的编译器、系统有关，使用的比较少。

- empty很好理解，判断vector是否为空，其实就是判断size是否等于0。定义vector时设定了大小、resize修改大小等操作，vector都不为空；clear后，size=0，那么empty判断就为空。

- resize改变size的大小，而reserve改变capacity的大小，shrink_to_fit减小capacity到size
```c++
vector<int> arr;
arr.resize(4);
arr.reserve(6);
cout << arr.size() << " " << arr.capacity() << endl;
cout << "##########################" << endl;
arr.shrink_to_fit();
cout << arr.size() << " " << arr.capacity() << endl;
```
**3. vector 常用算法**
1. push_back、pop_back 和 emplace_back
```c++
vector<int> arr;
for (int i = 0; i < 5; i++)
{
    arr.push_back(i);
}
for (int i = 0; i < 5; i++)
{
    arr.pop_back();
}

//emplace_back的效果和push_back一样，都是尾部插入元素
arr.emplace(10);

//两者底层实现机制不同：push_back将这个元素拷贝或者移动到容器中（拷贝事后会自行销毁先前创建的这个元素）而emplace_back直接在容器尾部创建这个元素,省去拷贝或移动元素过程.emplace_back速度更快。
```
2. insert 和 emplace
insert有三种用法：
```c++
//1.在指定位置插入值为val的元素。
//在arr的头部插入值为10的元素
vector<int> arr;
arr.insert(arr.begin(), 10);

//2.在指定位置插入n个值为val的元素
//从arr的头部开始，连续插入3个值为10的元素
vector<int> arr;
arr.insert(arr.begin(), 3, 10);

//3.在指定位置插入区间[start, end]的所有元素
//从arr的头部开始，连续插入arrs区间[begin, end]的所有元素
vector<int> arr;
vector<int> arrs = { 1, 2, 3, 4, 5 };
arr.insert(arr.begin(), arrs.begin(), arrs.end());

//emplace和insert同为插入元素，不过emplace只能插入一个元素：
//在arr的头部插入值为10的元素
vector<int> arr;
arr.emplace(arr.begin(), 10);
```
insert和emplace的区别和上面类似，就是一个是拷贝和复制的过程，而另一个则是直接创建一个新元素。

3. erase
erase通过迭代器删除某个或某个范围的元素，并返回下一个元素的迭代器。
```c++
vector<int> arr{1, 2, 3, 4, 5};
//删除arr开头往后偏移两个位置的元素，即arr的第三个元素，3
arr.erase(arr.begin() + 2);
//删除arr.begin()到arr.begin()+2之间的元素，删除两个;即删除arr.begin()而不到arr.begin()+2的元素
arr.erase(arr.begin(), arr.begin() + 2);
```

4. assign
assign修改vector，和insert操作类似，不过insert是从尾部插入，而assign则将整个vector改变。
```c++
//将整个vector修改为n个值为val的容器
//将arr修改为3个值为5的vector。
vector<int> arr = {5, 4, 3, 2, 1};
arr.assign(3, 10);
//将整个vector修改为某个容器[start, end]范围内的元素
//将arr修改为范围[arrs.begin, arrs.end]内的元素
vector<int> arr = {5, 4, 3, 2, 1};
vector<int> arrs = { 1, 2, 3, 4, 5 };
arr.assign(arrs.begin(), arrs.end());

//用数组的值进行范围修改
//将arr替换为数组arrs
vector<int> arr = {5, 4, 3, 2, 1};
int arrs[5] = { 1, 2, 3, 4, 5 };
arr.assign(arrs, arrs + 5);
```
5. swap 和 clear
```c++
//swap将两个vector进行交换。
vector<int> arr = {5, 4, 3, 2, 1};
vector<int> arrs = { 1, 2, 3, 4, 5 };
arr.swap(arrs);

//clear清空整个vector，size变为0，但空间仍然存在。
arr.clear();
```
6. vector二维操作
实际上，二维vector其实就是嵌套定义vector，那么对其进行操作我们可以从嵌套的vector得到单层的vector，就可以调用其方法了。

```c++
vector<vector<int>> arr;						//定义一个空的二维vector
vector<vector<int>> arr(5, vector<int>(3, 1));	//定义一个5行3列值全为1的二维vector

//访问和二维数组一样通过 [] [] 访问即可。

for (int i = 0; i < arr.size(); i++)
{
	for (int j = 0; j < arr[0].size(); j++)//注意如果arr为空不可直接arr[0]
	{
		cout << arr[i][j] << endl;
	}
}

//或者用范围for：

for (auto nums : arr)
{
    for (auto num : nums)
    {
    	cout << num << endl;
    }
}

//resize操作
vector<vector<int>> arr;
arr.resize(5);
for (auto num : arr)
{
    num.resize(3);
}
```
## 四、vector扩容原理
前面我们提到，vector作为容器有着动态数组的功能，当加入的数据大于vector容量(capacity)时会自动扩容，系统会自动申请一片更大的空间，把原来的数据拷贝过去，释放原来的内存空间。

看以下一段代码：
```c++
vector<int> arr;
for (int i = 0; i < 20; i++)
{
    arr.push_back(i);
    cout << arr.size() << " " << arr.capacity() << endl;
}
```
在VS中运行以上代码测试扩容，发现：

- 初始时capacity和size都是零；
- 开始capacity和size大小一致，在size=5时，capacity从4 -> 6，即发生了扩容：4 * 1.5 = 6，以1.5倍开始扩容。同样，在9、13、19时均是以1.5倍的方式扩容，向下取整。
- 其实，在capacity等于size时，下一次插入操作时vector就以1.5倍开始扩容。开始时，0 * 1.5 = 1（需要）, 1 * 1.5 = 2(此时需要)，2 * 1.5 = 3, 3 * 1.5 = 4，4 * 1.5 = 6,6 * 1.5 = 9 。。。
可以看到，理论上每次都是1.5扩容，但是遇到一些特殊情况如：0、1或者一次性插入多个元素时，也许1.5扩容就无法满足了。其实很简单，按照我们自己的思路，这无非是程序健壮性的体现，加一句判断语句即可。

看如下VS中vector扩容的源码：
```C++
size_type _Calculate_growth(const size_type _Newsize) const {
    
    const size_type _Oldcapacity = capacity();
    const auto _Max              = max_size();

    //扩容后将超出max_size，返回max_size
    if (_Oldcapacity > _Max - _Oldcapacity / 2) {
        return _Max; 
    }
	//采取1.5倍扩容
    const size_type _Geometric = _Oldcapacity + _Oldcapacity / 2;
	//扩容后仍然小于新加入元素后的大小，以新加入元素后的大小为准
    if (_Geometric < _Newsize) {
        return _Newsize; 
    }

    return _Geometric;
}
```
由此可见，确实是以1.5倍扩容，并且还有需要判断：是否超过max_size，以及是否小于newsize。

而其实，扩容时在插入时元素需要进行判断的，所以在vector的方法如：push_back、insert中都有用到扩容。

**push_back**
首先，在VS中，push_back有两个重载函数。_Ty就是vector模板的类型，如vector< int >中的int。发现push_back其实是调用了emplace_back成员函数。
```C++
void push_back(const _Ty& _Val) { 
	emplace_back(_Val);
}

void push_back(_Ty&& _Val) { 
	emplace_back(_STD move(_Val));
}
```
接着，进入emplace_back函数。判断capacity和size是否相等，如果相等就进入_Emplace_reallocate函数。

tips:对于size和capacity，代码里通过这三个指针实现内存管理。
![image1][image1]

```C++
template <class... _Valty>
    decltype(auto) emplace_back(_Valty&&... _Val) {
        
        auto& _My_data   = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;
        if (_Mylast != _My_data._Myend) {
            return _Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...);
        }
		
        _Ty& _Result = *_Emplace_reallocate(_Mylast, _STD forward<_Valty>(_Val)...);
#if _HAS_CXX17
        return _Result;
#else // ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv
        (void) _Result;
#endif // _HAS_CXX17
    }
```

我们进入_Emplace_reallocate函数。在这函数里，首先会检查size是否等于max_size，超过最大值时触发错误。接着，就看到了之前提到的扩容函数 _Calculate_growth，修改capacity的值。
```C++
template <class... _Valty>
pointer _Emplace_reallocate(const pointer _Whereptr, _Valty&&... _Val) {
    
    _Alty& _Al        = _Getal();
    auto& _My_data    = _Mypair._Myval2;
    pointer& _Myfirst = _My_data._Myfirst;
    pointer& _Mylast  = _My_data._Mylast;
	
    _STL_INTERNAL_CHECK(_Mylast == _My_data._Myend);

    const auto _Whereoff = static_cast<size_type>(_Whereptr - _Myfirst);
    const auto _Oldsize  = static_cast<size_type>(_Mylast - _Myfirst);
	
    if (_Oldsize == max_size()) {
        _Xlength();
    }
	
    const size_type _Newsize     = _Oldsize + 1;
    const size_type _Newcapacity = _Calculate_growth(_Newsize);

    const pointer _Newvec           = _Al.allocate(_Newcapacity);
    const pointer _Constructed_last = _Newvec + _Whereoff + 1;
    pointer _Constructed_first      = _Constructed_last;

    _TRY_BEGIN
    _Alty_traits::construct(_Al, _Unfancy(_Newvec + _Whereoff), _STD forward<_Valty>(_Val)...);
    _Constructed_first = _Newvec + _Whereoff;

    if (_Whereptr == _Mylast) { // at back, provide strong guarantee
        _Umove_if_noexcept(_Myfirst, _Mylast, _Newvec);
    } else { // provide basic guarantee
        _Umove(_Myfirst, _Whereptr, _Newvec);
        _Constructed_first = _Newvec;
        _Umove(_Whereptr, _Mylast, _Newvec + _Whereoff + 1);
    }
    _CATCH_ALL
    _Destroy(_Constructed_first, _Constructed_last);
    _Al.deallocate(_Newvec, _Newcapacity);
    _RERAISE;
    _CATCH_END
	
    _Change_array(_Newvec, _Newsize, _Newcapacity);
    return _Newvec + _Whereoff;
}
```


[image1]:data:image/png;base64,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