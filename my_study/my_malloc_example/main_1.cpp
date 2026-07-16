#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#define MEM_BLOCK 16
#define MEM_BLOCK_SIZE 1024
#define USED 1
#define UNUSED 0

class MemoryMeta
{
	public:
		int blockCount;
		int mInUse;
};
static int mInit=false;
unsigned char mMemory[MEM_BLOCK][MEM_BLOCK_SIZE];
MemoryMeta mMemoryMeta[MEM_BLOCK];

void * my_malloc(unsigned int size)
{
	if (mInit==false) {
		memset(mMemory, 0, MEM_BLOCK*MEM_BLOCK_SIZE);
		memset(mMemoryMeta, 0, MEM_BLOCK*sizeof(MemoryMeta));
		mInit=true;

	}
	printf("malloc %d\n");
	int blockCount = size/MEM_BLOCK_SIZE;
	if (size%MEM_BLOCK_SIZE!=0) blockCount++;
 
	int found=0;
	for (int i=0;i<MEM_BLOCK-blockCount;i++) {
		if (mMemoryMeta[i].mInUse==UNUSED) {
			int j=i;
			for (;j<i+blockCount;j++) {
				if (mMemoryMeta[j].mInUse==USED) {
					i=j+1;
					break;
				}
			}
			if (j==i+blockCount) {
				found=1;
			}
		}
		if (found==1) {
			for (int j=i; j<i+blockCount; j++) {
				mMemoryMeta[j].mInUse = USED;
				mMemoryMeta[j].blockCount=blockCount;
				printf("mMemoryMeta[%d] is set to %d %d\n", j, mMemoryMeta[j].mInUse, mMemoryMeta[j].blockCount);
			}
			return mMemory[i];
			
		}
		
	}
	return NULL;
}


void my_free(void * pMem)
{
	int index=0;
	for (;index < MEM_BLOCK; index++) {
		if (pMem == mMemory[index]){
			int blockCount=mMemoryMeta[index].blockCount;
			for (int i=index;i<index+blockCount;i++) {
				mMemoryMeta[i].mInUse=UNUSED;
				mMemoryMeta[i].blockCount=0;
			}
			return;
		}
	}
}

void printMemoryMeta(int startIndex, int endIndex)
{
	for (startIndex;startIndex<endIndex && startIndex+1<MEM_BLOCK;startIndex+=2) {
		printf("mMemoryMeta[%d]= %d %d, [%d]=%d %d\n", 
			startIndex,
			mMemoryMeta[startIndex].mInUse, mMemoryMeta[startIndex].blockCount,
			startIndex+1,
			mMemoryMeta[startIndex+1].mInUse, mMemoryMeta[startIndex+1].blockCount);
		
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

}
