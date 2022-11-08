# Markdown插入数学公式

# 在文本中插入数学公式
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
# 上标或下标
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

# 编辑数学符号
## 数学符号
Name	Display	Name
字母上方横线	$\overline{hh}$, $\bar{hh}$ or    
字母下方横线	$\underline{jj}$	  
字母上方波浪线	$\tilde{s}$, \widetilde{}	 or   
字母上方尖号^	$\hat{g}$, \widehat{}	 or   
字母上方箭头	$\vec{g}$, \overleftarrow{}, \overrightarrow{}	 or  or   
字母上方花括号	$\overbrace{s}$	  
下方花括号	\underbrace{}	  
字母上方点号	\dot{}, \ddot{}	 or   
省略号	\dots, \cdots	  
积分	$\int_{2}^{3}$	   
双重积分	$\iint$	  
行内积分	$\int_{-\infty}^{+\infty}f(x)$ \mathrm{d}x	  
行内积分limits模式	$\int\limits_{4}^{5}$	  
行内积分display模式	\displaystyle \int_{}^{}	  
环路积分	$\oint$	  
求乘积	$\prod_{}^{}$	  
根号	$\sqrt[n]{x}$	  
# 编号  
插入编号：  
使用\tag指令指定公式的具体编号，并使用\label指令埋下锚点。如y=x^2 \tag{1.5a} \label{eq:test}：


引用编号：
使用\eqref指令引用前面埋下的锚点，\eqref{eq:test}将显示为：


# 方程组
左侧花括号

\begin{equation}  
% \begin{equation*} 加'*'去掉公式编号
\left\{
\begin{aligned}     %请使用'aligned'或'align*'
2x + y &= 1  \\     %加'&'指定对齐位置
2x + 2y &= 2
\end{aligned}
\right.
\end{equation}
% \end{equation*}   加'*'去掉公式编号

% 注意：在 markdown 环境下，某些特殊字符，如'\', '*'等，会首先被 markdown 语法转义，然后再被 Latex 转义。
% 因此有时候 '\{'需要写作'\\{'，'*'需要写作'\*'，'\\'需要写作'\\\\'等，视不同的解释环境而定


注：如果各个方程需要在某个字符处对齐（如等号对齐），只需在所有要对齐的字符前加上 & 符号。如果不需要公式编号，只需在宏包名称后加上 * 号。

分情况讨论方程式

f(x) =
\begin{cases}
x^2 \qquad & a \gt 0 \\
e^x \qquad & a \le 0
\end{cases}




编辑特殊符号  
常见希腊字母表  
Name	Display	Capital Case	Display	Var Case	Display
\alpha					
\beta					
\gamma		\Gamma			
\theta		\Theta		\vartheta	
\mu					
\delta		\Delta			
\epsilon				\varepsilon	
\sigma		\Sigma		\varsigma	
\pi		\Pi		\varpi	
\omega		\Omega			
\xi		\Xi			
\zeta					
\chi					
\rho				\varrho	
\phi		\Phi		\varphi	
\eta					
\lambda		\Lambda			
\kappa					
\nu					
\upsilon		\Upsilon			
\psi		\Psi			
\tau					
\iota					
o		  

常用特殊字符表  
Name	Display	Name	Display	Name	Display	Name	Display  

$\times$	

$\div$

$\pm$		

$\mp$

$\otimes$

$\ominus	  
 \oplus		\odot	
\oslash		\triangleq		\ne		\equiv	
\lt		\gt		\le		\ge	
\cup		\cap		\Cup		\Cap	
\bigcup		\bigcap		\ast		\star	
\bigotimes		\bigoplus		\circ		\bullet	
\bigcirc		\amalg		\to		\infty	
\vee		\wedge		\lhd		\rhd	
\bigvee		\bigwedge		\unlhd		\unrhd	
\sqcap		\sqcup		\prec		\succ	
\subset		\supset		\sim		\approx	
\subseteq		\supseteq		\cong		\doteq	
\setminus		\mid		\ll		\gg	
\parallel		\Join		\in		\notin	
\propto		\neg		\ldots		\cdots	
\forall		\exists		\vdots		\ddots	
\aleph		\nabla		\imath		\jmath	
\ell		\partial		\int		\oint	
\uplus		\biguplus	$			


其他  
Name	Display	Name	Display  
$\triangleleft$	  
$\triangleright$  	  
$\bigtriangleup$    
$\bigtriangledown$    	
\uparrow		\downarrow	
\leftarrow		\rightarrow	
\Leftarrow		\Rightarrow	
\longleftarrow		\longrightarrow	
\Longleftarrow		\Longrightarrow	
\leftrightarrow		\longleftrightarrow	
\Leftrightarrow		\Longleftrightarrow	
\leftharpoonup		\rightharpoonup	
\leftharpoondown		\rightharpoondown	
\rightleftharpoons		\S	
\nwarrow		\nearrow	
\swarrow		\searrow	
\triangle		\box	
\diamond		\diamondsuit	
\heartsuit		\clubsuit	
\spadesuit	$

在线latex数学公式
利用“在线latex数学公式”来帮助生成数学公式代码，可以帮助我们更快的完成公式的书写。

作者：阿长啊
链接：https://www.jianshu.com/p/70917dab822b
