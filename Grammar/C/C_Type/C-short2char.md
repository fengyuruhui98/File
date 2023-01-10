# short2char

网上很多方法都是使用sprintf来做的，但我要的结果不是这样的，我只是要int的二进制放到char里就完事了，sprintf好像不是这样，如果是的话，请留言纠正。话不多说，直接上源码：

char angle[10];
short s = 1000;
angle[1] = (char)s;
angle[0] = s >> 8;
printf("%x,%x\n", angle[0], angle[1]); 
这个是原理性的问题，首先short强转char的时候取的是低8位，所以赋值到数组第二位，之后就是移位，把高8位移到低8位就好了。如果有人去调试看到整数是这样的，但负数就不是这样的，负数在计算机是用补码存放的，如果忘记补码了就百度一下，这样输出的16进制数就是对的了，补码就是除去符号位外取反再加1。