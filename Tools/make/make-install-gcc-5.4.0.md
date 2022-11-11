# 源码安装gcc-5.4.0
## 1.下载gcc-5.4.0源码
```vim
sudo -i
cd /opt
wget http://ftp.gnu.org/gnu/gcc/gcc-5.4.0/gcc-5.4.0.tar.gz
# 如果wget下载速度过慢 可以直接粘贴链接到浏览器下载 
```
## 2.解压
```vim
tar -zxf gcc-5.4.0.tar.gz
```
## 3.下载编译所需要依赖包
### 3.1wget下载速度快的时候
```vim
cd gcc-5.4.0
./contrib/download_prerequisites
```
之后便会通过wget下载四个文件呢，如果发现下载速度过慢的话，可以按照3.2执行；没有这种情况的话，直接执行4就行了。  
### 3.2 wget下载速度慢的时候
先进去解压的文件夹，打开文件`./contrib/download_prerequisites`，注释掉`wget`开头的行并保存。接着把`wget`后面的链接粘贴到浏览器下载（将ftp改成http），四个文件（isl-0.15.tar.bz2；mpc-0.8.1.tar.gz；mpfr-2.4.2.tar.bz2；gmp-4.3.2.tar.bz2）下载完成后，放在gcc-5.4.0的根目录。
放置完成后运行脚本`./contrib/download_prerequisites`。
## 4.编译安装gcc
```vim
./configure --prefix=/usr/local/gcc-5.4.0 --enable-bootstrap --enable-checking=release --enable-languages=c,c++ --disable-multilib  
make
make install
```
>编译参数说明：  
–prefix=/usr/local/ 指定安装路径  
–enable-bootstrap 这里引用网上一些文献对该参数的解释：用第一次编译生成的程序进行第二次编译，然后用再次生成的程序进行第三次编译，并且检查比较第二次和第三次结果的正确性，也就是进行冗余的编译检查工作。 非交叉编译环境下，默认已经将该值设为 enable，可以不用显示指定；交叉编译环境下，需要显示将其值设为 disable。  
–enable-checking=release 以软件发布版的标准来对编译时生成的代码进行一致性检查；设置该选项为 enable并不会改变编译器生成的二进制结果，但是会导致编译的时间增加；该选项仅支持gcc编译器； 总体而言，对于上面这个选项，机器的硬件配置较低，以及不愿等待太久编译时间的童鞋，可以设置为 disable；但是这会增加产生未预期的错误的风险，所以应该慎用。 可以同时设置 --disable-bootstrap 与 --disable-checking，这对编译过程的提速很有帮助。  
–enable-threads=posix 顾名思义，启用posix标准的线程支持 ，要让程序能在符合POSIX规范的linux发布版上正确运行，就应该启用该选项，取决于宿主或目标操作系统的类型，其它可用值有：aix，dec，solaris，win32等，如果你是其它的类UNIX系统，就需要设置相应的值。  
–enable-languages=c,c++ 支持的高级语言类型和运行时库，可以设置的所有语言包括ada,c,c++,Fortran,java,objc,obj-c++,GO 等语言。这里只开启了c和c++,因为支持的语言越多，就需要安装越多的相应静态与动态库，还有五花八门的依赖库，这会让管理变得困难，体积也会变得庞大。  
–disable-multilib 如果你的操作系统是32位，默认就已经设置为 disable，这意味着gcc仅能生成32位的可执行程序；如果你的操作系统是64位，默认就已经设置为 enable，这意味着用gcc编译其它源文件时可以通过 -m32 选项来决定是否生成32位机器代码。如果在64位系统上，要禁止生成32位代码， 设置 --disable-multilib。  
–enable-gather-detailed-mem-stats 允许收集详细的内存使用信息，如果设置该参数为 enable，则将来编译好的gcc可执行程序，可以通过 -fmem-report 选项来输出编译其它程序时的实时内存使用情况。  

# 设置方法
## 1.终端命令设置
GCC 5.4.0的所有文件，包括命令文件(如gcc、g++)、库文件等都在安装目录下。由于命令文件和库文件所在的目录还没有包含在相应的搜索路径内，所以必须要作适当的设置之后编译器才能顺利地找到并使用它们。
```vim
vim /etc/profile
# 打开之后添内容：export="install_path/bin:$PATH"
source /etc/profile
gcc -v
```
## 2.符号连接
首先，查看原来的gcc所在的路径：
```vim
 which gcc
```
在我的系统上，上述命令显示：/usr/bin/gcc。因此，原来的gcc命令在/usr/bin目录下。我们可以把GCC 5.4.0中的gcc、g++等命令在/usr/bin目录下分别做一个符号连接：
```
cd /usr/bin
#{destdir}是指在编译安装过程中 --prefix所指定的路径
ln -s {destdir}/bin/gcc gcc540
ln -s {destdir}/bin/g++ g++540
```
这样，就可以分别使用gcc540、g++540来调用GCC 5.4.0的gcc、g++ 完成对C、C++ 程序的编译了。同时，仍然能够使用旧版本的GCC编译器中的gcc、g++等命令。
