#include "stdlib.h"
#include "stdio.h"
#include <ostream>
#include <iostream>
#include <sys/time.h>


void swap(int * a, int * b)
{
	int temp=*a;
	*a=*b;
	*b=temp;
}
void mydumpSort(int * array, int size)
{
	for (int i=0;i<=size-1;i++) {
		for (int j=i+1;j<=size-1;j++) {
			if (array[i]>array[j]) {
				swap(&array[i], &array[j]);
			}
		}
	}
}


void myQuickSort(int * array, int size)
{
	if (array==NULL || size<=0) return;

	int index=size-1;	
	int indexValue=array[index];
	int pos=0;
	for (int i=0;i<size-1;i++){
		if (array[i]<indexValue) {
			if (i!=pos) {
				swap(&array[i], &array[pos]);
			}
			pos++;
		}
	}
	swap(&array[pos], &array[index]);
	myQuickSort(array, pos);
	myQuickSort(array+pos+1, size-pos-1);

}

#define SIZE 10
int test[SIZE]={4,2,1,8,7,0,9,3,6,5};

void printArray(int * const array, const int size)
{
	if (array==NULL) return;
	for(int i=0;i<size;i++){
		printf(" %d ", array[i]);
	}
	printf("\n");
}

bool validateSort(int * array, int size)
{
	for (int i =0;i<size-1;i++)
	{
		if (array[i]>array[i+1]) {
			printf("failed\n");
		       	return false;
		}
	}
	printf("success\n");
	return true;
}

int * myInput=NULL;
void genTestArray(int size)
{
	timeval times;
	gettimeofday(&times, NULL);
	srand(times.tv_sec);

	myInput=new int[size];
	for (int i=0;i<size;i++) {
		myInput[i]=rand()%size;
	}
}

int main(int argc, char * argv[])
{
	//mydumpSort(test, SIZE);
	genTestArray(1024*1024);
	myQuickSort(myInput, 1024*1024);
	validateSort(myInput, 1024*1024);
	//printArray(myInput, 1024);	
	return 0;
}
