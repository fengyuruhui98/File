# string转int
```c#
int intA = 0;
intA =int.Parse(str);//1
int.TryParse(str, out intA);//2
intA = Convert.ToInt32(str);//3
//以上都可以，其中 1和3 需要try｛｝异常，2不需要。

int i = -1;
bool b = int.TryParse(null, out i);
//执行完毕后，b等于false，i等于0，而不是等于-1，切记。

int i = -1;
bool b = int.TryParse("123", out i);
//执行完毕后，b等于true，i等于123；
```
1、(int)是一种类型转换；当int类型到long,float,double,decimal类型，可以使用隐式转换，但是当我们从long类型到int类型就需要使用显式转换，否则会产生编译错误。

2、int.Parse()是一种类容转换；表示将数字内容的字符串转为int类型。

如果字符串为空，则抛出ArgumentNullException异常；

如果字符串内容不是数字，则抛出FormatException异常；

如果字符串内容所表示数字超出int类型可表示的范围，则抛出OverflowException异常；

3、int.TryParse与 int.Parse 又较为类似，但它不会产生异常，转换成功返回 true，转换失败返回 false。

最后一个参数为输出值，如果转换失败，输出值为 0，如果转换成功，输出值为转换后的int值

4、Convert.ToInt32()是一种类容转换；但它不限于将字符串转为int类型，还可以是其它类型的参数；

比较：Convert.ToInt32 参数为 null 时，返回 0； int.Parse 参数为 null 时，抛出异常。Convert.ToInt32 参数为 “” 时，抛出异常； int.Parse 参数为 “” 时，抛出异常。 Convert.ToInt32 可以转换的类型较多； int.Parse 只能转换数字类型的字符串。int.TryParse相比较于以上两种方法则更加安全，不会报错，在某些情况下好用很多！

# int转string
```c#
int a = 15;
string s1 = a.ToString();
string s2 = Convert.ToString(a);
```