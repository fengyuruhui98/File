# memset给整型数组赋初值
现有一个整型数组a[5]，若要是用memset函数给每个元素赋初值1怎么做呢？是memset(a,1,5*sizeof(int))吗？  
其实这样是不对的，memset是以字节为单位就是对array指向的内存的4个字节进行赋值，每个都用ASCII为1的字符去填充，转为二进制后，1就是00000001,占一个字节。一个INT元素是4字节，合一起就是00000001000000010000000100000001，就等于16843009，就完成了对一个INT元素的赋值了。所以用memset对非字符型数组赋初值是不可取的！当然，可以用memset(a,0,sizeof(a)) 或者memset(a,0, 10 * sizeof(int))来清零。