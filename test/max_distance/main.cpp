#include "stdlib.h"
#include "stdio.h"

const static int SIZE=10;
const static int in1[SIZE]={1,2,3,4,5,6,7,8,9,10};
const static int in2[SIZE]={1,2,4,3,7,8,9,10,11,12};
const static int in3[SIZE]={4,5,1,2,3,4,5,6,7,8};
const static int in4[SIZE]={4,5,6,1,2,1,2,3,4};
const static int in5[SIZE]={10,9,8,7,6,5,4,3,2,1};

void getMaxDis(const int in[], int & startPos, int & endPos, int & maxValue)
{
	startPos=0; endPos=0; maxValue=0;
	int curStart=0;
	int count=0;
	for (int i=0;i<SIZE;i++)
	{
		if(in[i]>in[curStart])	
		{
			count++;
		}
		else if (in[i]==in[curStart] && i-curStart==1)
		{
			count++;
		}
		else
		{
			if (count>maxValue)
			{
				startPos=curStart;
				endPos=i;
				maxValue=count;
			}
			curStart=i;
		}
		if (i==SIZE-1)
		{
			if (count>maxValue)
			{
				startPos=curStart;
				endPos=i;
				maxValue=count;
				return;
			}
			else
			{
				return;
			}
		}
	}
}
int main(int argc, char * argv[])
{
	{
		int sp=0;
		int ep=0;
		int max=0;
		getMaxDis(in1, sp, ep, max);
		printf(" in in1, sp=%d, ep=%d, max=%d\n", sp, ep, max);
	}
	{
		int sp=0;
		int ep=0;
		int max=0;
		getMaxDis(in2, sp, ep, max);
		printf(" in in2, sp=%d, ep=%d, max=%d\n", sp, ep, max);
	}
	{
		int sp=0;
		int ep=0;
		int max=0;
		getMaxDis(in3, sp, ep, max);
		printf(" in in3, sp=%d, ep=%d, max=%d\n",sp,ep,max);
	}
	{
		int sp=0;
		int ep=0;
		int max=0;
		getMaxDis(in4, sp, ep, max);
		printf(" in in4, sp=%d, ep=%d, max=%d\n", sp,ep, max);
	}
	{
		int sp=0;
		int ep=0;
		int max=0;
		getMaxDis(in5, sp, ep, max);
		printf(" in in5, sp=%d, ep=%d, max=%d\n",sp, ep, max);
	}
	return 0;
}
	
