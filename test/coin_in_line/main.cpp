#include "stdlib.h"
#include "stdio.h"

static const int SIZE = 4;
static const int input1[SIZE] = {1, 2, 5, 3};

void init()
{
}
bool myturn = true;
int res1[SIZE] = {0, 0, 0, 0};
int res2[SIZE] = {0, 0, 0, 0};
int p[SIZE][SIZE] = {0};


int calcP(const int input[], int startPos, int endPos)
{
    for (int i = 0; i < SIZE; i++) { //i=0 means this very element
        for (int m = 0, n = i; n < SIZE; m++, n++) { //determine p[m][n]
            //p[m+2][n]: user1 choose m, user2 choose m+1
            //p[m+1][n-1]: user1 choose m, user2 choose n, or user1 choose n, user2 choose m
            //p[m][n-2]: user1 choose n, user2 choose n-1
            int pm2n0 = m + 2 < SIZE - 1 ? p[m + 2][n] : 0;
            int pm1n1 = (m + 1 < SIZE - 1) && (n - 1 >= 0) ? p[m + 1][n - 1] : 0;
            int pm0n2 = n - 2 >= 0 ? p[m][n - 2] : 0;
            int u1m = input[m] + (pm2n0 < pm1n1 ? pm2n0 : pm1n1);
            int u1n = input[n] + (pm0n2 < pm1n1 ? pm0n2 : pm1n1);
            p[m][n] = u1m > u1n ? u1m : u1n;
            printf("i=%d, m=%d, n=%d, pm2n0=%d, pm1n1=%d, pm0n2=%d, u1m=%d,u1n=%d, p[m][n]=%d, input[m]=%d, input[n]=%d\n"
                   , i, m, n, pm2n0, pm1n1, pm0n2, u1m, u1n, p[m][n], input[m], input[n]);
        }
    }

}
void printChoose()
{
    printf("P:\n");

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", p[i][j]);
        }

        printf("\n");
    }

    printf("the choose:\n");
    int first = 0;
    int last = SIZE - 1;

    for (int i = 0; i < SIZE; i++) {
        if (p[first + 1][last] > p[first][last - 1]) {
            printf("user%d choose %dth elem\n", i % 2, last);
            last--;
        } else {
            printf("user%d choose %dth elem\n", i % 2, first);
            first++;
        }
    }

}
int main(int argc, char *argv[])
{
    calcP(input1, 0, 3);
    printChoose();
    return 0;
}
