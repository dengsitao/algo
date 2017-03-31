#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "fiboheap.h"

void fh_node::p_me()
{
	printf("fhnode[%p]:value[%d] degree[%d] maxdegree[%d] parent[%p][%d] lc[%p][%d] rc[%p][%d], left[%p][%d] right[%p][%d]\n"
		, this, value, degree, max_degree
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
	root->n=len;
	root->max_degree = ceil(log2(len));
	printf("max_degree=%d\n", root->max_degree);
	return root;
}

void del_fib_node(fh_node * root)
{
	if (root == NULL) return;
	del_fib_node(root->lc);
	del_fib_node(root->rc);
	delete root;
}

void concatenate(fh_node * node1, fh_node * node2)
{
	if (node1==NULL || node2==NULL) return;
	fh_node * right1 = node1->right;
	fh_node * left2 = node2->left;
	if (right1==NULL) right1=node1;
	if (left2==NULL) left2=node2;
	node1->right=node2;
	node2->left=node1;
	right1->left = left2;
	left2->right = right1;
		
}
void insert2fibo_heap(fibo_heap * fb_hp, fh_node * node)
{
	if (node==NULL) return;
	if (fb_hp==NULL) return;
	node->degree=0;
	node->parent=NULL;
	//node->lc=NULL;
	//node->rc=NULL;
	node->left=NULL;
	node->right=NULL;
	if (fb_hp->min==NULL)
	{
		fb_hp->min=node;
		fb_hp->min=node;
	}
	else {
		concatenate(fb_hp->min, node);
		if (fb_hp->min->value > node->value) {
			fb_hp->min = node;
		}
		if (fb_hp->max_degree<node->max_degree) {
			fb_hp->max_degree=node->max_degree;
		}
	}
	fb_hp->n+=node->n;
}

fibo_heap * merge_fibo_heap(fibo_heap * fb_hp1, fibo_heap * fb_hp2)
{
	fibo_heap * fb_hp = new fibo_heap();
	fb_hp->min=fb_hp1->min;
	fb_hp->min=fb_hp1->min;
	concatenate(fb_hp->min, fb_hp2->min);
	if (fb_hp2->min->value < fb_hp->min->value) {
		fb_hp->min = fb_hp2->min;
	}
	if (fb_hp2->max_degree<fb_hp1->max_degree) {
		fb_hp->max_degree=fb_hp1->max_degree;
	}
	else {
		fb_hp->max_degree=fb_hp2->max_degree;
	}
	fb_hp->n=fb_hp1->n+fb_hp2->n;
	fb_hp->max_degree=ceil(log2(fb_hp->n));
	return fb_hp;
}

int fibo_heap::extract_min()
{
	int value = min->value;
	concatenate(min, min->rc);
	fh_node *min_child = min->lc;
	while(min_child!=NULL) {
		concatenate(min, min_child);
		min_child=min_child->left;
	}
	fh_node * left = min->left;
	fh_node * right = min->right;
	left->right = right;
	right->left = left;
	if (min==min->right) {
		delete min;
		min=NULL;
	}
	else {
		fh_node * temp = min;
		min=min->right;
		delete temp;
		consolidate();
	}
	n=n-1;
	max_degree=ceil(log2(n));
	return value;
}
void fibo_heap::consolidate()
{
	fh_node ** da = new fh_node*[max_degree];
	for (int i=0;i<max_degree;i++)
		da[i]=NULL;

	fh_node * node = min->right;
	do
	{
		int m_d = node->max_degree;
		while(da[m_d]!=NULL) {
			fh_node * ynode = da[m_d];
			if (node->value > ynode->value) {
				fh_node * temp=ynode;
				ynode=node;
				node=temp;
			}
			//delete ynode from min;
			fh_node * left = ynode->left;
			fh_node * right = ynode->right;
			left->right=right;
			right->left=left;
			//add ynode as a child of node
			if (node->lc!=NULL) {
				node->lc->left=ynode;
			}
			else {
				node->lc=ynode;
			}
			ynode->parent=node;
			ynode->right=node->lc;
			node->max_degree++;
			da[m_d]=NULL;
			m_d++;
		}
		da[m_d]=node;
		node=node->right;
	}
	while(node!=min);
	min=NULL;
	for (int i=0;i<max_degree;i++) {
		if (min==NULL && da[i]!=NULL) {
			min=da[i];
		}
		else {
			concatenate(min, da[i]);
			if (da[i]!=NULL && da[i]->value < min->value) {
				min=da[i];
			}
		}
	}
}

