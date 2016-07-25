#include "stdlib.h"
#include "stdio.h"
#include <string.h>

static const char * input1 = "abcab";
static const char * input2 = "abcdefg";
static const char * input3 = "aaaaa";
static const char * input4 = "aabcffghijklmnnn";

static const int ASCBASE = 97;

struct Data {
    int isOccupied;
    int pos;
};

Data record[26];

int getLongest(const char * input, int inlen, int & startPos)
{
    int maxLen = 0;
    //int curLen=0;
    int curStartPos = 0;
    startPos = 0;

    for (int i = 0; i < inlen; i++) {
        int cind = input[i] - ASCBASE;

        if (record[cind].isOccupied == 0) {
            record[cind].isOccupied = 1;
            record[cind].pos = i - curStartPos;

            //curLen++;
            if (i == inlen - 1) { //the end
                int curLen = i - curStartPos + 1;

                if (maxLen < curLen) {
                    maxLen = curLen;
                    startPos = curStartPos;
                }
            }
        } else { //now duplicated
            int curLen = i - curStartPos;

            if (maxLen < curLen) {
                maxLen = curLen;
                startPos = curStartPos;
            }

            for (int j = 0; j < 26; j++) {
                if (record[j].pos < record[cind].pos) {
                    record[j].isOccupied = 0;
                    record[j].pos = 0;
                }
            }

            curStartPos = record[cind].pos + curStartPos + 1;
            i += curLen;
        }
    }

    return maxLen;
}

void test(const char * input)
{
    memset(record, 0, sizeof(record));
    int startPos = 0;
    int maxlen = getLongest(input, strlen(input), startPos);
    printf("input=%s, maxlen=%d, starting from %d\n", input, maxlen, startPos);

}
int main(int argc, char * argv[])
{

    test(input1);
    test(input2);
    test(input3);
    test(input4);
    return 0;
}
