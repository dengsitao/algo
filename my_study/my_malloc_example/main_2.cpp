#include <stdio.h>
#include <stdlib.h>
#include <ostream>
#include <iostream>


//declare memor for allocate
#define MBLOCK 16
#define MSIZE 1024
#define INUSE 1
#define NOTUSED 0

class MemoryMeta {
public:
	void * mpAddr;
	void * mpRealAddr;
	bool mInUse;

};
unsigned char mMemory[MBLOCK][MSIZE];
MemoryMeta mMemoryMeta[MBLOCK];
bool init=false;
int mTotalFreeMemBlocks=MBLOCK;

int findAvailableBlock(int nblocks)
{
	int index=0;
	bool found=false;
	for (index;index<MBLOCK-nblocks;index++) {
		if (mMemoryMeta[index].mInUse== NOTUSED) { //find the first available block
			int seekIndex=index;
			for (seekIndex;seekIndex<index+nblocks;seekIndex++) {//find if nblocks free.
				if (mMemoryMeta[seekIndex].mInUse!=NOTUSED) {//not enough
					index=seekIndex;
					break;
				}
			}
			if (seekIndex - index == nblocks) {//enough
				found=true;
				break;
			}
		}
	}
	if (found==true) return index;
	else return -1;

}

void * my_malloc(unsigned int size)
{
	if (false == init) {
		for (int i=0;i<MBLOCK;i++) {
			mMemoryMeta[i].mpAddr=NULL;
			mMemoryMeta[i].mInUse=NOTUSED;
		}
		init=true;
	}
	int numBlocks = size/MSIZE;
	if (numBlocks<=0) return NULL;
	if (size%MSIZE>0) numBlocks++;
	//find available memory blocks.
	unsigned char * p = NULL;
	int index = findAvailableBlock(numBlocks);
	//printf("found index=%d\n", index);
	if (index != -1) {
		for (int i=index;i<index+numBlocks;i++) {
			mMemoryMeta[i].mInUse=INUSE;
			mMemoryMeta[i].mpAddr = mMemory[index];
		}
		mTotalFreeMemBlocks-=numBlocks;
		return mMemory[index];
	}
	if (mTotalFreeMemBlocks > numBlocks) { //fragment
		printf("due to fragment it can't allocate\n");
		
	}
	return NULL;
}

void my_free(void * p)
{
	if (p==NULL) return;
	int index=0;
	for (index;index<MBLOCK;index++) {
		if (mMemoryMeta[index].mpAddr == p) {
			while(mMemoryMeta[index].mpAddr==p) {
				mMemoryMeta[index].mpAddr=NULL;
				mMemoryMeta[index].mInUse=NOTUSED;
				index++;
			}	
			return;
		}
	}

}

void printMemoryMeta(int start, int end)
{
	for (start;start<end && start+1<MBLOCK;start+=2) {
		printf("mMemoryMeta[%d]= %d %p, [%d]=%d %p\n",
			start,
			mMemoryMeta[start].mInUse, mMemoryMeta[start].mpAddr,
			start+1,
			mMemoryMeta[start+1].mInUse, mMemoryMeta[start+1].mpAddr);

	}
}


int main(int argc, char * argv[])
{
	printf("malloc 2048\n");
	void * p = my_malloc(2048);
	printf("allocated p=%p\n", p);
	printf("mMemory[0]=%p\n", mMemory[0]);
	printMemoryMeta(0,16);
	printf("malloc 2049\n");
	void * p1 = my_malloc(2049);
	printf("allocated p=%p\n", p1);
	printf("mMemory[2]=%p\n", mMemory[2]);
	printMemoryMeta(0,16);
	void * p2 = my_malloc(4096);
	printf("allocated p=%p\n", p2);
	printf("mMemory[5]=%p\n", mMemory[5]);
	printMemoryMeta(0,16);
	void * p3 = my_malloc(4096);
	printf("allocated p=%p\n", p3);
	printf("mMemory[9]=%p\n", mMemory[9]);
	printMemoryMeta(0,16);
	void * p4 = my_malloc(4444);
	printf("allocated p=%p\n", p4);
	printf("mMemory[13]=%p\n", mMemory[13]);
	printMemoryMeta(0,16);
	void * p5 = my_malloc(5555);
	printf("allocated p=%p\n", p5);
	printf("mMemory[13]=%p\n", mMemory[13]);
	printMemoryMeta(0,16);

	printf("free p\n");
	my_free(p);
	printMemoryMeta(0,6);
	printf("free p1\n");
	my_free(p1);

	void * p6 = my_malloc(4096);
	printf("allocated p=%p\n", p6);
	printf("mMemory[0]=%p\n", mMemory[0]);
	printMemoryMeta(0,16);

	void * p7 = my_malloc(40960);
	printf("allocated p=%p\n", p7);
	printf("mMemory[0]=%p\n", mMemory[0]);
	printMemoryMeta(0,16);
	return 0;
}
