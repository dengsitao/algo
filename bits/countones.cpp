/**** count how many 1s are there in a binary number ***/

#include "stdio.h"
#include "stdlib.h"

int main(int argc, char * argv[])
{
	if (argc!=2) {
		printf("input a number\n");
		return -1;
	}
	int input = atoi(argv[1]);
	printf("   %d has ", input);
	int count=0;
	while(input!=0) {
		if (input%2==1) count++;
		input=input>>1;
	}
	printf("%d 1s in its binary form\n", count);
	return 0;
}
