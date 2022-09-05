# C语言图片转化成Base64码
## 编码思想
采用64个基本的ASCII码字符对数据进行重新编码。它将需要编码的数据拆分成字节数组。以3个字节为一组。按顺序排列24 位数据，再把这24位数据分成4组，即每组6位。再在每组的的最高位前补两个0凑足一个字节。这样就把一个3字节为一组的数据重新编码成了4个字节。当所要编码的数据的字节数不是3的整倍数，也就是说在分组时最后一组不够3个字节。这时在最后一组填充1到2个0字节.  

------------------------------
## 转换函数
```C
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#define MAXN 1025
#define UCHAR unsigned char
 
void base64Encode(UCHAR*, int, char*, int);
 
const char *MAP_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
int main()
{
    FILE *p = fopen("C:\\Users\\Administrator\\Desktop\\1.JPG", "rb");
    UCHAR buf[MAXN];
    fread(buf, sizeof(unsigned char), MAXN, p);
    fclose(p);
    printf("16进制码:\n");
    for (int i = 0; i < MAXN; i++)
    {
        if (i != 0 && i % 24 == 0) printf("\n");
        printf("%02x ", buf[i]);
    }
    printf("\n");
    char base64[MAXN / 3 * 4 + (MAXN % 3 != 0) * 4];
    memset(base64, '?', sizeof(base64));
    base64Encode(buf, sizeof(buf), base64, sizeof(base64));
    // printf("%d %d\n", sizeof(buf), sizeof(base64));
    printf("base64码:\n");
    for (int i = 0; i < sizeof(base64); i++)
    {
        if (i != 0 && i % 76 == 0) printf("\n");
        printf("%c", base64[i]);
    }
    return 0;
}
 
void base64Encode(UCHAR *buf, int buf_size, char *base64, int base64_size)
{
    if (base64_size * 3 < buf_size * 4)
    {
        printf("Error!\n");
        exit(0);
    }
    printf("----------------------------------------------------\n");
    printf("buf-size = %d\n", buf_size);
    printf("base64-size = %d\n", base64_size);
    UCHAR tmp[3];
    int i;
    for (i = 0; i < buf_size / 3; i++)
    {
        tmp[0] = buf[i * 3];
        tmp[1] = buf[i * 3 + 1];
        tmp[2] = buf[i * 3 + 2];
        // printf("buf data : %x %x %x\n", tmp[0], tmp[1], tmp[2]);
        base64[i * 4] = MAP_TABLE[tmp[0] >> 2];
        base64[i * 4 + 1] = MAP_TABLE[(tmp[0] << 4 | tmp[1] >> 4) & 0x3F];
        base64[i * 4 + 2] = MAP_TABLE[(tmp[1] << 2 | tmp[2] >> 6) & 0x3F];
        base64[i * 4 + 3] = MAP_TABLE[tmp[2] & 0x3F];
        // printf("=> %c%c%c%c\n", base64[i * 4], base64[i * 4 + 1], base64[i * 4 + 2], base64[i * 4 + 3]);
    }
    if (buf_size % 3 == 2)
    {
        tmp[0] = buf[i * 3];
        tmp[1] = buf[i * 3 + 1];
        base64[i * 4] = MAP_TABLE[tmp[0] >> 2];
        base64[i * 4 + 1] = MAP_TABLE[(tmp[0] << 4 | tmp[1] >> 4) & 0x3F];
        base64[i * 4 + 2] = MAP_TABLE[(tmp[1] << 2) & 0x3F];
        base64[i * 4 + 3] = '=';
    }
    else if (buf_size % 3 == 1)
    {
        tmp[0] = buf[i * 3];
        base64[i * 4] = MAP_TABLE[tmp[0] >> 2];
        base64[i * 4 + 1] = MAP_TABLE[(tmp[0] << 4) & 0x3F];
        base64[i * 4 + 2] = '=';
        base64[i * 4 + 3] = '=';
    }
 
    printf("----------------------------------------------------\n");
}
```

