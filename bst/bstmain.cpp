#include "stdlib.h"
#include "stdio.h"
#include <string>
#include <vector>
#include <list>

class tnode
{
public:
    tnode(): parent(NULL), left(NULL), right(NULL), value(-1), depth(0), index(1) {
    };
    tnode * parent;
    tnode * left;
    tnode * right;
    int depth;
    int index;
    int value;
};

tnode * deletenode(tnode * node)
{
    printf("deleting node [%d] from BST\n", node->value);
    tnode * successor = NULL;
    tnode * npapa = node->parent;

    if (node->right == NULL && node->left == NULL) {
        delete node;
        return npapa;
    } else if (node->right != NULL) {
        successor = node->right;

        while (successor->left != NULL)
            successor = successor->left; //find right subtree's minimum.

        //link successor's right child to successor's parent.
        tnode * s_right = successor->right; //successor doesn't have left.

        if (successor != node->right) { //if successor is not noes's right, then it must be successor's parent's left and a swap is needed
            successor->parent->left = s_right;

            if (s_right != NULL) {
                s_right->parent = successor->parent;
            }

            successor->right = node->right;
            node->right->parent = successor;
            successor->left = node->left;

            if (successor->left != NULL)
                successor->left->parent = successor;
        }

        successor->parent = npapa;
        delete node;
    } else {
        successor = node->left;

        while (successor->right != NULL)
            successor = successor->right; //find left subtree's maximum.

        //link successor's left child to successor's parent.
        tnode * s_left = successor->left; //successor doesn't have left.

        if (successor != node->left) { //if successor is not noes's left, then it must be successor's parent's right and a swap is needed
            successor->parent->right = s_left;

            if (s_left != NULL) {
                s_left->parent = successor->parent;
            }

            successor->left = node->left;
            node->left->parent = successor;
        }

        successor->parent = npapa;
        delete node;
    }

    if (npapa != NULL) {
        if (node == npapa->left)
            npapa->left = successor;
        else
            npapa->right = successor;
    }

    return npapa;
}

tnode * deletefrombst(tnode * inode)
{
    if (inode->right == NULL) { //right child is null, direct delete.
        tnode * retv = inode->left;

        if (retv == NULL) { // left is null too
            if (inode->parent != NULL) {
                if (inode->parent->left == inode)
                    inode->parent->left = NULL;
                else
                    inode->parent->right = NULL;

                delete inode;
                return NULL;
            } else {
                delete inode;
                return NULL;
            }
        }

        retv->parent = inode->parent;

        if (inode->parent != NULL) {
            if (inode == inode->parent->left)
                inode->parent->left = inode->left;
            else
                inode->parent->right = inode->right;
        }

        delete inode;
        return retv;
    } else { //need check right child's left and right.
        tnode * right = inode->right;

        if (right->left == NULL) { //right child don't have left child, so right become the new root.
            tnode* retv = right;

            if (inode->left != NULL) {
                inode->left->parent = right;
                right->left = inode->left;
            }

            if (inode->parent != NULL) {
                if (inode == inode->parent->left)
                    inode->parent->left = inode->left;
                else
                    inode->parent->right = inode->right;
            }

            delete inode;
            return retv;
        } else { //right has left, then re-org is needed.
            int tmpv = right->value;
            right->value = inode->value;
            inode->value = tmpv;
            inode->right = deletefrombst(inode->right);
            return inode;
        }
    }

}
void insert2bst(int value, tnode * root, int & depth)
{
    int d = depth;

    if (root == NULL)
        return;

    if (value >= root->value) {
        if (root->right != NULL)
            insert2bst(value, root->right, ++depth);
        else {
            tnode* p = new tnode();
            p->parent = root;
            p->value = value;
            p->depth = depth;
            p->index = p->parent->index * 2;
            root->right = p;
            printf("add node[%d] to right depth[%d] index[%d]\n", p->value, p->depth, p->index);
        }
    } else {
        if (root->left != NULL)
            insert2bst(value, root->left, ++depth);
        else {
            tnode* p = new tnode();
            p->parent = root;
            p->value = value;
            p->depth = depth;
            p->index = p->parent->index * 2 - 1;
            root->left = p;
            printf("add node[%d] to left depth[%d] index[%d]\n", p->value, p->depth, p->index);
        }

    }
}

