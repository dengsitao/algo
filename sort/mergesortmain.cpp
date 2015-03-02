#include "stdlib.h"
#include "stdio.h"
#include <climits>
#include <time.h>
#include <sys/time.h>


static const int MAXRAND=10000;
static const int SIZE=1000;
static const int LOOP=1000;
int intArr[SIZE];
int resArr[SIZE];
int resArr2[MAXRAND];
class intNode
{
public:
    int value;
    intNode * next;
    intNode * prev;
    intNode(int v):value(v),next(NULL),prev(NULL){};
};
intNode * starter=NULL;
intNode * res=NULL;

inline unsigned long GetElapsedTimeInMicroS()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (tv.tv_sec*1000*1000)+(tv.tv_usec);
}
int generateRandInt(int randMax)
{
    return rand()%randMax;
}
    
void generateInput()
{
    time_t time_;
    time(&time_);
    srand(time_);
    //for direct method.
    for (int count=0; count<SIZE; count++)
    {
        intArr[count]=generateRandInt(MAXRAND);
    }
    //for linkedlist solution.
    starter=new intNode(intArr[0]);
    intNode * prevNode=starter;
    for(int count=1;count<SIZE;count++)
    {
        intNode * p=new intNode(intArr[count]);
        prevNode->next=p;
        p->prev=prevNode;
        prevNode=p;
    }

}
void printArr(int *arr, int length)
{
    for (int i=0;i<length;i++)
    {
        printf("%d ", arr[i]);
        if (i%20==0 && i!=0)
            printf("\n");
    }
}
void printLL(intNode * first)
{
    int count=0;
    while(first!=NULL)
    {
        printf("%d ", first->value);
        if (count%20==0 && count!=0)
            printf("\n");
        count++;
        first=first->next;
    }
}
void validateArr(int *arr, int length)
{
    if(length<2) return;
    for (int i=0;i<length-1;i++)
    {
        if(arr[i]>arr[i+1])
            printf("[ERROR]: arr[%d]=%d > arr[%d]=%d\n", i, arr[i], i+1, arr[i+1]);
    }
}
void validateLL(intNode * first)
{
    int count=0;
    while(first!=NULL)
    {
        if(first->next!=NULL && first->value>first->next->value)
            printf("[ERROR]: arr[%d]=%d > arr[%d]=%d\n", count, first->value, count+1, first->next->value);
        count++;
        first=first->next;
    }
}

void mergeSort()
{
    for(int i=0;i<SIZE;i++)
    {
        int replacedValue=-1;
        for(int j=0;j<=i;j++)
        {
            if(replacedValue==-1)
            {
                if(resArr[j]<=intArr[i])
                   continue;
                else
                {
                    replacedValue=resArr[j];
                    resArr[j]=intArr[i];
                }
            }
            else
            {
                int temp=resArr[j];
                resArr[j]=replacedValue;
                replacedValue=temp;
            }
        }
    }
}

void mergeSortLL()
{
    intNode * input=starter->next;
    //intNode * output=res;
    res=new intNode(starter->value);
    while(input!=NULL)
    {
        intNode * output=res;
        intNode * prev=NULL;
        while(output!=NULL && output->value<=input->value)
        {
            prev=output;
            output=output->next;
        }
        intNode * temp=new intNode(input->value);
        //printf("insert %d between %d and %d\n", input->value, prev==NULL?INT_MAX:prev->value, output==NULL?INT_MAX:output->value);
        if(prev!=NULL)
            prev->next=temp;
        else
            res=temp;
        temp->prev=prev;
        if(output!=NULL)
            output->prev=temp;
        temp->next=output;
        input=input->next;
    }
}
void mergeSortBucket()
{
    for (int i=0;i<SIZE;i++)
    {
        resArr2[intArr[i]]++;
    }
    int pos=0;
    for (int i=0;i<MAXRAND;i++)
    {
        while(resArr2[i]-->0)
        {
           resArr[pos++]=i;
        }
    }
}
int main(int argc, char * argv[])
{
    for(int i=0;i<SIZE;++i)
    {
        resArr[i]=INT_MAX;
    }
    for(int i=0;i<MAXRAND;++i)
    {
        resArr2[i]=0;
    }
    unsigned long sortTime=0;//GetElapsedTimeInMicroS();
    unsigned long sortTimeLL=0;//GetElapsedTimeInMicroS();
    unsigned long sortTimeB=0;//GetElapsedTimeInMicroS();
    for(int k=0;k<LOOP;k++)
    {
        generateInput();
        //printf("intArr:\n");
        //printArr(intArr, SIZE);
        //printf("\n");
        {
            unsigned long startTime=GetElapsedTimeInMicroS();
            mergeSort();
            unsigned long endTime=GetElapsedTimeInMicroS();
            sortTime+=(endTime-startTime);
        }
        //printf("resArr:\n");
        //printArr(resArr, SIZE);
        //printf("\n");
        //printf("intLL:\n");
        //printLL(starter);
        //printf("\n");
        {
            unsigned long startTime=GetElapsedTimeInMicroS();
            mergeSortLL();
            unsigned long endTime=GetElapsedTimeInMicroS();
            sortTimeLL+=(endTime-startTime);
        }
        
        for(int i=0;i<SIZE;++i)
        {
            resArr[i]=INT_MAX;
        }
        {
            unsigned long startTime=GetElapsedTimeInMicroS();
            mergeSortBucket();
            unsigned long endTime=GetElapsedTimeInMicroS();
            sortTimeB+=(endTime-startTime);
            //validateArr(resArr, SIZE);
            //printArr(resArr, SIZE);
        }
        //printf("resLL:\n");
        //printLL(res);
        //printf("\n");
    }
    printf("merge sort using array      take %llu micro seconds\n", sortTime);
    printf("merge sort using linkedlist take %llu micro seconds\n", sortTimeLL);
    printf("merge sort using Bucket     take %llu micro seconds\n", sortTimeB);
    return 0;
}
