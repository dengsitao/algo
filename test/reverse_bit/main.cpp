#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "stdint.h"

int main(int argc, char * argv[])
{
    uint32_t x = 0x654321;
    uint32_t y = 0;

    for (int i = 0; i < 16; i++) {
        y |= ((x & (1 << i)) << (31 - i - i));
        //printf("x=0x%x,i=%d,1<<i=0x%x, x&(1<<i)=0x%x, ((x&(1<<i))<<(31-i-i))=0x%x, y=0x%x\n",x,i, 1<<i, x&(1<<i), ((x&(1<<i))<<(31-i-i)),  y);
    }

    for (int i = 16; i < 31; i++) {
        y |= ((x & (1 << i)) >> (i + i - 31));
        //printf("x=0x%x,i=%d,1<<i=0x%x, x&(1<<i)=0x%x, ((x&(1<<i))<<(i+i-31))=0x%x, y=0x%x\n",x,i, 1<<i, x&(1<<i), ((x&(1<<i))<<(i+i-31)),  y);
    }


    printf("%x\n", y);
}
