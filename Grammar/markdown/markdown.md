# makedown 语法
## 一、标题语法
### （1）三级标题，中间要有空格
## 二、段落语法
### （1）段落：使用空白行将一行或多行文本进行分割

不要使用空格与缩进符号
## 三、换行
在结尾两个空格  
之后按回车  
不按两个空格就回车会换行失败  
此外还可以使用</br>来进行兼容
## 四、粗体与斜体
以下是**粗体**，以下是*斜体*</br>以下是***粗体加斜体***
## 五、引用语法
### （1）引用
>一般引用
### （2）多个段落的引用
>第一个段落

>第二个段落

>第三个段落
### （3）嵌套块引用
>块引用可以嵌套。在要嵌套的段落前添加一个>>符号
>>嵌套块。
### （4）带有其他元素的块引用
块引用可以包含其他Markdown格式的元素。并非所有元素都可以使用，需要进行试验查看哪些元素有效。
> ## 块引用中的高级标题
> -第一条  
> -第二条  
> *斜体*与**粗体**

## 六、列表语法
### （1）有序列表
1. 第一条
2. 第二条
3. 第三条
   1. 第3.1条
   2. 第3.2条
### （2）无序列表
- 第一条
* 第二条
+ 第三条
    - 第一条
    - 第二条
    - 第三条

### （3）列表中嵌套其他元素
* 要在保留列表连续性的同时

    在列表中添加另一元素，

* 将该元素缩进四个空格或一个制表符</br>

引用块：

* 要在保留列表连续性的同时

    >在列表中添加另一元素，

* 将该元素缩进四个空格或一个制表符</br>

代码块：

代码块通常采用四个空格或者一个制表符缩进。当他们被放在列表中时，将它们缩进八个空格或者两个制表符</br>
        
        #include <stdio.h>
        using namespace std;
        int main(){
            printf("Markdown!");
        }

