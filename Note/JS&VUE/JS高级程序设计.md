# 第一章-JS
1. JS包含ECMAscript/ES（核心），DOM（文档对象模型），BOM（浏览器对象模型）
2. DOM-文档对象模型-Document Object Model
>将整个页面抽象为分层的节点
>通过创建标签的文档树可控制网页内容与结构
3. BOM控制浏览显示页面以外的部分。
>弹窗，放大缩小
>**navigator对象，提供关于浏览器详细信息**
>**location对象，提供浏览器加载页面详细信息**
>screen对象，用户屏幕分辨率信息
>performance对象，浏览器占用内存，导航行为，时间统计等

# 第二章 HTML中的JS
1. script标签的八个属性
>async:立即下载脚本但不阻止页面其他动作。--只对外部脚本生效
>charset:指定字符集
>crossorigin:配置CORS跨资源共享
>defer:延迟到文档被完全解析与显示后执行 --只对外部脚本生效
>integrity:允许对比加密签名完整性
>language:表示代码中的脚本语言
>src:包含要执行代码的外部文件
>type:代替language，通常为："text/javascript"

2. 使用\<script>两种方式：
> 网页中嵌入
```Javascript
<script>
    function sayHi(){
        console.log("Hi");
    }   
</script>
//首先加载，在其解析完成前，网页其他内容不会被加载显示渲染
```
>外部代码引入
```javascript
<script src = "example.js"></script>
//同样会产生阻塞
//src可以指向外部域乃至一个url
<script src = "www.baidu.com/a.js"></script>
```
3. 浏览器会根据script出现的先后顺序进行解析，第一个解析完成，解析第二个。。。后一个解析必须等前一个解析完成。
4. JS引用通常放在body 所有标签的后面。
5. defer立即下载，延迟执行
   async不必等下载完脚本再加载页面，也不必加载完页面再下载脚本
```javascript
<script defer src="a.js"></script>
<script asnyc src="a.js"></script>
<script asnyc="asnyc" src="a.js"></script><!--XHTML-->
```
6. 动态加载JS
通过DOM API 创建一个script元素并添加到DOM
```javascript
let script = document.createElement('script');
script.src = 'a.js';
//script.async = fasle; 设置为同步加载
document.head.appendChild(script);//默认异步加载
```
7. strict 严格模式
   将严格判断语法,用法类似预编译
```js
function dosomething(){
    "use strict"
    //
}
```

# 第三章-语言基础
1. 变量的声明使用：var, let, constant
2. var 声明变量类型为undefined ,可以改变值及类型
```js
var message = "jjjjj";
message = 10;

function test(){
    message = "ikjj"; //声明全局变量、、局部作用域中定义全局变量，不推荐
}

function test1(){
    console.log(rte);
    var rte = 26;
}//不会报错，js会将其升至函数作用域顶部
/*function test1(){
    var rte = 26;
    console.log(rte);
}*/
```
3. let与var的区别在于let声明的范围是块作用域，var声明的为函数作用域, var 允许冗余声明，let不允许.**两个声明并不声明类型，只是限定作用域**
```js
if(true){
    let v = 1;
    let a;
    let a;//err
}
console.log(v);//err v没有定义

if(true){
    var v = 1;
    var a;
    var a;
}
console.log(v);//1
```
4. 此外let不存在作用域内提升（暂时死区）
```js
if(true){
    console.log(a);//err a未定义
    let a = 1;
}
```
5. 此外，let声明的全局变量不会成为Window对象的属性
```js
var name = 'matt';
console.log(window.name); 

let na = 26;
console.log(window.na);//err undefined
```
6. 条件声明的区别
```js
<script>
    var age = 26;
    let name = 'hh';
</script>

<script>
    //假设脚本不确定是否声明过，可以假设还未声明
    var age = 23;
    //没有问题，可作为提升声明处理，不需检查是否声明过
    let name = 'kj';
    //若之前声明过，会报错

    //使用 try catch也不行
    try{
        console.log(name);
    }catch(error){
        let name;
        //name 被限制在catch{}作用域内
    }
    name = 26;//，因此该声明相当于**全局赋值**
</script>
```

7. for循环中使用let做到块内声明作用域限制在块中，而使用var做for循环变量，容易将其变为全局变量而遭到修改
```js
for(let i = 0; i < 5; ++i){

}

//此处var 值可能不会根据for循环来进行，第一次循环之后，i更改为5， 不会进行改变。
for(var i = 0; i < 6; ++i){

}
```
8. const 与let 用法基本一致，除：const 必须赋初值，且不能更改。const 不允许重复声明，作用域也是块。
9. const 只适用于指向的变量的引用，对于引用对象，对象内属性的修改不违反const限制。
```js
const person = {};
person.name = 'dd';
```
10. 现有var后有let, const,推荐减少使用var

