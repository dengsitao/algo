#include "stdlib.h"
#include "stdio.h"


#include "heap.h"
#include "fiboheap.h"


int main(int argc, char * argv[])
{
	int * input1 = new int[HEAP_LEN];
	gen_input(input1, HEAP_LEN, MAX_VALUE);
	build_minheap(input1, HEAP_LEN);
	print_arr(input1, HEAP_LEN);
	
	fh_node * root1 = heap2fh_node(input1, HEAP_LEN);
	print_tree(root1);
	fibo_heap * fh1 = new fibo_heap();
	insert2fibo_heap(fh1, root1);

	int * input2 = new int[HEAP_LEN];
	gen_input(input2, HEAP_LEN, MAX_VALUE);
	build_minheap(input2, HEAP_LEN);
	print_arr(input2, HEAP_LEN);
	
	fh_node * root2 = heap2fh_node(input2, HEAP_LEN);
	print_tree(root2);
	fibo_heap * fh2 = new fibo_heap();
	insert2fibo_heap(fh2, root2);

	fibo_heap * fh = merge_fibo_heap(fh1, fh2);
	int min_value = fh->extract_min();
	printf("min value=%d\n", min_value);
	min_value = fh->extract_min();
	printf("min value=%d\n", min_value);
	min_value = fh->extract_min();
	printf("min value=%d\n", min_value);

	del_fib_node(root2);
	del_fib_node(root1);
	return 0;
}
