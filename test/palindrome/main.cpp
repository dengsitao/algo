#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include <cstring>
#include "time.h"
#include "stdint.h"

const char * input1 = "abccba";
const char * input2 = "abcdcba";
const char * input3 = "abcdecba";
static int const LOOP = 10;
static int const MAXRAND = 1000000;
uint32_t arr[LOOP];
void init()
{

    time_t time_;
    time(&time_);
    srand(time_);

    for (int i = 0; i < LOOP; i++) {
        arr[i] = rand() % MAXRAND;
        //printf("%d ", arr[i]);
    }

    arr[0] = 1221;
    arr[1] = 12321;
    arr[2] = 123421;
    //printf("\n");
}
bool isPalindromeStr(const char * input, int length)
{
    for (int i = 0; i < length / 2; i++) {
        if (input[i] != input[length - 1 - i])
            return false;
    }

    return true;
}

static const int MAX = 10;
uint32_t xy(uint32_t x, uint32_t y)
{
    if (y == 0) return 1;

    uint32_t res = x;

    while (y > 1) {
        res = res * x;
        y--;
    }

    return res;
}
int getUintDigitCount(uint32_t v, int base)
{
    int count = 0;

    while (count < MAX) {
        if (v > xy(base, count))
            count++;
        else
            break;
    }

    //printf("getUintDigitCount: v=%u, base=%d return %d\n", v, base, count);
    return count;
}
int getUintDigit(uint32_t v, int base, int pos, int count)
{
    //printf("getUintDigit: v=%u, base=%d, pos=%d, count=%d\n", v, base, pos, count);
    int dis = count - pos;

    while (dis > 0) {
        int dc = 1;
        uint32_t bb = xy(base, count - 1);

        while (v >= dc * bb)
            dc++;

        v -= (dc - 1) * bb;
        dis--;
        count--;
    }

    int dc = 1;

    while (v >= dc * xy(base, count - 1))
        dc++;

    //printf("retuen %d\n", dc-1);
    return dc - 1;



}
int main(int argc, char* argv[])
{
    //getUintDigitCount(12345, 10);
    //getUintDigitCount(145, 10);
    //getUintDigitCount(123456789, 10);

    getUintDigit(123450, 10, 1, 6);
    //getUintDigit(123450, 10, 2, 6);
    //getUintDigit(123450, 10, 3, 6);
    //getUintDigit(123450, 10, 4, 6);
    //getUintDigit(123450, 10, 5, 6);
    //getUintDigit(123450, 10, 6, 6);
    //return 0;
    init();

    for (int j = 0; j < LOOP; j++) {
        int length = getUintDigitCount(arr[j], 10);
        bool isP = true;

        for (int i = 1; i <= length / 2; i++) {
            int digitleft = getUintDigit(arr[j], 10, i, length);
            int digitright = getUintDigit(arr[j], 10, length - i + 1, length);

            if (digitleft != digitright) {
                printf("v=%d is not palindrome\n", arr[j]);
                isP = false;
                break;
            }
        }

        if (isP)
            printf("v=%d is palindrome\n", arr[j]);

    }

    return 0;
    printf("input1=%s, is palindrome?%d\n", input1, isPalindromeStr(input1, 6));
    printf("input2=%s, is palindrome?%d\n", input2, isPalindromeStr(input2, 7));
    printf("input3=%s, is palindrome?%d\n", input3, isPalindromeStr(input3, 8));

    init();
    char numStr[512];

    for (int i = 0; i < LOOP; i++) {
        int input = arr[i];
        sprintf(numStr, "%d", input);
        printf("input=%d, is palindorome=%d\n", input, isPalindromeStr(numStr, strlen(numStr)));
    }

    {
        int input = 1234321;
        sprintf(numStr, "%d", input);
        printf("input=%d, is palindorome=%d\n", input,  isPalindromeStr(numStr, strlen(numStr)));
    }

    {
        int input = 123321;
        sprintf(numStr, "%d", input);
        printf("input=%d, is palindorome=%d\n", input,  isPalindromeStr(numStr, strlen(numStr)));
    }

    {
        int input = 12356321;
        sprintf(numStr, "%d", input);
        printf("input=%d, is palindorome=%d\n", input,  isPalindromeStr(numStr, strlen(numStr)));
    }

    return 0;
}