## 数据类型
1. 数据类型：
undefined：未定义
boolean：布尔
string：串
number：数值
object：对象
function：函数
symbol：符号
typeof(null) 返回为"object"，null默认为对空对象的引用。
undefined是一个加值，声明但为定义会设置为undefined。

2.  对于==，null==undefined会返回TRUE
3.  true和false 区分大小写。
4.  let f = 1.2e4; 
5.  number类型包含浮点数，不要测试某个特定的浮点值。**计算机无法表示$1\over3$**
6.  isFinite()判断是否为无穷，Number.MIN_VALUE, Number.MAX_VALUE, isNan()判断是否为数字;
7.  数值转换：
number(),parseInt(),parseFloat()
8.  null与undefined没有toString()方法，要使用String()方法，本质为，如果有toString方法就调用，没有就输出字面值。
9.  字符串不可变，一旦创建，无法改变，只有先销毁，后重建。
10. 模板字面量插值通过在\${}中使用一个js句法表达式来实现。
11. string.raw'\u009ssdd'会保留原始\u
## symbol类型
1. 符号是原始值，且唯一 不可变，确保对象属性使用唯一标识符不会发生属性冲突。
### 基本用法
2. 符号需要使用Symbol()函数来初始化，符号本身是原始类型因此typeof返回为symbol
3. 调用符号函数时，可传入一个字符串参数作为对符号的描述，将来可使用该字符串调试。但该字符串与符号定义或标识无关。
4. **符号没有字面量语法**，只要创建Symbol()示例并将其用作对象的新属性，就可以保证它不会覆盖已有的对象属性，无论是符号属性还是字符串属性。
5. **Symbol()不能与new一起作为构造函数使用，避免创建符号包装对象**
### 使用全局符号注册表
1. 如果运行时的不同部分需要共享和重用符号实例，那么可以用一个字符串作为键，在全局符号注册表中创建并重用符号。使用Symbol.for()方法。发现符号实例则返回，否则创建并返回。
2. 全局注册表中的符号与Symbol()生成的符号不相同。
3. Symbol.for()只接受字符串参数非字符串参数会自动转化为字符串参数。
4. 前缀@@：如：@@iterator -> Sybmol.iterator
### Object.getOwnPropertyNames(),Object.getOwnPropertySymbols()
1. 前者返回同时包含常规和符号属性描述符的对象，后者返回对象示例的符号属性数组。

## object 类型
1. 本质是一组数据和功能的集合。
2. Object也是派生其他类的基类
3. 基本属性与方法
>- constructor：创建当前对象的函数，这个属性的值就是Object()函数。
>- hasOwnProperty(propertyName):判断当前对象实例是否存在给定属性。(属性名必须为字符串)
>- isPrototypeOf(object):判断当前对象是否是另一对象原型。
>- propertyIsEnumberable(propertyName):判断给定的属性是否可以使用for-in来枚举。(属性名必须为字符串)
>-toLocaleString():返回对象的字符串表示，该字符串反映对象所在的本地化执行环境。
>-toString():返回对象的字符串表示。
>-valueOf():返回对象对应的字符串、数值或者布尔值表示。通常与toString()相同。

4. BOM与DOM对象不一定继承Object。不同浏览器不同。

## 操作符
1. ** 为指数操作符
2. 全等于不全等
>- \=\=\=与!\=\=\= 在不转换操作数情况下比较相等与否。
>- \=\=操作符会转换操作数："5"与5相等。

3. for-in语句
**严格的迭代语句，枚举对象中的非符号键属性，不保证返回值顺序**：
for(property in expression) statement
for(const propName in window){
    document.write(propName);
}//循环显示BOM对象window的所有属性。

4. for-of语句
**严格的迭代语句，用于遍历可迭代对象的元素，会按照可迭代元素的next()方法产生值的顺序迭代元素**。
for(property in expression) statement
for(const el of [2,4,6,8]){
    document.write(el);
}//如果尝试迭代的变量不支持迭代，则for-of语句会抛错。

5. with语句
**将代码作用域设置为特定的对象,用于针对一个对象的反复操作**。
```js
with (expression) statement;
let qs = location.search.substring(1);
let hostName = location.hostname;
let url = location.href;

with(location){
    let qs = search.substring(1);
    let hostName = hostname;
    let url = href;
}
//with用于连接location对象。
```
6. Switch语句
switch(expression){
    case value1:
        statement
        break;
    case value2:
        statement
        break;
    default:
        statement
}
**switch可以用于所有数据类型，甚至字符串及对象，条件的值不需要是常量，也可以是变量甚至表达式**

7. 函数
函数不需要指定返回值，可以在任何需要的地方进行返回。return也可以不带任何值返回。

# 第四章-变量、作用域及内存
1. 变量包含原始值（最简单的数据）以及引用值（多个值构成的对象）。
2. 保存原始值的变量是按值访问的，操作的就是存储在变量中的实际值。
3. JS不允许直接操纵内存，操作对象时实际是操作对象的引用。
4. string类型是原始值。
5. 


# ku 12-20