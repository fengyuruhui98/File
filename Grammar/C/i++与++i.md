Ubuntu版本：ubuntu-gnome-16.04-desktop-amd64，gnome版

gcc：gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4)

-----------------------------------------------------------------------------------

 

**对于++j：当一行表达式中的++j全部计算完毕之后，再把表达式中的所有j全部替换成更新之后的值。**
**对于j++：尽量不要在同一个表达式中使用了j++后再使用含j的表达式，结果可能因编译器不同而不同。**

 

1. a[j++]=j++;
```c
void main(){
 
int j = 0;
int a[5] = {5,5,5,5,5};
a[j++]=j++;
 
 
printf("j = %d\n", j );
for(int i = 0; i < 5; i++)
printf("a[%d] = %d\n", i, a[i] );
 
}
```
结果：a[0] = 1, j = 2



 

2. a[j++] = j;
```c
void main(){
 
int j = 0;
int a[5] = {5,5,5,5,5};
a[j++] = j;
 
printf("j = %d\n", j );
for(int i = 0; i < 5; i++)
    printf("a[%d] = %d\n", i, a[i] );
    
}
```
结果：a[0] = 1, j = 1

 

3. a[j] = j ++;
```c
void main(){
 
int j = 0;
int a[5] = {5,5,5,5,5};
a[j] = j ++;
 
 
printf("j = %d\n", j );
for(int i = 0; i < 5; i++)
    printf("a[%d] = %d\n", i, a[i] );
 
 
}
```
结果：a[1] = 0, j = 1

 

4. a[++j] = ++j;
```c
void main(){
 
int j = 0;
int a[5] = {5,5,5,5,5};
a[++j] = ++j;
 
 
printf("j = %d\n", j );
for(int i = 0; i < 5; i++)
    printf("a[%d] = %d\n", i, a[i] );
 
}
```
结果：a[2] = 2, j = 2

 

5. a[++j] = j;
```c
void main(){
 
int j = 0;
int a[5] = {5,5,5,5,5};
a[++j] = j;
 
printf("j = %d\n", j );
for(int i = 0; i < 5; i++)
    printf("a[%d] = %d\n", i, a[i] );
 
}
```
结果：a[1] = 1, j = 1

 

6. a[j] = ++j;
```c
void main(){
 
int j = 0;
int a[5] = {5,5,5,5,5};
a[j] = ++j;
 
printf("j = %d\n", j );
for(int i = 0; i < 5; i++)
    printf("a[%d] = %d\n", i, a[i] );
 
}
```
结果：a[1] = 1, j = 1
