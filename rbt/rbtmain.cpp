#include "stdlib.h"
#include "stdio.h"
#include <string>

typedef enum {RED, BLACK} tcolor;
const char * const color2str[] = {"RED", "BLACK"};

class tnode
{
public:
    tnode(int value, tcolor color): value_(value), parent_(NULL), lc_(NULL), rc_(NULL), color_(color) {
    };
    tnode(): value_(-1), parent_(NULL), lc_(NULL), rc_(NULL), color_(RED) {
    };
    int value_;
    tnode * parent_;
    tnode * lc_;
    tnode * rc_;
    tcolor color_;
    void printmyself();
};

static tnode nullNode(-1, BLACK);
std::string tostring(tnode * node)
{
    char str[64];

    if (node != NULL) {
        sprintf(str, "[%d][%p][%s]", node->value_, node, color2str[node->color_]);
    } else {
        sprintf(str, "[-1][NULL][BLACK]");
    }

    return str;
}

void tnode::printmyself()
{
    printf("this: %s parent: %s lc_: %s rc_: %s \n",
           tostring(this).c_str(), tostring(this->parent_).c_str(), tostring(this->lc_).c_str(), tostring(this->rc_).c_str());
}

void PrintRBT(tnode * node)
{
    if (node != NULL) {
        node->printmyself();
        PrintRBT(node->lc_);
        PrintRBT(node->rc_);
    }
}

void PrintRBTFromRoot(tnode * node)
{
    printf("-------print rbt start------\n");

    while (node->parent_ != NULL)
        node = node->parent_;

    PrintRBT(node);
    printf("-------print rbt end------\n");
}

tnode* GetSuccessor(tnode * a)
{
    tnode * suc=NULL;
    if (a->rc_!=NULL){
	a=a->rc_;
        while(a!=NULL && a->lc_!=NULL) a=a->lc_;
        return a;
    }
    suc = a->parent_;
    while(suc != NULL && suc->rc_==a) {
        a = suc;
        suc = suc->parent_;
    }
    return suc;
}
void RotateLeft(tnode * z)
{
    if (z == NULL || z->rc_ == NULL) {
        //invalid
        printf("INVALID! z=%p, z->rc_=%p \n",
               z, z == NULL ? NULL : z->rc_);
        return;
    }

    tnode * parent = z->parent_;

    if (parent != NULL) { //z is root
        tnode * rc_lc_ = z->rc_->lc_;

        if (z == parent->lc_) {
            parent->lc_ = z->rc_;
        } else {
            parent->rc_ = z->rc_;
        }
    }

    tnode * rc_lc_ = z->rc_->lc_;
    z->rc_->lc_ = z;
    z->parent_ = z->rc_;
    z->rc_->parent_ = parent;
    z->rc_ = rc_lc_;

    if (rc_lc_ != NULL)
        rc_lc_->parent_ = z;
}

void RotateRight(tnode * z)
{
    if (z == NULL || z->lc_ == NULL) {
        //invalid
        printf("INVALID! z=%p, z->rc_=%p \n",
               z, z == NULL ? NULL : z->lc_);
        return;
    }

    tnode * parent = z->parent_;

    if (parent != NULL) { //z is not root
        if (z == parent->lc_) {
            parent->lc_ = z->lc_;
        } else {
            parent->rc_ = z->lc_;
        }
    }

    tnode * lc_rc_ = z->lc_->rc_;
    z->lc_->rc_ = z;
    z->parent_ = z->lc_;
    z->lc_->parent_ = parent;
    z->lc_ = lc_rc_;

    if (lc_rc_ != NULL)
        lc_rc_->parent_ = z;
}