--------------------------
## 把图片转换成base64 Demo
```C
// #define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#define MAX_KB 1024
#define MAXN 1024 * MAX_KB
#define UCHAR unsigned char
 
void base64Encode(UCHAR*, int, char*);
char *parseBase64(const char *);
 
const char *MAP_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
int main()
{
    // freopen("C:\\Users\\Administrator\\Desktop\\out.txt", "w+", stdout);
    char *base64 = parseBase64("C:\\Users\\Administrator\\Desktop\\1.JPG");
    /*printf("base64码(长度:%d):\n", strlen(base64));
    for (int i = 0; i < strlen(base64); i++)
    {
        if (i != 0 && i % 76 == 0) printf("\n");
        printf("%c", *(base64 + i));
    }*/
    return 0;
}
 
char *parseBase64(const char * imgPath)
{
    FILE *p = fopen(imgPath, "rb");
    UCHAR buf[MAXN];
    int len = fread(buf, sizeof(unsigned char), MAXN, p);
#ifdef DEBUG
    printf("共读取%d个字节.\n", len);
    printf("16进制码:\n");
    for (int i = 0; i < len; i++)
    {
        if (i != 0 && i % 24 == 0) printf("\n");
        printf("%02x ", buf[i]);
    }
    printf("\n");
#endif // DEBUG
    fclose(p);
    int base64_len = len / 3 * 4 + (len % 3 != 0) * 4 + 1;
    char *base64 = new char[base64_len];
    memset(base64, '\0', sizeof(char) * base64_len);
    base64Encode(buf, len, base64);
    return base64;
}
 
void base64Encode(UCHAR *buf, int buf_size, char *base64)
{
#ifdef DEBUG
    printf("----------------------------------------------------\n");
    printf("buf-size = %d\n", buf_size);
#endif // DEBUG
    UCHAR tmp[3];
    int i;
    for (i = 0; i < buf_size / 3; i++)
    {
        tmp[0] = buf[i * 3];
        tmp[1] = buf[i * 3 + 1];
        tmp[2] = buf[i * 3 + 2];
        base64[i * 4] = MAP_TABLE[tmp[0] >> 2];
        base64[i * 4 + 1] = MAP_TABLE[(tmp[0] << 4 | tmp[1] >> 4) & 0x3F];
        base64[i * 4 + 2] = MAP_TABLE[(tmp[1] << 2 | tmp[2] >> 6) & 0x3F];
        base64[i * 4 + 3] = MAP_TABLE[tmp[2] & 0x3F];
#ifdef DEBUG
        printf("buf data : %x %x %x\n", tmp[0], tmp[1], tmp[2]);
        printf("=> %c%c%c%c\n", base64[i * 4], base64[i * 4 + 1], base64[i * 4 + 2], base64[i * 4 + 3]);
#endif // DEBUG
    }
    if (buf_size % 3 == 2)
    {
        tmp[0] = buf[i * 3];
        tmp[1] = buf[i * 3 + 1];
        base64[i * 4] = MAP_TABLE[tmp[0] >> 2];
        base64[i * 4 + 1] = MAP_TABLE[(tmp[0] << 4 | tmp[1] >> 4) & 0x3F];
        base64[i * 4 + 2] = MAP_TABLE[(tmp[1] << 2) & 0x3F];
        base64[i * 4 + 3] = '=';
    }
    else if (buf_size % 3 == 1)
    {
        tmp[0] = buf[i * 3];
        base64[i * 4] = MAP_TABLE[tmp[0] >> 2];
        base64[i * 4 + 1] = MAP_TABLE[(tmp[0] << 4) & 0x3F];
        base64[i * 4 + 2] = '=';
        base64[i * 4 + 3] = '=';
    }
#ifdef DEBUG
    printf("----------------------------------------------------\n");
#endif // DEBUG
}
```