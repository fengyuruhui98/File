# C-hash 用法

hash在头文件`uthash.h`中已实现，需要先下载源码。  
## 1.哈希结构体
```c
#include "uthash.h"
typedef struct{
    int key;
    int value;
    UT_hash_handle hh;
}Hash;
Hash * hash = NULL; //一定要初始化为NULL不然报错
```
其中，UT_hash_handle是头文件“`uthash.h`”中定义的，然后Hash结构体是自定义的。`key`的类型可以是`int, char *, char[], void*`这四种，value可以是任意值。***且value不一定是存在的***  
可以在结构体中仅定义key值，这样哈希表中就只关注表中有没有某个key，而不关心它对应的value值，这样构造的哈希表可以看做***Python中的set， 可以保证内部无重复元素（因为hash表key不能重复）***  
```c
struct my_struct{
    int id;
    UT_hash_handle hh;
}
```
**结构体中key的定义一定要有，`UT_hash_handle hh`也不能去掉。  
**hh是内部使用使用的hash处理句柄，在使用过程中，只需要在结构体中定义一个UT_hash_handle类型的变量即可，不需要为该句柄变量赋值，但必须在结构体中定义该变量**  
## 2.增删查
增加
```c
void add(int key, int value){
    Hash * s = NULL;
    s = (Hash *)malloc(sizeof(Hash));
    s -> key = key;
    s -> value = value;
    HASH_ADD_INT(hash, key, s);
}
```
查找
```c
int find(int key){
    Hash * s = NULL;
    HASH_FIND_INT(hash, &key, s); //要查key值取地址，s为返回指针
    if(s != NULL) return 1;
    else return 0;
}
```
删除
```c
void delete(Hash * s){
    HASH_DEL(hash, s);
    free(s);
    s = NULL;
}
```
其中，HASH_ADD_INT、HASH_FIND_INT、HASH_DEL都是标准函数，其他都是自定义代码。***插入之前首先要查找确认没有这个元素---如果要修改已经存在的键值对，要先删除再插入***

## 3.遍历、清空、计数
遍历：
1. for循环进行遍历，在uthash中，哈希表中每个键值对之间有指针相连，并且可以通过句柄hh来实现指针调用。 每个键值对都会有一个前向指针hh.prev与后向指针hh.next，因此哈希表也可以当作双向链表使用。可以用 s = s->hh.next 实现从前向后循环的功能。  
```c
void print_users() {
    struct my_struct *s;
    for(s=users; s != NULL; s=s->hh.next) {
        printf("user id %d: name %s\n", s->id, s->name);
    }
}
```
这种方法中，在循环时s不能被随便释放或删除，因为还要获取它的next值；不过可以通过加入一个临时变量tmp，把s->hh.next 保存下来，再删除s。于是就出现了函数HASH_ITER，它里面即包含了这样一个tmp变量，使我们在循环过程中可以安全地删除s。  
2. HASH_ITER函数
```c
//清空
void clearAll(){
    Hash * s, * tmp;
    HASH_ITER(hh, hash, s, tmp){
        delete(s);
    }
}
//计数
int cnt = HASH_COUNT(hash);
```
其中，HASH_ITER是标准宏，HASH_COUNT是标准函数，其他为自定义代码。

## 4. 其他类型的key
```c
//char* key HASH_ADD_KEYPTR
//char key[100] HASH_ADD_STR
//void* key HASH_ADD_PTR
//void* key HASH_FIND_PTR
```
除次4个之外，其它函数都一样。