void InsertFix(tnode * z)
{
    while (z != NULL && z->parent_ != NULL && z->parent_->color_ == RED) { //father is red, violation.
        tnode * parent = z->parent_;
        tnode * grandparent = z->parent_->parent_;

        if (grandparent != NULL && parent == grandparent->lc_) { //parent is left child.
            tnode * uncle = grandparent->rc_;

            if (uncle != NULL && uncle->color_ == RED) { //uncle is red.
                printf("[left]situation 1: z=\n");
                z->printmyself();
                printf("[left] will change parent[%d][%p] to black, uncle[%d][%p] to black, grandparent[%d][%p]=red\n"
                       , parent->value_, parent, uncle->value_, uncle, grandparent->value_, grandparent);

                parent->color_ = BLACK;
                uncle->color_ = BLACK;
                grandparent->color_ = RED;
                //now move upword.
                z = grandparent;
                //PrintRBTFromRoot(z);
            } else if (uncle == NULL || uncle->color_ == BLACK) {
                if (z == parent->rc_) { //if z is right child, rotate left to make it left child.
                    printf("[left]situation 2: z=\n");
                    z->printmyself();
                    printf("[left]situation 2: parent=\n");
                    parent->printmyself();

                    z = parent;
                    RotateLeft(z);
                    //PrintRBTFromRoot(z);
                }

                printf("[left]: situation 3: z=\n");
                z->printmyself();
                printf("[left]situation 2: parent=\n");
                z->parent_->printmyself();
                printf("[left]situation 2: grandparent=\n");
                z->parent_->parent_->printmyself();

                z->parent_->color_ = BLACK;
                z->parent_->parent_->color_ = RED;
                RotateRight(z->parent_->parent_);
                //PrintRBTFromRoot(z);
            }
        } else if (grandparent != NULL && parent == grandparent->rc_) { //parent is left child.
            tnode * uncle = grandparent->lc_;

            if (uncle != NULL && uncle->color_ == RED) { //uncle is red.
                printf("[right]: situation 1\n");
                z->printmyself();
                printf("[right] will change parent[%d][%p] to black, uncle[%d][%p] to black, grandparent[%d][%p]=red\n"
                       , parent->value_, parent, uncle->value_, uncle, grandparent->value_, grandparent);

                parent->color_ = BLACK;
                uncle->color_ = BLACK;
                grandparent->color_ = RED;
                //now move upword.
                z = grandparent;
                //PrintRBTFromRoot(z);
            } else if (uncle == NULL || uncle->color_ == BLACK) {
                if (z == parent->lc_) { //if z is left child, rotate right to make it right child.
                    printf("[right]situation 2: z=\n");
                    z->printmyself();
                    printf("[right]situation 2: parent=\n");
                    parent->printmyself();

                    z = parent;
                    RotateRight(z);
                    //PrintRBTFromRoot(z);
                }

                printf("[right]: situation 3: z=\n");
                z->printmyself();
                printf("[right]situation 2: parent=\n");
                z->parent_->printmyself();
                printf("[right]situation 2: parent=\n");
                z->parent_->parent_->printmyself();

                z->parent_->color_ = BLACK;
                z->parent_->parent_->color_ = RED;
                RotateLeft(z->parent_->parent_);
                //PrintRBTFromRoot(z);
            }
        } else { //grandparent==NULL
            printf("grand parent is NULL, parent is root\n");
            parent->color_ = BLACK;
            //PrintRBTFromRoot(z);
        }
    }

    while (z->parent_ != NULL)
        z = z->parent_;

    z->color_ = BLACK;
    PrintRBTFromRoot(z);
}

void Insert2RBT(tnode * root, tnode * z)
{
    if (root == NULL || z == NULL) {
        return;
    }

    printf("====================insert start========\n");
    tnode * cur = root;

    while (cur != NULL) {
        if (cur->value_ >= z->value_) {
            if (cur->lc_ != NULL)
                cur = cur->lc_;
            else {
                printf("insert \n");
                z->printmyself();
                printf("   as left child of \n");
                cur->printmyself();

                cur->lc_ = z;
                z->parent_ = cur;
                z->color_ = RED;
                break;
            }
        } else {
            if (cur->rc_ != NULL)
                cur = cur->rc_;
            else {
                printf("insert \n");
                z->printmyself();
                printf("    as right child of \n");
                cur->printmyself();

                cur->rc_ = z;
                z->parent_ = cur;
                z->color_ = RED;
                break;
            }
        }
    }

    InsertFix(z);
    printf("====================insert done========\n");
}

tcolor getColor(tnode* node)
{
    if (node==NULL) return BLACK;
    return node->color_;
}