## 七、代码语法
### （1）要将单词或短语表示为代码，将其包裹在反引号(`)中
变量`place`  
转义反引号用两个反引号``place in `C` ``
## 八、分割线语法
要创建分割线，在单独一行上使用三个或多个星号、破折号、或下划线并且不能包含其他内容
###
---
____________________________
## 九、链接语法
链接文本放在中括号内链接地址放在后面的括号中，链接title可选
超链接Markdown语法代码：[Markdown语法](https://markdown.com.cn).</br>
### （1）给链接增加title
当鼠标悬停在链接上会出现的文字，这个title是可选的，它放在圆括号中链接地址后面，跟链接地址之间以空格分隔。</br>
[Markdown语法](https://markdown.com.cn "最好的Markdown教程")

### （2）网址与email地址
使用尖括号可以很方便地把URL或者email变为可点击的链接</br>
<htps://markdown.com.cn></br>
<fake@example.com>

### （3）带格式化的链接
强调 ***[链接](https://eff.org)*** ，在链接语法前后加星号。要将链接表示为代码，在方括号中添加反引导
### （4）引用类型链接
引用样式链接是一种特殊的链接，它使URL在Markdown中更易于显示和阅读。参考样式链接分为两部分：与文本保持内联的部分以及存储在文件中其他位置的部分，以使文本易于阅读。
#### 链接的第一部分格式
引用类型的链接的第一部分使用两组括号进行格式设置。第一组方括号包围应显示为链接的文本。第二组括号显示了一个标签，该标签用于指向您存储在文档其他位置的链接。  
尽管不是必需的，可以在第一组和第二组括号之间包含一个空格。第二组括号中的标签不区分大小写，可以包含字母，数字，空格或标点符号。  
以下示例格式对于链接的第一部分效果相同：
- [hobbit-hole][1]
- [hobbit-hole] [1]
#### 链接的第二部分格式
引用类型链接的第二部分使用以下属性设置格式：
1. 放在括号中的标签，其后紧跟一个冒号和至少一个空格（例如[label]:）。
2. 链接的URL，可以选择将其括在尖括号中。
3. 链接的可选标题，可以将其括在双引号，单引号或括号中。
以下示例格式对于链接的第二部分效果相同：  
- [1]: https://en.wikipedia.org/wiki/Hobbit#Lifestyle
- [1]: https://en.wikipedia.org/wiki/Hobbit#Lifestyle "Hobbit lifestyles"
- [1]: https://en.wikipedia.org/wiki/Hobbit#Lifestyle 'Hobbit lifestyles'
- [1]: https://en.wikipedia.org/wiki/Hobbit#Lifestyle (Hobbit lifestyles)
- [1]: <https://en.wikipedia.org/wiki/Hobbit#Lifestyle> "Hobbit lifestyles"
- [1]: <https://en.wikipedia.org/wiki/Hobbit#Lifestyle> 'Hobbit lifestyles'
- [1]: <https://en.wikipedia.org/wiki/Hobbit#Lifestyle> (Hobbit lifestyles)
可以将链接的第二部分放在Markdown文档中的任何位置。有些人将它们放在出现的段落之后，有些人则将它们放在文档的末尾（例如尾注或脚注）。  

------------------------------------------------------
## 十、图片语法
### 添加图像 使用（！），在方括号里增加替代文本，图片链接放在圆括号里，括号里的链接后可以增加一个可选的图片标题文本。
```
![这是图片](/assets/img/philly-magic-garden.jpg "Magic Gardens")

[![沙漠中的岩石图片](/assets/img/shiprock.jpg "Shiprock")](https://markdown.com.cn)
```
## 十一、转义字符语法
要显示原本用于格式化MarkDown文档的字符，在字符前添加反斜杠\、
```
\* Without the backslash, this would be a bullet in an unordered list.
```
### 特殊字符自动转义
在 HTML 文件中，有两个字符需要特殊处理： < 和 & 。 < 符号用于起始标签，& 符号则用于标记 HTML 实体，如果你只是想要使用这些符号，你必须要使用实体的形式，像是 \&lt; 和 \&amp;。

& 符号其实很容易让写作网页文件的人感到困扰，如果你要打「AT&T」 ，你必须要写成「AT\&amp;T」 ，还得转换网址内的 & 符号，如果你要链接到：
```
http://images.google.com/images?num=30&q=larry+bird  
```
你必须要把网址转成：  
```
http://images.google.com/images?num=30&amp;q=larry+bird  
```
才能放到链接标签的 href 属性里。不用说也知道这很容易忘记，这也可能是 HTML 标准检查所检查到的错误中，数量最多的。

Markdown 允许你直接使用这些符号，它帮你自动转义字符。如果你使用 & 符号的作为 HTML 实体的一部分，那么它不会被转换，而在其它情况下，它则会被转换成 \&amp;。所以你如果要在文件中插入一个著作权的符号，你可以这样写：

\&copy;

Markdown 将不会对这段文字做修改，但是如果你这样写：

AT&T  
Markdown 就会将它转为：

AT\&amp;T    

类似的状况也会发生在 < 符号上，因为 Markdown 支持 行内 HTML ，如果你使用 < 符号作为 HTML 标签的分隔符，那 Markdown 也不会对它做任何转换，但是如果你是写：

4 < 5
Markdown 将会把它转换为：

4 \&lt; 5
需要特别注意的是，在 Markdown 的块级元素和内联元素中， < 和 & 两个符号都会被自动转换成 HTML 实体，这项特性让你可以很容易地用 Markdown 写 HTML。（在 HTML 语法中，你要手动把所有的 < 和 & 都转换为 HTML 实体。）

## 十二、内嵌HTML标签
对于 Markdown 涵盖范围之外的标签，都可以直接在文件里面用 HTML 本身。如需使用 HTML，不需要额外标注这是 HTML 或是 Markdown，只需 HTML 标签添加到 Markdown 文本中即可。
### 行级内联标签
HTML 的行级內联标签如 \<span>、\<cite>、\<del> 不受限制，可以在 Markdown 的段落、列表或是标题里任意使用。依照个人习惯，甚至可以不用 Markdown 格式，而采用 HTML 标签来格式化。例如：如果比较喜欢 HTML 的 \<a> 或 \<img> 标签，可以直接使用这些标签，而不用 Markdown 提供的链接或是图片语法。当你需要更改元素的属性时（例如为文本指定颜色或更改图像的宽度），使用 HTML 标签更方便些。
HTML 行级內联标签和区块标签不同，在內联标签的范围内， Markdown 的语法是可以解析的。  
This **word** is bold. This <em>word</em> is italic.

### 区块标签
区块元素──比如 \<div>、\<table>、\<pre>、\<p> 等标签，必须在前后加上空行，以便于内容区分。而且这些元素的开始与结尾标签，不可以用 tab 或是空白来缩进。Markdown 会自动识别这区块元素，避免在区块标签前后加上没有必要的 \<p> 标签。

例如，在 Markdown 文件里加上一段 HTML 表格：

This is a regular paragraph.

<table>
    <tr>
        <td>Foo</td>
    </tr>
</table>

This is another regular paragraph.  
请注意，Markdown 语法在 HTML 区块标签中将不会被进行处理。例如，你无法在 HTML 区块内使用 Markdown 形式的*强调*。