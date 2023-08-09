# 基础内容 1-11
1. JS包含ECMAscript/ES（核心），DOM（文档对象模型），BOM（浏览器对象模型）
2. 使用<script>两种方式：
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
8. 变量的声明使用：var, let, constant
9. var 声明变量类型为undefined ,可以改变值及类型
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
10. let与var的区别在于let声明的范围是块作用域，var声明的为函数作用域, var 允许冗余声明，let不允许.**两个声明并不声明类型，只是限定作用域**
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
11. 此外let不存在作用域内提升（暂时死区）
```js
if(true){
    console.log(a);//err a未定义
    let a = 1;
}
```
12. 此外，let声明的全局变量不会成为Window对象的属性
```js
var name = 'matt';
console.log(window.name); 

let na = 26;
console.log(window.na);//err undefined
```
13. 条件声明的区别
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

14. for循环中使用let做到块内声明作用域限制在块中，而使用var做for循环变量，容易将其变为全局变量而遭到修改
```js
for(let i = 0; i < 5; ++i){

}

//此处var 值可能不会根据for循环来进行，第一次循环之后，i更改为5， 不会进行改变。
for(var i = 0; i < 6; ++i){

}
```
15. const 与let 用法基本一致，除：const 必须赋初值，且不能更改。const 不允许重复声明，作用域也是块。
16. const 只适用于指向的变量的引用，对于引用对象，对象内属性的修改不违反const限制。
```js
const person = {};
person.name = 'dd';
```
17. 现有var后有let, const,推荐减少使用var
18. 数据类型：
undefined：未定义
boolean：布尔
string：串
number：数值
object：对象
function：函数
symbol：符号
typeof(null) 返回为"object"，null默认为对空对象的引用。
undefined是一个加值，声明但为定义会设置为undefined。
19. 对于==，null==undefined会返回TRUE
20. true和false 区分大小写。
21. let f = 1.2e4; 
22. isFinite()判断是否为无穷，Number.MIN_VALUE, Number.MAX_VALUE;
23. 数值转换：
number(),parseInt(),parseFloat()
24. null与undefined没有toString()方法，要使用String()方法，本质为，如果有toString方法就调用，没有就输出字面值。
25. 模板字面量插值通过在\${}中使用一个js句法表达式来实现。
26. string.raw'\u009ssdd'会保留原始\u
27. 

# ku 12-20