void deleteFix(tnode * root, tnode * x)
{
    printf("fix for node :\n");
    x->printmyself();
    while (x != root && x->color_ == BLACK)
    {
        if (x == x->parent_->lc_) {
            tnode * w = x->parent_->rc_;
            if (w->color_ == RED) {
		printf("right case 1\n");
                w->color_ = BLACK;
                w->parent_->color_ = RED;
                RotateLeft(w);
                w = x->parent_->rc_;
            }
            if (getColor(w->lc_)==BLACK && getColor(w->rc_) == BLACK) {
		printf("right case 2\n");
                w->color_=RED;
                x = x->parent_;
            }
            else {
                if (getColor(w->rc_) == BLACK) { //left_->color==RED
		    printf("right case 3\n");
                    w->color_=RED;
                    w->lc_->color_=BLACK;
                    RotateRight(w);
                    w = x->parent_->rc_;
                }
		//now w->right_->color_==RED
		printf("right case 4\n");
                w->color_=RED;
                w->parent_->color_=BLACK;
                RotateLeft(w);
            }
        }
        else {  //if (x == x->parent_->left) {
            tnode * w = x->parent_->lc_;
            if (w->color_ == RED) {
                printf("right case 1\n");
                w->color_ = BLACK;
                w->parent_->color_ = RED;
                RotateRight(w);
                w = x->parent_->lc_;
            }
            if (getColor(w->lc_)==BLACK && getColor(w->rc_) == BLACK) {
                printf("right case 2\n");
                w->color_=RED;
                x = x->parent_;
            }
            else {
                if (getColor(w->lc_) == BLACK) { //right_->color==RED
                    printf("right case 3\n");
                    w->color_=RED;
                    w->rc_->color_=BLACK;
                    RotateLeft(w);
                    w = x->parent_->lc_;
                }
		//now w->left_->color_==RED
                printf("right case 4\n");
                w->color_=RED;
                w->parent_->color_=BLACK;
                RotateRight(w);
            }
        }
    }
    x->color_=BLACK;
}

void ReplaceAtoB(tnode * a, tnode* b)
{
    if (a->parent_!=NULL) {
        if(a->parent_->lc_==a) a->parent_->lc_=b;
        else a->parent_->rc_=b;
    }
    if (b!=NULL)
    	b->parent_=a->parent_;
    if (a->lc_!=NULL) {
        a->lc_->parent_=b;
    }
    if (b!=NULL)
    	b->lc_=a->lc_;
    if (a->rc_!=NULL) {
        a->rc_->parent_=b;
    }
    if (b!=NULL) {
    	b->rc_=a->rc_;
        b->color_=a->color_; 
    }
}

void deleteFromRBT(tnode * root, tnode * z)
{
    tnode * y=NULL;
    tnode * x=NULL;
    tcolor yocolor = z->color_;
    if (z->lc_!=NULL && z->rc_==NULL) {
        y = z->lc_;
        x = y;
    }
    else if (z->rc_!=NULL && z->lc_==NULL) {
        y = z->rc_;
        x = y;
    }
    else if (z->rc_==NULL && z->lc_==NULL){
        y = NULL;
        x = y;
    }
    else {
        y = GetSuccessor(z);
        yocolor = y->color_;
        x = y->rc_;
	if (x!=NULL) {
            x->parent_=y->parent_;
            //x->color_=y->color;
        }
	if (y == y->parent_->lc_) y->parent_->lc_=x;
	else y->parent_->rc_=x;
	
    }
    ReplaceAtoB(z, y);
    //printf("node to delete is :\n");
    //z->printmyself();
    //printf("node to replace is :\n");
    //y->printmyself();
    //printf("node to replace the replace is :\n");
    //if (x==NULL) printf("NULL\n");
    //else x->printmyself();
    //if (x!=NULL && x->color_==BLACK) {
    if (yocolor == BLACK) {
        deleteFix(root, x);
    }
}

#define TOTAL  13
int main(int argc, char * argv[])
{
    int input[TOTAL] = { 83,  86,  77,  15,  93,  35,  88,  92,  49,  21, 100, 80, 90};
    printf("input={");

    for (int i = 0; i < TOTAL; i++) {
        //input[i]=rand()%100;
        printf(" %d ", input[i]);
    }

    printf("}\n");
    tnode* root = new tnode();//gen a root first.
    root->value_ = 50;

    for (int i = 0; i < TOTAL; i++) {
        tnode * node = new tnode();
        node->value_ = input[i];

        while (root->parent_ != NULL) //find the new root.
            root = root->parent_;

        Insert2RBT(root, node);
    }

    printf("*******************done insert*****************\n");
    PrintRBTFromRoot(root);

    printf("*******************start delete*****************\n");
    tnode* pd = root->lc_;
    deleteFromRBT(root, pd);
    printf("*******************done delete*****************\n");
    PrintRBTFromRoot(root);
    return 0;
}
