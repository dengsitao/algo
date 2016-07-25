#include "stdlib.h"
#include "stdio.h"
int main(int argc, char * argv[])
{
    if (argc < 2) {
        printf(" add a char from A-Z as param\n");
        return -1;
    }

    char last = *(argv[1]);
    int HEIGHT = -1;

    for (int i = 0; i < 26; i++) {
        if (i == ((last) - 65)) {
            HEIGHT = i;
        }
    }

    if (HEIGHT == -1) {
        //printf("atoi(%s)=%d,   %s\n",last,  atoi(last), argv[0]);
        return -1;
    }

    for (int i = 0; i <= HEIGHT; ++i) {
        for (int j = 0; j <= HEIGHT - i; ++j) {
            printf(" ");
        }

        for (int j = 0; j <= i; ++j) {
            if (j == 0 || j == i)
                printf("%c", (char)(i + 65));
            else
                printf(" ");

            printf(" ");
        }

        printf("\n");
    }

    for (int i = HEIGHT - 1; i >= 0; --i) {
        for (int j = 0; j <= HEIGHT - i; ++j) {
            printf(" ");
        }

        for (int j = 0; j <= i; ++j) {
            if (j == 0 || j == i)
                printf("%c", (char)(i + 65));
            else
                printf(" ");

            printf(" ");
        }

        printf("\n");
    }

    return 0;
}
