#include "stdlib.h"
#include "stdio.h"
#include <map>
#include <list>

int main(int argc, char * argv[])
{
    std::map<int, int> mm;
    mm.insert(mm.end(), std::pair<int, int>(1, 1));
    mm.insert(mm.end(), std::pair<int, int>(2, 2));
    std::map<int, int>::iterator itr = mm.begin();

    while (itr != mm.end()) {
        printf("elem=%d, %d\n", itr->first, itr->second);
        itr++;
    }

    std::list<int> ll;
    std::list<int>::iterator l_itr = ll.begin();

    while (l_itr != ll.end())
        itr++;

    printf("elem of ll = %d\n", (*l_itr));

    return 0;
}
