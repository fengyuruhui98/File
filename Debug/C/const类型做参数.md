# C语言编译报错error: variably modified ‘***’ at file scope

## 报错原因
在数组声明中使用了只读类型。  
造成此报错的原因是由于使用了类似一下的代码
```c
const int length = 256;
char buffer[length] = {0};
```
在C语言中，const不是一个真真正正的常量，其代表的含义仅仅是只读。使用const声明的对象是一个运行时对象，无法使用其作为某个量的初值、数组的长度、case的值或在类型的情形中使用。例如
```
//注释中为报错信息
const int length = 256;
char buzzer[length];        //error: variably modified ‘buffer’ at file scope
int i = length;             //error: initializer element is not constant

switch (x) {
case length:            //error: case label does not reduce to an integer constant
	/* code */
	 break;
default:
	 break;
}
```
解决方法  
使用宏定义#define代替只读类型const例如
```
//解决报错
#define LENGTH 256
char buzzer[LENGTH];        //error: variably modified ‘buffer’ at file scope
int i = LENGTH;             //error: initializer element is not constant

switch (x) {
case length:            //error: case label does not reduce to an integer constant
	/* code */
	 break;
default:
	 break;
```
#define和const的不同  
由const修饰的类型在内存中占空间而#define不占空间，#define只是在编译前将要编译的源文件中相应的部分使用字符串替换例如前面的代码在编译前会被预处理为
```c
char buzzer[256];       
int i = 256;            

switch (x) {
case 256:           
	/* code */
	 break;
default:
	 break;
}
```