#include "stdio.h"
#include "stdlib.h"

#include "fiboheap.h"

void fh_node::p_me()
{
	printf("fhnode[%p]:value[%d] degree[%d] parent[%p][%d] lc[%p][%d] rc[%p][%d], left[%p][%d] right[%p][%d]\n"
		, this, value, degree
		, this->parent, this->parent==NULL?-1:this->parent->value
		, this->lc, this->lc==NULL?-1:this->lc->value
		, this->rc, this->rc==NULL?-1:this->rc->value
		, this->left, this->left==NULL?-1:this->left->value
		, this->right, this->right==NULL?-1:this->right->value
		);
}

void print_tree(fh_node * root)
{
	if (root==NULL) return;
	root->p_me();
	print_tree(root->lc);
	print_tree(root->rc);
}

fh_node * hp_arr2fh(int hp[], int idx, int len, fh_node * parent)
{
	fh_node * node = new fh_node();
	node->value = hp[idx];
	if (parent==NULL)//root
	{
		node->degree = 0;
		printf("hp_arr2fh: add hp[%d]=%d as root\n", idx, hp[idx]);
	}
	else 
	{
		node->degree = parent->degree+1;
		node->parent = parent;
		if (idx%2==1)
		{
			parent->lc = node;
			printf("hp_arr2fh: add hp[%d]=%d to fh[%d]'s lc\n", idx, hp[idx], node->parent->value);
		}
		else 
		{
			parent->rc=node;
			printf("hp_arr2fh: add hp[%d]=%d to fh[%d]'s rc\n", idx, hp[idx], node->parent->value);
		}
	}
	if (idx*2+1<len)
	{
		hp_arr2fh(hp, idx*2+1, len, node);
	}
	if ((idx+1)*2<len)
	{
		hp_arr2fh(hp, (idx+1)*2, len, node);
	} 
	return node;
}

fh_node* heap2fh_node(int hp[], int len)
{
	fh_node * root = hp_arr2fh(hp, 0, len, NULL);
	return root;
}


