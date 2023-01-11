# Markdown插入数学公式

- [Markdown插入数学公式](#markdown插入数学公式)
- [1.在文本中插入数学公式](#1在文本中插入数学公式)
  - [同行插入公式](#同行插入公式)
  - [换行插入公式](#换行插入公式)
- [2.上标或下标](#2上标或下标)
  - [编辑单个上标或者下标](#编辑单个上标或者下标)
  - [编辑双上标或者下标(下/上标为公式)](#编辑双上标或者下标下上标为公式)
- [3.编辑数学符号](#3编辑数学符号)
  - [数学符号](#数学符号)
- [4.编号](#4编号)
- [5.方程组 表达式 推导过程](#5方程组-表达式-推导过程)
  - [删除符号](#删除符号)
  - [左侧花括号](#左侧花括号)
  - [方程组](#方程组)
  - [公式推导](#公式推导)
- [6.编辑特殊符号](#6编辑特殊符号)
  - [空心字母与Fraktur字母](#空心字母与fraktur字母)
  - [常见希腊字母表](#常见希腊字母表)
- [7.常用特殊字符表](#7常用特殊字符表)


# 1.在文本中插入数学公式
## 同行插入公式
就像这样 

$f(x)=ax^2+bx+c$
```
$写你的公式$ 
```
## 换行插入公式
就像这样:

```md
$$f(x)=ax^2+bx+c$$
$$公式$$
```
两个$$使公式居中

# 2.上标或下标
## 编辑单个上标或者下标
编辑公式上标使用用 ^ ，下标用_ 。
例如：  $x^2\&x_2$ 
```md
$x^2\&x_2$
```
## 编辑双上标或者下标(下/上标为公式)
如果要在公式使用双下/上标或者上/下标插入公式，将上标/下标用{ }括起来即可
例如：  

$e^{ln(x)}=x$
```md
$e^{ln(x)}=x$
```

# 3.编辑数学符号
## 数学符号
Name|Display|Name
-|-|-
字母上方横线|\overline{}, \bar{} |   $\overline{ABC} or A\bar{2}$
字母下方横线|	\underline{}	  |$\underline{ABC}$
字母上方波浪线	|\tilde{}, \widetilde{}	   |$\tilde{A} or \widetilde{ABC}$
字母上方尖号^	|\hat{}, \widehat{}|$\hat{A} or \widehat{ABC}$	   
字母上方箭头	|\vec{}, \overleftarrow{}, \overrightarrow{}|$\vec{b} or \overleftarrow{ab} or \overrightarrow{ab}$	   
字母上方花括号	|\overbrace{}|$\overbrace{1 + 2 + 3}$	  
下方花括号	|\underbrace{}	|$\underbrace{1 + 2 + 3}$  
字母上方点号	|\dot{}, \ddot{}|$\dot{a} \  or \  \ddot{a}$   
省略号	|\dots, \cdots	  |$1,2,\dots$
积分	|\int_{}^{}	   | $\int_{a}^{b} \ or \ \int_{-\infty}^{+\infty}f(x)dx$
双重积分	|\iint	 |$\iint_{-\infty}^{+\infty}f(x, y)dxdy$
行内积分	|\int_{-\infty}^{+\infty}f(x) \mathrm{d}x|	  $\int_{-\infty}^{+\infty}f(x)dx$
行内积分limits模式|	\int\limits_{}^{}|$\int\limits_{-\infty}^{+\infty}f{x}dx$ 
行内积分display模式	|\displaystyle \int_{}^{}|$\displaystyle \int_{-\infty}^{+\infty}f(x)dx$	  
环路积分	|\oint|$\oint_{-\infty}^{+\infty}f(x)dx$	  
求乘积	|\prod_{}^{}|$\prod_{i = 1}^{n}a_i$	  
根号	|\sqrt[n]{x}| $\sqrt[n]{x}$	
分号   |\over|${3}\over{4}$

# 4.编号  
- 插入编号：  
使用\tag指令指定公式的具体编号，并使用\label指令埋下锚点。如 ```y = x^2 \tag{(1.5a)} \label{eq:test}```:

$$y = x^2 \tag{1.5a} \label{eq1}$$

- 引用编号：
使用\eqref指令引用前面埋下的锚点，\eqref{eq:test}将显示为：

$$\eqref{eq1}$$

# 5.方程组 表达式 推导过程

## 删除符号

~~
~~删除内容~~

## 左侧花括号
$$
\begin{equation}  
% \begin{equation*} 加'*'去掉公式编号
\left \{
\begin{aligned}     %请使用'aligned'或'align*'
2x + y &= 1  \\     %加'&'指定对齐位置
2x + 2y &= 2
\end{aligned}
\right.
\end{equation}
% \end{equation*}   加'*'去掉公式编号

% 注意：在 markdown 环境下，某些特殊字符，如'\', '*'等，会首先被 markdown 语法转义，然后再被 Latex 转义。
% 因此有时候 '\{'需要写作'\\{'，'*'需要写作'\*'，'\\'需要写作'\\\\'等，视不同的解释环境而定
$$

注：如果各个方程需要在某个字符处对齐（如等号对齐），只需在所有要对齐的字符前加上 & 符号。如果不需要公式编号，只需在宏包名称后加上 * 号。

## 方程组

f(x) =  
\begin{cases}  
x^2 \qquad & a \gt 0 \\  
e^x \qquad & a \le 0  
\end{cases}

$$f(x) =
\begin{cases}
x^2 \qquad & a \gt 0 \\
e^x \qquad & a \le 0
\end{cases}$$

## 公式推导
有时一行放不下所有的推导过程，放到多行并使得每行的等号对齐可以大大增加可读性。下面这个例子原始形式是A，然后经过三步推导最终得到了D。以符号 & 的下一个字符进行对齐，末尾的三个反斜杠 \ 用来分割行。这里是由于markdown与mathjax的渲染有冲突才需要用三个反斜杠。
需要效果为

$$
\begin {aligned}
A&=B \\\
&=C \\\
&=D
\end {aligned}
$$


源码为：
```Latex
$$
\begin {aligned}
A&=B \\\
&=C \\\
&=D
\end {aligned}
$$
```


# 6.编辑特殊符号  

## 空心字母与Fraktur字母
A-Z皆可用
符号|markdown原文
-|-
A|$\mathbb{A}$
B|$\mathfrak{B}$

## 常见希腊字母表  
Name|Display|Capital Case|Display|Var Case|Display
-|-|-|-|-|-
\alpha	|$\alpha$	
\beta	|$\beta$				
\gamma	|$\gamma$|\Gamma|$\Gamma$			
\theta	|$\theta$|	\Theta|$\Theta$|		\vartheta	|$\vartheta$
\mu		|$\mu$			
\delta	|$\delta$	|\Delta		|$\Delta$	
\epsilon|$\epsilon$|			|	|\varepsilon	|$\varepsilon$
\sigma	|$\sigma$	|\Sigma	|$\Sigma$	|\varsigma	|$\varsigma$
\pi	|$\pi$|	\Pi|$\Pi$|		\varpi|$\varpi$	
\omega|$\omega$	|	\Omega		|$\Omega$	
\xi	|$\xi$|	\Xi|$\Xi$			
\zeta	|$\zeta$				
\chi	|$\chi$				
\rho	|$\rho$|	|	|	\varrho|$\varrho$	
\phi	|	\Phi		\varphi	
\eta	|$\eta$				
\lambda	|$\lambda$|	\Lambda|	$\Lambda$		
\kappa	|$\kappa$				
\nu		|$\nu$			
\upsilon|$\upsilon$|\Upsilon|$\upsilon$			
\psi|$\psi$|		\Psi|$\Psi$			
\tau|$\tau$					
\iota|$\iota$					
o|$o$

# 7.常用特殊字符表  
Name|Display|Name|Display|Name|Display|Name|Display  
-|-|-|-|-|-|-|-
\times|$\times$|\div|$\div$|\pm|$\pm$|\mp|$\mp$	
\otimes|$\otimes$|\ominus|$\ominus$|\oplus|$\oplus$|\odot|$\odot$	
\oslash|$\oslash$|\triangleq|$\triangleq$|\ne|$\ne$|\equiv|$\equiv$	
\lt|$\lt$|\gt|$\gt$|\le|$\le$|\ge|$\ge$	
\cup|$\cup$|\cap|$\cap$|\Cup|$\Cup$|\Cap|$\Cap$	
\bigcup|$\bigcup$|\bigcap|$\bigcap$|\ast|$\ast$|\star|$\star$	
\bigotimes|$\bigotimes$|\bigoplus|$\bigoplus$|\circ|$\circ$|\bullet|$\bullet$	
\bigcirc|$\bigcirc$|\amalg|$\amalg$|\to|$\to$|\infty|$\infty$	
\vee|$\vee$|\wedge|$\wedge$|\lhd|$\lhd$|\rhd|$\rhd$	
\bigvee|$\bigvee$|\bigwedge|$\bigwedge$|\unlhd|$\unlhd$|\unrhd|$\unrhd$	
\sqcap|$\sqcap$|\sqcup|$\sqcup$|\prec|$\prec$|\succ|$\succ$	
\subset|$\subset$|\supset|$\supset$||$\sim$|\approx|$\approx$	
\subseteq|$\subseteq$|\supseteq|$\supseteq$|\cong|$\cong$|\doteq|$\doteq$	
\setminus|$\setminus$|\mid|$\mid$|\ll|$\ll$|\gg|$\gg$	
\parallel|$\parallel$|\Join|$\Join$|\in|$\in$|\notin|$\notin$	
\propto|$\propto$|\neg|$\neg$|\ldots|$\ldots$|\cdots|$\cdots$	
\forall|$\forall$|\exists|$\exists$|\vdots|$\vdots$|\ddots|$\ddots$	
\aleph|$\aleph$|\nabla|$\nabla$|\imath|$\imath$|\jmath|$\jmath$	
\ell|$\ell$|\partial|$\partial$|\int|$\int$|\oint|$\oint$	
\uplus|$\uplus$|\biguplus|$\biguplus$|

其他  
Name|Display|Name|Display 
-|-|-|- 
\triangleleft|$\triangleleft$|\triangleright|$\triangleright$  	  
\bigtriangleup|$\bigtriangleup$|\bigtriangledown|$\bigtriangledown$   
\uparrow|$uparrow$|\downarrow|$\downarrow$	
\leftarrow|$\leftarrow$|\rightarrow|$\rightarrow$	
\Leftarrow|$\Leftarrow$|\Rightarrow|$\Rightarrow$	
\longleftarrow|$\longleftarrow$|\longrightarrow|$\longrightarrow$	
\Longleftarrow|$\Longleftarrow$|\Longrightarrow|$\Longrightarrow$	
\leftrightarrow|$\leftrightarrow$|\longleftrightarrow|$\longleftrightarrow$	
\Leftrightarrow|$\Leftrightarrow$|\Longleftrightarrow|$\Longleftrightarrow$	
\leftharpoonup|$\leftharpoonup$|\rightharpoonup|$\rightharpoonup$	
\leftharpoondown|$\leftharpoondown$|\rightharpoondown|$\rightharpoondown$	
\rightleftharpoons|$\rightleftharpoons$|\S|$\S$	
\nwarrow|$\nwarrow$|\nearrow|$\nearrow$	
\swarrow|$\swarrow$|\searrow|$\searrow$	
\triangle|$\triangle$|\Box|$\Box$	
\diamond|$\diamond$|\diamondsuit|$\diamondsuit$	
\heartsuit|$\heartsuit$|\clubsuit|$\clubsuit$	
\spadesuit|$\spadesuit$	

在线latex数学公式
利用“[在线latex数学公式](http://latex.codecogs.com/eqneditor/editor.php)”来帮助生成数学公式代码，可以帮助我们更快的完成公式的书写。

作者：阿长啊  
链接：https://www.jianshu.com/p/70917dab822b
