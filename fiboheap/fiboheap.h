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
	int value;
	void p_me();
};

fh_node * heap2fh_node(int hp[], int hp_len);
void print_tree(fh_node * root);
