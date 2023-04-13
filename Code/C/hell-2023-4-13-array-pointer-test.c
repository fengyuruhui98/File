#include <stdio.h>
 
 
void main(void)
{
    char (*p)[5] = NULL;
    char (*p1)[5] = NULL;
    char a[5] = {0,1,2,3,4};
    p = a;
    p1 = &a;
    
    printf("a[5] = %d\n", a[5]);
    printf("p = %p, p+1 = %p, p1 = %p, P1+1 = %p\n", p, p+1, p1, p1+1);
    printf("*(char *)p = %d, *((char *)p+1) = %d, *(char *)p1 = %d, *((char *)p1+1) = %d\n", *(char *)p, *((char *)p+1), *(char *)p1, *((char *)p1+1));
    printf("*(char *)p = %d, *(char *)(p+1) = %d, *(char *)p1 = %d, *(char *)(p1+1) = %d\n", *(char *)p, *(char *)(p+1), *(char *)p1, *(char *)(p1+1));
    printf("*p = %p, *(p+1) = %p, *p1 = %p, *(P1+1) = %p\n", *p, *(p+1), *p1, *(p1+1));
    printf("**p = %d, **(p+1) = %d, **p1 = %d, **(P1+1) = %d\n", **p, **(p+1), **p1, **(p1+1));
}