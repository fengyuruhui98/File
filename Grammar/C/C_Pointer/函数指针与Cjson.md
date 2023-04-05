# C�����к���ָ��͸�������

����ڿ�c���Խ���json�������⣬������һЩ�Ƚϸ��ӵ�������ʽ�����������»ع���һ�����֪ʶ��

## ����ָ��
��ͬ��JS�к���������Ϊ����������������C�����к��������Ǳ��������ǿ��Զ���ָ������ָ�룬������ָ�룬�Ӷ�������������һЩ�������ݵȡ�

������һЩʾ�����������⡣

```c
// ����һ������ָ��
int (*comp)(void *, void *) /* ��ȷ��д�� */
int *comp(void *, void *)   /* �����д�� */
```

��仰�涨�ˣ�comp��һ������ָ�룬����ָ��ĺ�����������void\*���͵Ĳ���������������int��

(\*comp)�������Ǳز����ٵģ����ȱ�پͻ��������ڶ��ִ���д��������д���ĺ����ǣ�����һ�������������������������int���͵�ָ�롣�����Ǻ���ָ�롣������ĸ����������ֽ���ϸ����\*��ʹ�ù��ɡ���

void\*��ͨ��ָ�����ͣ��κ����͵�ָ�붼����ת��Ϊvoid\*�����Ҵ�void\*��ת����ʱ���ᶪʧ��Ϣ���ڵ���compʱ������Ѳ���ǿ��ת��Ϊvoid\*���͡�

```c
// ���øú���
if((*comp)(v[i], v[left]) < 0)
```

comp����һ������ָ�룬\*comp����һ������������д�����������comp��ָ��ĺ����������ж���������ķ���ֵ�Ƿ�<0��

## ��������
C���Ե��������ܴ��������Ķ�������Ƶ��ʹ�ô���������ϸ������֣������������Ӷ�������⡣

�����Ѿ�˵������Ϊ����ָ����\*�Ľ�Ϲ�������������������

```c
int *f();       // f��һ������������һ��ָ��int���͵�ָ��
int (*pf)();    // pf��һ������ָ�룬����һ��int���͵Ķ���
```

�����ϲ�����Կ�����\*��һ��ǰ׺����������ȼ�����()��

## typedef���庯��ָ��

```c
typedef int (*PFI)(void *, void *);
PFI comp;
```

����д������������PFI�ǡ�һ������ָ�룬����ָ��ĺ�����������void\*���͵Ĳ���������������int����Ȼ������compΪ�����͵ĺ���ָ�롣����Ҫ������ͬ���ͺ���ָ��ʱ������д����Ϊ��ࡣ

## ʵ��Ӧ��
�ٻص�json���������У�����ɴ����Թ�����

```c
// json.h
typedef struct json_hooks   json_hooks;

struct json_hooks
{
    void *(*malloc_fn)(size_t sz);
    void (*free_fn) (void *ptr);
};

void json_init_hooks(json_hooks *hooks);



// json.c
static void *(*json_malloc)(size_t sz) = malloc;
static void (*json_free)(void *ptr) = free;

void json_init_hooks(json_hooks *hooks)
{
    if (!hooks) /* Reset hooks */
    { 
        json_malloc = malloc;
        json_free = free;
        return;
    }

    json_malloc = (hooks->malloc_fn)?hooks->malloc_fn:malloc;
    json_free = (hooks->free_fn)?hooks->free_fn:free;
}
```

��������������ķ���������json_hooks�ṹ�����У�

```c
void *(*malloc_fn)(size_t sz);
void (*free_fn) (void *ptr);
```

malloc_fn��һ������ָ�룬����ָ��ĺ�������һ��size_t���͵Ĳ���������void���͵�ָ�롣
free_fn��һ������ָ�룬����ָ��ĺ�������һ��void\*���͵�ָ�룬�޷���ֵ��
json.c�У�

```c
static void *(*json_malloc)(size_t sz) = malloc;
static void (*json_free)(void *ptr) = free;
```

��ȫ��������������̬����ָ�루json_malloc��json_free������json_mallocָ��malloc������json_freeָ��free������

```c
void json_init_hooks(json_hooks *hooks)
{
    if (!hooks)
    { 
        json_malloc = malloc;
        json_free = free;
        return;
    }

    json_malloc = (hooks->malloc_fn)?hooks->malloc_fn:malloc;
    json_free = (hooks->free_fn)?hooks->free_fn:free;
}
```

���hooksδ�����������������ָ��������á�
���hooks��������ʹ������Ԫ�ж�ʽ�����hooks��malloc_fn���ڣ�����json_mallocָ����������ָ��malloc��freeͬ��
˵���ף�hook����˼�ǹ��ӣ����仰˵���ǽٳ֣��ⲿ�ִ�����������ڣ�**�ṩһ���ӿ����������Զ���һ��malloc��free���������������û�ж��壬Ĭ�ϲ���stdlib.h�е�malloc��free��**
