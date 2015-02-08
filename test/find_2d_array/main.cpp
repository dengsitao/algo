#include "stdlib.h"
#include "stdio.h"

static const int COLUMN=10;
static const int ROW=5;

int arr[ROW][COLUMN];

void init()
{
	for (int i=0;i<ROW;i++)
	{
		for(int j=0;j<COLUMN;j++)
		{
			arr[i][j]=i*ROW*2+j;
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}

void searchit(int v,int rstart, int cstart, int rlen, int clen, int & rindex, int& cindex)
{
	if (rindex!=-1||cindex!=-1)
		return;
	printf("searchit: v=%d, rstart=%d. cstart=%d, rlen=%d, clen=%d\n", v, rstart, cstart, rlen, clen);
	int ro=1;//-rlen%2;
	int co=1;//-clen%2;
	if(arr[rstart+rlen/2-ro][cstart+clen/2-co]>v)
		searchit(v, rstart, cstart, rlen/2, clen/2, rindex, cindex);
	else if(arr[rstart+rlen/2-ro][cstart+clen/2-co]<v)
	{
		if(arr[rstart+rlen/2-ro][cstart+clen-co]>v)
			searchit(v, rstart, cstart+clen/2, rlen/2, clen/2, rindex, cindex);
		else if(arr[rstart+rlen/2-ro][cstart+clen-co]<v)
		{
			if (arr[rstart+rlen-ro][cstart+clen/2-co]>v)
				searchit(v, rstart+rlen/2, cstart, rlen/2, clen/2, rindex, cindex);
			else if (arr[rstart+rlen-ro][cstart+clen/2-co]<v)
				searchit(v, rstart+rlen/2, cstart+clen/2, rlen/2, clen/2, rindex, cindex);
			else
			{
				rindex=rstart+rlen;
				cindex=cstart+clen/2;
			}
		}
		else
		{
			rindex=rstart+rlen/2;
			cindex=cstart+clen;
		}
	}
	else
	{
		rindex=rstart+rlen/2;
		cindex=cstart+clen/2;
	}
}
int main(int argc, char * argv[])
{
	if (argc==1)
	{
		printf("input a number \n");
		//getchar();
		return -1;
	}
	int v=atoi(argv[1]);
	init();
	int rindex=-1;
	int cindex=-1;
	searchit(v, 0,0, ROW, COLUMN, rindex, cindex);
	printf("rindex=%d, cindex=%d\n", rindex, cindex);
	return 0;
}
