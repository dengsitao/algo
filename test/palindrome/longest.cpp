#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include <cstring>
#include "time.h"
#include "stdint.h"

const char * input1 = "ababa";
const char * input2 = "abcdcbaab";
const char * input3 = "abcdecba";
bool isPalindromeStr_1(const char * input, int length)
{
    for (int i = 0; i < length / 2; i++) {
        if (input[i] != input[length - 1 - i])
            return false;
    }

    return true;
}
bool isPalindromeStr(const char * input, int length, int & maxLength)
{
    int startIndex = length / 2;
    maxLength = 0;

    if (length % 2 == 1) {
        for (int i = 1; i <= length / 2; i++) {
            maxLength++;

            if (input[startIndex - i] != input[startIndex + i]) {
                return false;
            }
        }
    } else {
        for (int i = 0; i < length / 2; i++) {
            if (input[startIndex - i] != input[startIndex + i])
                return false;

            maxLength++;
        }
    }

    return true;
}
int main(int argc, char* argv[])
{
    int max = 0;
    bool isP = isPalindromeStr(input1, strlen(input1), max);
    printf("input1=%s, is palindrome?%d, max=%d\n", input1, isP, max);
    isP = isPalindromeStr(input2, strlen(input2), max);
    printf("input2=%s, is palindrome?%d, max=%d\n", input2, isP, max);
    isP = isPalindromeStr(input3, strlen(input3), max);
    printf("input3=%s, is palindrome?%d, max=%d\n", input3, isP, max);
    return 0;
}
