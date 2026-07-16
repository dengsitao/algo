#include <stdio.h>
#include <stdlib.h>

struct my_bit_struct
{
	long lb;
	//long lc;
	int lc;
	long ld;
} mb;

int main(int argc, char * argv[])
{
	long * p1=&mb.lb;
	//long * p2=&mb.lc;
	int * p2=&mb.lc;
	long * p3=&mb.ld;
	printf("size = %lu\n", sizeof(struct my_bit_struct));
	//printf("p2-p1 = %d\n", p2-p1);
	//printf("p3-p2 = %d\n", p3-p2);
	printf("p3-p1 = %d\n", p3-p1);
	printf("p3-p1 = %lu\n", (unsigned long) p3-(unsigned long)p1);
	printf("size of pointer to int =%lu, pointer to long=%lu\n", sizeof(int*), sizeof(long*));
	printf("p1=%p, p2=%p,p3=%p\n", p1, p2, p3);
}
