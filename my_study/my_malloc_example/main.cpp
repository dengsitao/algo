#include "stdio.h" 
#include "stdlib.h" 
#include "unistd.h"
#include <pthread.h>

#define BLOCK 16
#define SIZE 1024
unsigned char mMemory[BLOCK][SIZE];

class MemoryMeta
{
public:
	void * mAddr;
	bool mIsFree;
	unsigned int mNumBlocks;
	MemoryMeta(): mAddr(NULL), mNumBlocks(0), mIsFree(true) {}
	MemoryMeta(void * p, int nb): mAddr(p), mNumBlocks(nb), mIsFree(true) {}
	~MemoryMeta() {}
};

MemoryMeta mMemoryMeta[BLOCK];

void * my_malloc(unsigned int size)
{
	//calculate how many blocks needed.
	int numOfBlocks = size/SIZE;
	if (size%SIZE !=0) numOfBlocks++;
	//printf("myMalloc size=%u numOfBlocks=%d\n", size, numOfBlocks);
	//find an available block
	int index=0;
       	for (index;index<BLOCK;index++) {
		bool hasEnough=true;
		if (mMemoryMeta[index].mIsFree==true) {
			//found a free block, check if it followed by enough blocks
			for (int i=index;i<index+numOfBlocks;i++) {
				if (mMemoryMeta[i].mIsFree==false) {
					hasEnough=false;
					break;
				}
			}
			//has enough blocks.makr all of them used.
			if (hasEnough==true) {
				for (int i=index;i<index+numOfBlocks;i++) {
					mMemoryMeta[i].mAddr=mMemory[index];
					mMemoryMeta[i].mIsFree=false;
					mMemoryMeta[i].mNumBlocks=numOfBlocks;
				}
				return mMemory[index];
			}
		}
	}	
	return NULL;
}

void my_free(void * p)
{
	if (p == NULL) return;
	for ( int i=0;i<BLOCK;i++){
		if (mMemoryMeta[i].mAddr == p) {
			//find the block, mark following numBlocks to be free.
			int numB = mMemoryMeta[i].mNumBlocks;
			for (int j=i;j<i+numB;j++) {
				mMemoryMeta[j].mAddr = NULL;
				mMemoryMeta[j].mNumBlocks=0;
				mMemoryMeta[j].mIsFree=true;
			}
			return;
		}
	}
}

void * re_alloc(void * p, unsigned int size)
{
	if (p==NULL || size ==0) return p;

	//calculate num of blocks needed
	int numOfBlocks = size/SIZE;
	if (size%SIZE != 0) numOfBlocks++;
	//find p's meta
	
	for ( int i=0;i<BLOCK;i++){
		if (mMemoryMeta[i].mAddr == p) {
			int numB = mMemoryMeta[i].mNumBlocks;
			//compare size and p's original size
			//if size > original size 
			//then need find bigger space
			//if size < original size, direct release trailing blocks.
			
			if (numOfBlocks > numB) {
				my_free(p);
				void * pNew =my_malloc(size);
				return pNew;
			}
			else if (numOfBlocks == numB) {
				return p;
			}
			else {
				int blocksToRelease = numB-numOfBlocks;
				int index=i+numOfBlocks;
				for (int j=i;j<i+numB;j++) {
					if (j<index) {
						mMemoryMeta[j].mNumBlocks=numOfBlocks;
					}
					else {
						mMemoryMeta[j].mAddr=NULL;
						mMemoryMeta[j].mNumBlocks=0;
						mMemoryMeta[j].mIsFree=true;
					}

				}
				return p;
			}
		}
	}
	printf("[ERROR]: not found input %p %ud\n", p, size);
	return NULL;
}

void printMemoryMeta(int start, int end)
{
	for (start;start<end && start+1<BLOCK;start+=2) {
		printf("mMemoryMeta[%d]= %d %p, [%d]=%d %p\n",
			start,
			mMemoryMeta[start].mIsFree, mMemoryMeta[start].mAddr,
			start+1,
			mMemoryMeta[start+1].mIsFree, mMemoryMeta[start+1].mAddr);

	}
}

int main(int argc, char * argv[])
{
	printf("malloc 2048\n");
	void * p = my_malloc(2048);
	printf("allocated p=%p 2048\n", p);
	printf("mMemory[0]=%p\n", mMemory[0]);
	printMemoryMeta(0,16);
	printf("malloc 2049\n");
	void * p1 = my_malloc(2049);
	printf("allocated p=%p\n", p1);
	printf("mMemory[2]=%p\n", mMemory[2]);
	printMemoryMeta(0,16);
	void * p2 = my_malloc(4096);
	printf("allocated p=%p 4096\n", p2);
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

	//printf("free p\n");
	//my_free(p);
	//printMemoryMeta(0,6);
	printf("free p1\n");
	my_free(p1);

	void * p6 = re_alloc(p,4096);
	printf("re-allocated p6=%p 4096\n", p6);
	printf("mMemory[0]=%p\n", mMemory[0]);
	printMemoryMeta(0,16);

	void * p7 = my_malloc(40960);
	printf("allocated p=%p\n", p7);
	printf("mMemory[0]=%p\n", mMemory[0]);
	printMemoryMeta(0,16);
	return 0;
}
