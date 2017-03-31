#include "string.h"

class fh_node
{
public:
	fh_node * parent;
	fh_node * left;
	fh_node * right;
	fh_node * lc;
	fh_node * rc;
	int degree;
	int max_degree;
	int value;
	void p_me();
};

fh_node * heap2fh_node(int hp[], int hp_len);
void print_tree(fh_node * root);
void del_fib_node(fh_node * root);


class fibo_heap
{
public:
	int n;
	int max_degree;
	fh_node * root_list;
	fh_node * min;
	int extrace_min();
	void consolidate();
};

void insert2fibo_heap(fibo_heap * fb_hp, fh_node * node);

fibo_heap * merge_fibo_heap(fibo_heap * fb_hp1, fibo_heap * fb_hp2);


