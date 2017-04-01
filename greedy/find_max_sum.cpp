/***************************************
* find a max sum of a series of integers 
****************************************/

#include "stdlib.h"
#include "stdio.h"

int maxSubarray(int a[], int size) {
  if (size<=0) printf("error array size");
  int sum = 0;
  int max = - (1 << 31);
  int cur = 0;
  while (cur < size) {
    sum += a[cur++];
	printf("sum=%d\n", sum);
    if (sum > max) {
      max = sum;
    }
    if (sum < 0) {
      sum = 0;
    }
  }
  return max;
}

int main(int argc, char * argv[])
{
	int a[] = {-1,3,5,-10, 2};
	printf("max sub = %d\n", maxSubarray(a, 5));
	return 0;
}
