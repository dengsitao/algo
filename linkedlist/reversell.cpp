#include "stdlib.h"
#include "stdio.h"

class lnode
{
public:
	int value;
	lnode * next;
	//lnode():value(-1), next(NULL) {};
	lnode(int v):value(v), next(NULL) {};
};

void print_lnode(lnode * node)
{
	while(node!=NULL) {
		printf("node[%d] \n", node->value);
		node=node->next;
	}
}
lnode * reverse(lnode * head)
{
	lnode * first = head;
	if (first==NULL) return NULL;
	lnode * second = first->next;
	first->next=NULL;
	while(second!=NULL) {
		lnode * third=second->next;
		second->next=first;
		first=second;
		second=third;
		//printf("first[%d] second[%d] third[%d]\n", first->value, second->value, third->value);
	}
	return first;
}

int main(int argc, char * argv[])
{
	lnode * head=new lnode(1);
	lnode * phead=head;
	for (int i=2;i<10;i++) {
		lnode * p=new lnode(i);
		phead->next=p;
		phead=p;
	}
	print_lnode(head);
	phead = reverse(head);
	print_lnode(phead);
	return 0;
}
