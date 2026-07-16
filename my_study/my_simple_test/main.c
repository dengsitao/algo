#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

struct abc {
	int a;
	int b;
};

void change_abc_ptr(struct abc *p)
{
	p->a=12;
	p->b=13;
}
void change_abc_value(struct abc p)
{
	p.a=14;
	p.b=15;
}
int main()
{
	struct abc a;
	a.a=10;
	a.b=11;
	change_abc_ptr(&a);
	printf("a.a=%d a.b=%d\n", a.a, a.b);
	change_abc_value(a);
	printf("a.a=%d a.b=%d\n", a.a, a.b);
	int ia=0;
	int pid = fork();
	if (pid < 0) {
		printf("fork failed\n");
		return -1;
	}
	else if (pid == 0) {
		printf("in child %d, ia=%d %p\n", getpid(), ++ia, &ia);
	}
	else {
		printf("in parent %d, a=%d %p\n", getpid(), ++ia, &ia);
	}

	return 0;
}
