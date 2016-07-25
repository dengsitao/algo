#include "stdlib.h"
#include "stdio.h"
#include "time.h"

static const int SIZE = 10;
static const int MAXRAND = 10;
static int ia[SIZE] = {0};
static int input1[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
static int input2[SIZE] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

int generateRandInt(int randMax)
{
    return rand() % randMax;
}

void init()
{
    time_t time_;
    time(&time_);
    srand(time_);

    for (int count = 0; count < SIZE; count++) {
        ia[count] = generateRandInt(MAXRAND);
        printf("%d ", ia[count]);
    }

    printf("\n");

}

void getBuySellDate(int input[], int & buyTime, int & sellTime)
{
    int max = 0;
    buyTime = 0;
    sellTime = 0;

    for (int i = 0; i < SIZE - 1; i++) {
        if (input[i] >= input[i + 1])
            continue;

        for (int j = i + 1; j < SIZE; j++) {
            {
                if (j == SIZE - 1) {
                    int res = input[j] - input[i];

                    if (max < res) {
                        max = res;
                        buyTime = i;
                        sellTime = j;
                    }

                    return;
                } else if (input[j] < input[j + 1]) {
                    continue;
                } else {
                    int res = input[j] - input[i];

                    if (max < res) {
                        max = res;
                        buyTime = i;
                        sellTime = j;
                    }

                    i = j;
                }
            }
        }
    }
}
int main(int argc, char * argv[])
{
    int bt = 0;
    int st = 0;
    init();
    getBuySellDate(ia, bt, st);
    printf("bt=%d, st=%d\n", bt, st);
    getBuySellDate(input1, bt, st);
    printf("bt=%d, st=%d\n", bt, st);
    getBuySellDate(input2, bt, st);
    printf("bt=%d, st=%d\n", bt, st);
    return 0;
}
