/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        bool flag=false;
        ListNode * curNode=NULL;
        ListNode * startNode=NULL;
        int curVal=-1;
        while (l1!=NULL && l2 != NULL) {
            curVal=l1->val+l2->val;
            if (flag==true) {
                curVal+=1;
            }
            if (curVal>=10) {
                flag=true;
                curVal-=10;
            }
            else {
                flag=false;
            }
            ListNode * thisNode=new ListNode(curVal);
            if (curNode==NULL) {
                curNode=thisNode;
                startNode=curNode;
            }
            else {
                curNode->next=thisNode;
            }
            curNode=thisNode;
            l1=l1->next;
            l2=l2->next;
        }
        while(l1!=NULL) {
            if (flag==true) {
                l1->val+=1;
                if (l1->val>=10) {
                    flag=true;
                    l1->val-=10;
                }
                else {
                    flag=false;
                }
            }
            curNode->next=l1;
            l1=l1->next;
            curNode=curNode->next;
        }
        while(l2!=NULL) {
            if (flag==true) {
                l2->val+=1;
                if (l2->val>=10) {
                    flag=true;
                    l2->val-=10;
                }
                else {
                    flag=false;
                }
            }
            curNode->next=l2;
            l2=l2->next;
            curNode=curNode->next;
        }
        if (flag==true) {
            ListNode * thisNode=new ListNode(1);
            if (curNode==NULL) {
                curNode=thisNode;
                startNode=curNode;
            }
            else {
                curNode->next=thisNode;
            }
        }
        return startNode;
    }
};
