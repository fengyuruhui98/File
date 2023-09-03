一句话概括：for in是遍历（object）键名，for of是遍历（array）键值。

# for...in
for...in 循环只遍历可枚举属性（包括它的原型链上的可枚举属性）。像 Array和Object使用内置构造函数所创建的对象都会继承自Object.prototype和String.prototype的不可枚举属性，例如 String 的 indexOf()  方法或 Object的toString()方法。循环将遍历对象本身的所有可枚举属性，以及对象从其构造函数原型中继承的属性（更接近原型链中对象的属性覆盖原型属性）。
```js
var obj = {a:1, b:2, c:3};
    
for (let key in obj) {
  console.log(key);
}

// a
// b
// c
```
# for...of
for...of语句在可迭代对象（包括Array，Map，Set，String，TypedArray，arguments 对象等等）上创建一个迭代循环，调用自定义迭代钩子，并为每个不同属性的值执行语句
```js
const array1 = ['a', 'b', 'c'];

for (const val of array1) {
  console.log(val);
}

// a
// b
// c
```
for of不可以遍历普通对象，想要遍历对象的属性，可以用for in循环, 或内建的Object.keys()方法

# for...of与for...in的区别
无论是for...in还是for...of语句都是迭代一些东西。它们之间的主要区别在于它们的迭代方式。

for...in语句**以任意顺序迭代对象的可枚举属性**。

for...of 语句**遍历可迭代对象定义要迭代的数据**。

以下示例显示了与Array一起使用时，for...of循环和for...in循环之间的区别。
```js
Object.prototype.objCustom = function() {}; 
Array.prototype.arrCustom = function() {};

let iterable = [3, 5, 7];
iterable.foo = 'hello';

for (let i in iterable) {
  console.log(i); // 0, 1, 2, "foo", "arrCustom", "objCustom"
}

for (let i in iterable) {
  if (iterable.hasOwnProperty(i)) {
    console.log(i); // 0, 1, 2, "foo"
  }
}

for (let i of iterable) {
  console.log(i); // logs 3, 5, 7
}
```
总结：for in 一般用来遍历对象的key、for of 一般用来遍历数组的value