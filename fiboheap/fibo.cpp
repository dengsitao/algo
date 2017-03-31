#include "stdlib.h"
#include "stdio.h"


#include "heap.h"
#include "fiboheap.h"


int main(int argc, char * argv[])
{
	int * input = new int[HEAP_LEN];
	gen_input(input, HEAP_LEN, MAX_VALUE);
	build_minheap(input, HEAP_LEN);
	print_arr(input, HEAP_LEN);
	
	fh_node * root = heap2fh_node(input, HEAP_LEN);
	print_tree(root);
	
	return 0;
}
