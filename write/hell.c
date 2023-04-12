#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int len;
typedef void ( * p_fun)(void);
 
 
void fun(const char* input, int size)
{
    char buf[8];
 
    memcpy(buf, input, size);
    printf("\nAddress of param=%p\n",&input);
    printf("Address of ret=%p\n",*(&input-1));
    printf("Address of local value=%p\n",buf);
 
    len = (long long)&input - (long long)buf;    //计算从local variable到fun param的长度
    printf("i am in fun\n");
}
 
void bad_fun()
{
    printf("\nok, i did it.\n");
}
 
 
int stackflow(){
    int addr[8] = {0};
    char s[] = "cal len";
    long long go = (long long)bad_fun;
    char ss[100] = {0};
 
 
    fun(s, sizeof(s));
  
     //((p_fun)go)();
    printf("len = %d\n", len);
 
    printf("Address of fun=%p\n", fun);
    printf("Address of bad_fun=%p\n", bad_fun);
    printf("Address of go=%lx\n", go);
 
    //把bad_fun()函数的地址分离成字节, 然后储存到ss中
    addr[0]=(go >> 0) & 0xff; addr[1]=(go >> 8) & 0xff; addr[2]=(go >> 16) & 0xff;  addr[3]=(go >> 24) & 0xff;
    addr[4]=(go >> 32) & 0xff; addr[5]=(go >> 40) & 0xff; addr[6]=(go >> 48) & 0xff;  addr[7]=(go >> 56) & 0xff;
    for(int j=0;j<8;j++)
    {
        ss[len-j-1]=addr[7-j];
    }
    
    fun(ss, len);
    printf("test end.\n");    //由于函数返回地址被修改，此代码不被执行
    return 0;
}


int main(int argc, char* argv[]){
    stackflow();
    return 0;
}