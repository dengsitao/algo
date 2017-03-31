#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include <time.h>

#define HEAP_LEN 10
#define MAX_VALUE HEAP_LEN*10
class tnode
{
public:
	int value;
	tnode * pnt;
	tnode * lc;
	tnode * rc;
};

//helper area
#define PARENT(x) x/2
#define LEFT(x) x*2
#define RIGHT(x) x*2+1

int gen_rand(int randMax);

void gen_input(int hp[], int len, int randMax);

void print_arr(int hp[], int len);

void validate_min_heap(int hp[], int len);

void validate_max_heap(int hp[], int len);

//helper area end
//int heap_arr[HEAP_LEN];
void exchange(int *a, int *b);
void max_heapify(int hp[], int index, int len);

void build_maxheap(int hp[], int len);

void min_heapify(int hp[], int index, int len);

void build_minheap(int hp[], int len);

void heap_sort_max(int hp[], int len);

void heap_sort_min(int hp[], int len);

void test_heap_sort(int heap_len, int max_value);
