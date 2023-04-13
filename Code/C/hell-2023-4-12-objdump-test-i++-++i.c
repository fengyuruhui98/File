#include <stdio.h>
void print(int a, int b, int c){
	printf("a = %d\n", a);
	printf("b = %d\n", b);
	printf("c = %d\n", c);
}

void main(void){
	int p1 = 5, p2 = 6, p3 = 7;
	print(p1++, ++p2, p3);
	print(p1, p2, p3);
}