int buildBST(int input[], int length, tnode * root)
{
    root->value = input[0];
    int max = 1;

    for (int i = 1; i < length; i++) {
        int depth = 1;
        insert2bst(input[i], root, depth);

        if (depth > max)
            max = depth;
    }

    return max;//depth;
}

int getValueInBST(tnode * root, int row, int pos)
{
    if (row == 0) return root->value;

    std::list<int> turnList;
    int tmp = pos;

    for (int i = 0; i < row; i++) {
        int dir = tmp & 1; //the last bit
        //printf("i=%d, dir=%d, tmp=%d ", i, dir, tmp);
        turnList.push_back(dir);
        tmp = tmp >> 1;
    }

    //printf(" pos=%d ", pos);
    for (int i = 0; i < row; i++) {
        int dir = turnList.back();
        turnList.pop_back();

        //printf("row=%d, dir=%d ", i, dir);
        if (dir == 0) root = root->left;
        else root = root->right;

        if (root == NULL) {
            //printf("no further searching");
            return -1;
        }
    }

    //printf(": ");
    return root->value;
}
void BSTprint(tnode * root, int depth)
{
    if (root == NULL)
        return;

    while (root->parent != NULL)
        root = root->parent;

    int maxOfElem = 1 << (depth - 1);;

    for (int i = 0; i < depth; i++) { //row number
        int rnoe = (i == 0 ? 1 : 1 << i);

        for (int k = 0; k < rnoe; k++) {
            for (int j = 0; j < ((maxOfElem * 3 / (rnoe) - 1) / 2); j++) {
                printf(" ");
            }

            char valueStr[2];
            int value = getValueInBST(root, i, k);

            if (value != -1)
                sprintf(valueStr, "%d", value);
            else
                sprintf(valueStr, "x");

            printf("%s", valueStr);

            for (int j = 0; j < ((maxOfElem * 3 / (rnoe) - 1) / 2); j++) {
                printf(" ");
            }

            if (i != depth - 1)
                printf(" ");

            //printf("\n");
        }

        printf("\n");
    }
}

class dlk_node 
{
public:
	int value;
	dlk_node * next;
	dlk_node * prev;
	dlk_node():value(-1),next(NULL),prev(NULL){};
};
void transBST2dlink(tnode * node, dlk_node ** dlknode)
{
	if (node==NULL) return;
	transBST2dlink(node->left, dlknode);
	if ((*dlknode)->value!=-1 ) {// not the root
		dlk_node * p1 = new dlk_node();
		p1->value=node->value;
		p1->prev = *dlknode;
		(*dlknode)->next=p1;
		*dlknode = p1;
		printf("add node [%d] to linked list, prev[%d]\n", p1->value, (*dlknode)->value);
	}
	else {
		(*dlknode)->value=node->value;
		printf("add node [%d] as root to linked list \n", node->value);
	}
	transBST2dlink(node->right, dlknode);
	return ;		
}
void print_dlinkedlist(dlk_node * root)
{
	while(root->next!=NULL) {
		printf(" %d ", root->value);
		root=root->next;
	}
	printf("\n");
}
void testTransBST2DoubleLinkedlist()
{
    tnode *root = new tnode();
    int input[10] = {5, 3, 8, 2, 6, 4, 7, 1, 9, 0};
    int d = buildBST(input, 10, root);
    BSTprint(root, 5);
	dlk_node * dlk_root = new dlk_node();	
	dlk_node * droot = dlk_root;
	transBST2dlink(root, &droot);
	print_dlinkedlist(dlk_root);
}
int main(int argc, char * argv[])
{
    tnode *root = new tnode();
    int input[10] = {5, 3, 8, 2, 6, 4, 7, 1, 9, 0};
    int d = buildBST(input, 10, root);
    BSTprint(root, 5);
    deletenode(root->right->right);
    BSTprint(root, 5);
    deletenode(root->right->left);
    BSTprint(root, 5);


    return 0;
}

