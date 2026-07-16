#include "stdlib.h"
#include "stdio.h"
#include <ostream>
#include <iostream>
#include "time.h"

bool trueIfFind(int * array, int size, int find) {
	if (array == NULL || size<=0) return false;

	if (size==1) return array[0]==find;
	if (size==2) {
		if (array[0]==find || array[1]==find) return true;
		return false;
	}

	int middle=0;
	if (size%2==0) middle = size/2-1;
	else middle = (size+1)/2-1;

	//std::cout<<"array[middle]="<<array[middle]<<" size="<<size<<" middle="<<middle<<std::endl;
	if (array[middle] > find) {
		//std::cout<<"> array[0]="<<array[0]<<" size="<<size<<" middle="<<middle<<std::endl;
		return trueIfFind(array, middle, find);
	}
	else if (array[middle] < find) {
		//std::cout<<"< array[0]="<<array[0]<<" size="<<size<<" middle="<<middle<<std::endl;
		return trueIfFind(array+middle+1, middle, find);
	}
	else {
		return true;
	}

}

bool trueIfFind_slow(int * array, int size, int find) {
	int i=0;
	for (;i<size;i++) {
		if (array[i]==find) return true;
	}
	return false;
}


#define SIZE 11
int x[SIZE]= {1,3,4,8,9, 10, 11, 12,35,42, 49};
int x_find[SIZE]={0};
int x_exp[SIZE]={0};

int main(int argc, char * argv[])
{
	struct timespec curTime;
	clock_gettime(CLOCK_MONOTONIC, &curTime);

	unsigned int time_seed=curTime.tv_sec;
	srand(time_seed);
	for (int i=0;i<SIZE;i++) {
		int value=rand()%50;
		printf("[%d] v=%d\n", i, value);
		x_find[i]=value;
	}
	
	for (int i=0;i<SIZE;i++) {
		if (trueIfFind_slow(x, SIZE, x_find[i])) {
			x_exp[i]=1;
		}
		else {
			x_exp[i]=0;
		}
	}

	for (int i=0;i<SIZE;i++) {
		std::cout<<" expect "<<x_exp[i]<<" get "<<trueIfFind(x, SIZE, x_find[i])<<std::endl;
	}
#if 0 
	std::cout<<" expect 1 "<<trueIfFind(x, SIZE, 4)<<std::endl;
	std::cout<<" expect 1 "<<trueIfFind(x, SIZE, 1)<<std::endl;
	std::cout<<" expect 1 "<<trueIfFind(x, SIZE, 3)<<std::endl;
	std::cout<<" expect 1 "<<trueIfFind(x, SIZE, 4)<<std::endl;
	std::cout<<" expect 1 "<<trueIfFind(x, SIZE, 8)<<std::endl;
	std::cout<<" expect 1 "<<trueIfFind(x, SIZE, 9)<<std::endl;
	std::cout<<" expect 0 "<<trueIfFind(x, SIZE, 0)<<std::endl;
	std::cout<<" expect 1 "<<trueIfFind(x, SIZE, 10)<<std::endl;
	std::cout<<" expect 0 "<<trueIfFind(x, SIZE, 24)<<std::endl;
#endif

}
