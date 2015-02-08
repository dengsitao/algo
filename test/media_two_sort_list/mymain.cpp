#include "stdlib.h"
#include "stdio.h"
#include <time.h>
#include <strings.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include "math.h"
#include "errno.h"
#include <iostream>
#include <fstream>
#include <sstream>


//static const int LOOPCOUNT = 1;//0000;
//static const int ARRAYSIZE = 16;//0000;
//static const unsigned int MAXRAND = 20;//0000;
static const int LOOPCOUNT = 10000;
static const int ARRAYSIZEA = 10;
static const int ARRAYSIZEB = 5;

static const unsigned int MAXRAND = 10000;
static int arrA[ARRAYSIZEA];
static int arrB[ARRAYSIZEB];
static int verifyAB[ARRAYSIZEA+ARRAYSIZEB];

void init()
{
    for (int i=0;i<ARRAYSIZEA;i++)
    {
        arrA[i]=i*7;
        printf("%d ", arrA[i]);
    }
    printf("\n");
    for (int i=0;i<ARRAYSIZEB;i++)
    {
        arrB[i]=i*2+3;
        printf("%d ", arrB[i]);
    }
    printf("\n");
}
int findM2Sort(int startPosA, int endPosA, int startPosB, int endPosB)
{
    printf("enter startPosA=%d endPosA=%d, startPosB=%d endPosB=%d\n", startPosA,endPosA, startPosB, endPosB);
    for (int i=startPosA;i<=endPosA;i++)
    {
        printf("%d ", arrA[i]);
    }
    printf("\n");
    for (int i=startPosB;i<=endPosB;i++)
    {
        printf("%d ", arrB[i]);
    }
    printf("\n");
    int sizea=endPosA-startPosA;
    int sizeb=endPosB-startPosB;
    if(sizea==0&&sizeb==0)
    {
        printf("return1 startPosA=%d endPosA=%d, startPosB=%d endPosB=%d\n", startPosA,endPosA, startPosB, endPosB);
        return (arrA[startPosA]+arrB[startPosB])/2;
        }
    if(sizea==1&&sizeb==0)
    {
        printf("return2 startPosA=%d endPosA=%d, startPosB=%d endPosB=%d\n", startPosA,endPosA, startPosB, endPosB);
        return (arrB[startPosB]>arrA[startPosA]?arrB[startPosB]:arrA[startPosA]);
        }
    if(sizea==0&&sizeb==1)
    {
        printf("return3 startPosA=%d endPosA=%d, startPosB=%d endPosB=%d\n", startPosA,endPosA, startPosB, endPosB);
        return (arrA[startPosA]>arrB[startPosB]?arrA[startPosA]:arrB[startPosB]);
        }
    if(sizea==1&&sizeb==1)
    {
        printf("return4 startPosA=%d endPosA=%d, startPosB=%d endPosB=%d\n", startPosA,endPosA, startPosB, endPosB);
        int mid1=(arrA[startPosA]>arrB[startPosB]?arrA[startPosA]:arrB[startPosB]);
        int mid2=(arrA[endPosA]>arrB[endPosB]?arrB[endPosB]:arrA[endPosA]);
        return (mid1+mid2)/2;
    }
    int lenA=(endPosA-startPosA+1);
    int lenB=(endPosB-startPosB+1);
    printf("lenA=%d, lenB=%d\n", lenA, lenB);
    if (lenA==2)
    {
        //merge A[startPosA, endPosA] to B then find the median.
        if (lenB%2==1)
        {
            int midindexB = startPosB+lenB/2;
            if(arrA[startPosA]<arrB[midindexB] && arrA[endPosA]>arrB[midindexB])
            {
                return arrB[midindexB];
            }
            else if(arrA[startPosA]>arrB[midindexB] && arrA[endPosA]>arrB[midindexB])
            {
                return arrB[midindexB+1];
            }
            else if(arrA[startPosA]<arrB[midindexB] && arrA[endPosA]<arrB[midindexB])
            {
                return arrB[midindexB-1];
            }
        }
        else
        {
            int midindexB1 = startPosB+lenB/2;
            int midindexB2=midindexB1+1;
            return findM2Sort(startPosA, endPosA, midindexB1, midindexB2);
        }
        
    }
    if (lenB==2)
    {
        //merge B[startPosA, endPosA] to A then find the median.
        if (lenA%2==1)
        {
            int midindexA = startPosA+lenA/2;
            if(arrB[startPosB]<arrA[midindexA] && arrB[endPosB]>arrA[midindexA])
            {
                return arrA[midindexA];
            }
            else if(arrB[startPosB]>arrA[midindexA] && arrB[endPosB]>arrA[midindexA])
            {
                return arrA[midindexA+1];
            }
            else if(arrB[startPosB]<arrA[midindexA] && arrB[endPosB]<arrA[midindexA])
            {
                return arrA[midindexA-1];
            }
        }
        else
        {
            int midindexA1 = startPosA+lenA/2;
            int midindexA2=midindexA1+1;
            return findM2Sort(midindexA1, midindexA2, startPosB, endPosB);
        }
    }
    
    int indexA=startPosA+lenA/2;
    int indexB=startPosB+lenB/2;
    int vA=arrA[indexA];
    int vB=arrB[indexB];

    if (vA<vB)
    {
        if (lenA%2==1)
            indexA++;
        int step=indexA-startPosA;
        if(lenA>lenB)
            step=endPosB-indexB;
        return findM2Sort(startPosA+step, endPosA, startPosB, endPosB-step);
    }
    else
    {
        if (lenB%2==1)
            indexB++;
        int step=indexB-startPosB;
        if(lenA<lenB)
            step=endPosA-indexA;
        return findM2Sort(startPosA, endPosA-step, startPosB+step, endPosB);
    }
    
}

int verify()
{
    int indexA=0;
    int indexB=0;
    for (int i=0;i<ARRAYSIZEA+ARRAYSIZEB;i++)
    {
        if (indexA<ARRAYSIZEA && indexB<ARRAYSIZEB)
        {
            if(arrA[indexA]<arrB[indexB])
            {
                verifyAB[i]=arrA[indexA++];
            }
            else
            {
                verifyAB[i]=arrB[indexB++];
            } 
        }
        else if (indexA>=ARRAYSIZEA)
        {
            verifyAB[i]=arrB[indexB++];
        }
        else if (indexB>=ARRAYSIZEB)
        {
            verifyAB[i]=arrA[indexA++];
        }
        printf("%d ", verifyAB[i]);
    }
    printf("\n");
    if ((ARRAYSIZEA+ARRAYSIZEB)%2==0)
    {
        printf("verify return even %d and %d\n", verifyAB[(ARRAYSIZEA+ARRAYSIZEB)/2], verifyAB[(ARRAYSIZEA+ARRAYSIZEB)/2+1]);
        return (verifyAB[(ARRAYSIZEA+ARRAYSIZEB)/2]+verifyAB[(ARRAYSIZEA+ARRAYSIZEB)/2+1])/2;
        }
    else
    {
        printf("verify return odd %d\n", verifyAB[(ARRAYSIZEA+ARRAYSIZEB)/2]);
        return verifyAB[(ARRAYSIZEA+ARRAYSIZEB)/2];
        }
}
////////////Main////////////
int main(int argc, char * argv[])
{
    init();
    int median = findM2Sort(0, ARRAYSIZEA-1, 0, ARRAYSIZEB-1);
    printf("median=%d\n", median);
    printf("correct median=%d\n", verify());
    return 0;
}

/////////////////end///////////////////////







