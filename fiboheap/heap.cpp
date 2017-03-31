#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include <time.h>
#include "heap.h"

//helper area
int gen_rand(int randMax)
{
	return rand() % randMax;
}

void gen_input(int hp[], int len, int randMax)
{
	time_t time_;
	time(&time_);
	srand(time_);
	for (int i=0;i<len;i++)
	{
		hp[i]=gen_rand(randMax);
	}
}

void print_arr(int hp[], int len)
{
	for (int i=0;i<len;i++)
	{
		printf(" %d ", hp[i]);
	}
	printf("\n");
}

void validate_min_heap(int hp[], int len)
{
	for (int i=0;i<len-1;i++)
		if (hp[i]<hp[i+1])
		{
			printf("WRONG! hp[%d]=%d, hp[%d]=%d\n", i, hp[i], i+1, hp[i+1]);
		}
}
void validate_max_heap(int hp[], int len)
{
	for (int i=0;i<len-1;i++)
		if (hp[i]>hp[i+1])
		{
			printf("WRONG! hp[%d]=%d, hp[%d]=%d\n", i, hp[i], i+1, hp[i+1]);
		}
}
//helper area end
void exchange(int *a, int *b)
{
	if (a==NULL || b==NULL) return;
	int temp = *b;
	*b=*a;
	*a=temp;
}
void max_heapify(int hp[], int index, int len)
{
	//do a triple value find max here
	int lc_idx = LEFT(index);
	int rc_idx = RIGHT(index);
	int max_idx = index;
	if (lc_idx < len && hp[lc_idx] > hp[max_idx])
		max_idx = lc_idx;
	if (rc_idx< len && hp[rc_idx] > hp[max_idx])
		max_idx = rc_idx;
	if (max_idx != index)
	{
		exchange(&hp[max_idx], &hp[index]);
		max_heapify(hp, max_idx, len);
	}
}

void build_maxheap(int hp[], int len)
{
	for (int i=len/2;i>=0;i--)
	{
		max_heapify(hp, i, len);
	}
}

void min_heapify(int hp[], int index, int len)
{
	//do a triple value find max here
	int lc_idx = LEFT(index);
	int rc_idx = RIGHT(index);
	int min_idx = index;
	if (lc_idx < len && hp[lc_idx] < hp[min_idx])
		min_idx = lc_idx;
	if (rc_idx< len && hp[rc_idx] < hp[min_idx])
		min_idx = rc_idx;
	if (min_idx != index)
	{
		exchange(&hp[min_idx], &hp[index]);
		min_heapify(hp, min_idx, len);
	}
}

void build_minheap(int hp[], int len)
{
	for (int i=len/2;i>=0;i--)
	{
		min_heapify(hp, i, len);
	}
}

void heap_sort_max(int hp[], int len)
{
	for (int i=len;i>0;i--)
	{
		exchange(&hp[0], &hp[i-1]);
		max_heapify(hp, 0, i-1);
	}
}

void heap_sort_min(int hp[], int len)
{
	for (int i=len;i>0;i--)
	{
		exchange(&hp[0], &hp[i-1]);
		min_heapify(hp, 0, i-1);
	}
}

//int main(int argc, char * argv[])
void test_heap_sort(int heap_len, int max_value)
{
	int *heap_arr = new int[heap_len];
	memset(heap_arr, -1, sizeof(int)*heap_len);
	gen_input(heap_arr, heap_len, max_value);
	//printArr(heap_arr, heap_len);
	build_maxheap(heap_arr, heap_len);
	//printArr(heap_arr, heap_len);
	heap_sort_max(heap_arr, heap_len);
	//printArr(heap_arr, heap_len);
	validate_max_heap(heap_arr, heap_len);

	memset(heap_arr, -1, sizeof(int)*heap_len);
	gen_input(heap_arr, heap_len, max_value);
	//printArr(heap_arr, heap_len);
	build_minheap(heap_arr, heap_len);
	//printArr(heap_arr, heap_len);
	heap_sort_min(heap_arr, heap_len);
	//printArr(heap_arr, heap_len);
	validate_min_heap(heap_arr, heap_len);

	delete[] heap_arr;
	//return 0;
}
