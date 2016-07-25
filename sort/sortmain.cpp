#include "stdlib.h"
#include "stdio.h"
#include <time.h>
#include <strings.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include "math.h"
#include "errno.h"
#include <iostream>
#include <fstream>
#include <sstream>


//static const int LOOPCOUNT = 1;//0000;
//static const int ARRAYSIZE = 16;//0000;
//static const unsigned int MAXRAND = 20;//0000;
static const int LOOPCOUNT = 10000;
static const int ARRAYSIZE = 10000;
static const unsigned int MAXRAND = 10000;
static int intArr[ARRAYSIZE];
static const char * configFile = "sort.cfg";
static unsigned long long memcpyPartCount = 0;
static unsigned long long exchangePartCount = 0;
////////////sort function claim////////////
//
void mergeSort(int * iArr, int size);
void quickSort_exchange(int * iArr, int size);
void quickSort_memcpy(int * iArr, int size);
void bucketSort(int * iArr, int size);
void countingSort(int * iArr, int size, int maxvalue);
int partition(int * iArr, int startPos, int endPos);
void printBiTree(int * btArr, int size);
int heapIndexLeft(int i);
int heapIndexRight(int i);
int heapParent(int *hArr, int i);
void maxHeapify(int * hArr, int size, int i);
void makeHeap(int * hArr, int size);
void heapSort(int *hArr, int size);
/////////////////end///////////////////////
////////////sort function wrapper////////////
//
void mergeSortIntf(int * iArr, int size, int extra)
{
    mergeSort(iArr, size);
}
void quickSort_exchangeIntf(int * iArr, int size, int extra)
{
    quickSort_exchange(iArr, size);
}
void quickSort_memcpyIntf(int * iArr, int size, int extra)
{
    quickSort_memcpy(iArr, size);
}
void bucketSortIntf(int * iArr, int size, int extra)
{
    bucketSort(iArr, size);
}
void countingSortIntf(int * iArr, int size, int maxvalue)
{
    countingSort(iArr, size, MAXRAND);
}
void heapSortIntf(int *hArr, int size, int extra)
{
    heapSort(hArr, size);
}
/////////////////end///////////////////////


typedef void (*sortFuncPointer)(int * iArr, int size, int extra);
class sortAlgo
{
public:
    sortAlgo(std::string algoName, sortFuncPointer func) {
        name = algoName;
        pFunc = func;
        isEnabled = false;
    };
    std::string name;
    sortFuncPointer pFunc;
    bool isEnabled;
};
static const char* CFGITEMS[] = {"mergesort",
                                 "quicksort",
                                 "quickSort_memcpy",
                                 "heapsort",
                                 "countingsort"
                                };
static const int MAX_SORT_ALGO_COUNT = 6;
static sortAlgo algos[MAX_SORT_ALGO_COUNT] = {sortAlgo("mergesort", mergeSortIntf),
                                              sortAlgo("quickSort_exchange", quickSort_exchangeIntf),
                                              sortAlgo("quickSort_memcpy", quickSort_memcpyIntf),
                                              sortAlgo("bucketSort", bucketSortIntf),
                                              sortAlgo("heapSort", heapSortIntf),
                                              sortAlgo("countingSort", countingSortIntf),
                                             };

////////////utility function//////////////
int readConfig();

void init()
{
    readConfig();
}

int readConfig()
{
    std::ifstream fs(configFile, std::fstream::in);
    std::string cfgItem;
    int cfgValue = 0;

    while (fs) {
        std::getline(fs, cfgItem);
        //std::cout<<cfgItem<<std::endl;
        size_t pos = cfgItem.rfind("=");

        if (pos != std::string::npos) {
            std::string cfgName = cfgItem.substr(0, pos);
            cfgValue = atoi(cfgItem.substr(pos + 1, cfgItem.length()).c_str());

            //std::cout<<cfgName.c_str()<<" value= "<<cfgValue<<std::endl;
            for (int i = 0; i < MAX_SORT_ALGO_COUNT; i++) {
                //std::cout<<algos[i].name.c_str()<<std::endl;
                if (cfgName.compare(algos[i].name) == 0) {
                    if (0 < cfgValue) {
                        //std::cout<<cfgName.c_str()<<" enabled"<<std::endl;
                        algos[i].isEnabled = true;
                    }
                }
            }
        }
    }

    fs.close();
    return 0;
}
void exchangeInts(int * i1, int * i2)
{
    int tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}
int generateRandInt(int randMax)
{
    //time_t time_;
    //time(&time_);
    //srand(time_);
    return rand() % randMax;
}

void generateInput()
{
    time_t time_;
    time(&time_);
    srand(time_);

    for (int count = 0; count < ARRAYSIZE; count++) {
        intArr[count] = generateRandInt(MAXRAND);
    }
}

bool validateSortedArr(int array[], int size)
{
    for (int count = 0; count < size - 1; count++) {
        if (array[count] > array[count + 1]) {
            printf("ERROR! array[%d]=%d > array[%d]=%d\n", count, array[count], count + 1, array[count + 1]);
        }
    }
}
void printArray(int array[], int size)
{
    for (int count = 0; count < size; count++) {
        printf(" %u ", array[count]);
    }

    printf("\n");
}

void printBiTree(int * btArr, int size)
{
    const double bi = 2;
    const int eleWidth = 8;
    double lg2 = log(bi);
    double depth = log((double)size) / lg2;
    int idepth = (int)floor(depth);
    int buttomEleCount = pow(bi, idepth);
    double fullbtEleCount = 0;

    for (int i = idepth; i >= 0; i--) {
        double eleCount = pow(bi, i);
        fullbtEleCount += eleCount;
    }

    if ((int)fullbtEleCount > size) {
        buttomEleCount = size - (int)pow(bi, idepth) + 1;
    }

    int printCount = 0;
    int fullbtButtomEleCount = pow(bi, idepth);
    int buttomlength = fullbtButtomEleCount * eleWidth;

    //printf("size=%u, depth=%f, idepth=%d, buttomEleCount=%d, fullbtEleCount=%f, buttomelength=%d\n", size, depth, idepth, buttomEleCount, fullbtEleCount, buttomlength);
    for (int i = 0; i <= idepth; i++) {
        int eleCount = (int)pow(bi, i);
        int wspLength = (buttomlength - eleCount * eleWidth) / (eleCount + 1);

        if (i == idepth) {
            eleCount = buttomEleCount;
        } else {
            wspLength = (buttomlength - eleCount * eleWidth) / (eleCount + 1);
        }

        //printf("eleCount=%d, wsplength=%d, printCount=%d\n",eleCount, wspLength, printCount);

        for (int j = 0; j < eleCount; j++) {
            for (int k = 0; k < wspLength; k++) {
                printf(" ");
            }

            printf(" [%04d] ", btArr[printCount]);
            printCount++;
        }

        printf("\n");

    }

}
inline unsigned long getCurTimeMS()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
////////////Utility End///////////////////////

////////////Main///////////////////////////
static unsigned int mmcpTime = 0;
static unsigned int exchTime = 0;
int main(int argc, char * argv[])
{
    init();

    {
        //std::cout<<algos[i].name.c_str()<<" is "<<((algos[i].isEnabled==true)?"enabled":"disabled")<<std::endl;
        //if (true==algos[i].isEnabled)
        {
            //printf("Start testing %s\n", algos[i].name.c_str());
            unsigned long startTime = getCurTimeMS();

            for (int ct = 0; ct < LOOPCOUNT; ct++) {
                bzero(intArr, sizeof(intArr));
                generateInput();
                int cpIntArr[ARRAYSIZE];
                bzero(cpIntArr, sizeof(cpIntArr));
                memcpy(cpIntArr, intArr, sizeof(intArr));
                unsigned long time1 = getCurTimeMS();
                algos[1].pFunc(cpIntArr, ARRAYSIZE, 0);//exchange
                unsigned long time2 = getCurTimeMS();
                algos[2].pFunc(cpIntArr, ARRAYSIZE, 0);//memcpy
                unsigned long time3 = getCurTimeMS();
                exchTime += (time2 - time1);
                mmcpTime += (time3 - time2);
                validateSortedArr(cpIntArr, ARRAYSIZE);
            }

            unsigned long endTime = getCurTimeMS();
            //printf("%s Time taken:%lu, average = %lu\n", algos[i].name.c_str(), endTime - startTime, (endTime-startTime)/LOOPCOUNT);
        }
    }
    printf("quicksort using memcpy, part loop=%llu, total time=%llu, exchange part loop=%llu, total time=%llu\n", memcpyPartCount, mmcpTime, exchangePartCount, exchTime);
    return 0;

    for (int i = 0; i < MAX_SORT_ALGO_COUNT; i++) {

        std::cout << algos[i].name.c_str() << " is " << ((algos[i].isEnabled == true) ? "enabled" : "disabled") << std::endl;

        if (true == algos[i].isEnabled) {
            printf("Start testing %s\n", algos[i].name.c_str());
            unsigned long startTime = getCurTimeMS();

            for (int ct = 0; ct < LOOPCOUNT; ct++) {
                bzero(intArr, sizeof(intArr));
                generateInput();
                int cpIntArr[ARRAYSIZE];
                bzero(cpIntArr, sizeof(cpIntArr));
                memcpy(cpIntArr, intArr, sizeof(intArr));
                algos[i].pFunc(cpIntArr, ARRAYSIZE, 0);
                validateSortedArr(cpIntArr, ARRAYSIZE);
            }

            unsigned long endTime = getCurTimeMS();
            printf("%s Time taken:%lu, average = %lu\n", algos[i].name.c_str(), endTime - startTime, (endTime - startTime) / LOOPCOUNT);
        }
    }

    printf("quicksort using memcpy, part loop=%llu, exchange part loop=%llu\n", memcpyPartCount, exchangePartCount);
    return 0;
}
////////////Main End///////////////////////

////////////sort function claim////////////

void mergeSort(int * iArr, int size)
{
    for (int i = 1; i < size; i++) {
        for (int j = i - 1; j >= 0; j--) {
            if (iArr[j + 1] < iArr[j]) {
                //exchange
                exchangeInts(&(iArr[j]), &(iArr[j + 1]));
            }
        }
    }
}
int PartitionMemcpy(int * iArr, int size, int &smallCount, int & equalCount, int & bigCount)
{

    int pos = size - 1;
    int ith = iArr[pos];
    int * smallhalf = new int[size];
    int * bighalf = new int[size];
    int * equalpart = new int[size];
    memset(smallhalf, 0, sizeof(smallhalf));
    memset(bighalf, 0, sizeof(bighalf));
    memset(equalpart, 0, sizeof(equalpart));

    for (int i = 0; i < size; i++) {
        memcpyPartCount++;

        if (iArr[i] < ith) {
            smallhalf[smallCount++] = iArr[i];
        } else if (iArr[i] > ith) {
            bighalf[bigCount++] = iArr[i];
        } else {
            equalpart[equalCount++] = iArr[i];
        }
    }

    memcpy(iArr, smallhalf, sizeof(int)*smallCount);
    memcpy(iArr + smallCount, equalpart, sizeof(int)*equalCount);
    memcpy(iArr + smallCount + equalCount, bighalf, sizeof(int)*bigCount);
    delete[] smallhalf;
    delete[] bighalf;
    delete[] equalpart;
}

void quickSort_memcpy(int * iArr, int size)
{
    if (2 > size) {
        return;
    }

    int smallCount = 0;
    int bigCount = 0;
    int equalCount = 0;
    PartitionMemcpy(iArr, size, smallCount, equalCount, bigCount);
    quickSort_memcpy(iArr, smallCount);
    quickSort_memcpy(iArr + smallCount + equalCount, bigCount);
}

int PartitionExchange(int * iArr, int size, int pos)
{

    //partition
    //int pos=size-1;
    int pos_index = -1;
    int ith = iArr[pos];

    for (int i = 0; i < pos; i++) {
        exchangePartCount++;

        if (iArr[i] <= ith) {
            pos_index++;
            exchangeInts(&iArr[pos_index], &iArr[i]);
            //printArray(intArr, ARRAYSIZE);
        }
    }

    exchangeInts(&iArr[pos_index + 1], &iArr[pos]);
    return pos_index;
}
void quickSort_exchange(int * iArr, int size)
{
    if (2 > size) {
        return;
    }

    //partition
    /*int pos=size-1;
    int pos_index=-1;
    int ith = iArr[pos];
    for (int i=0; i<pos; i++)
    {
        if (iArr[i] <= ith)
        {
            pos_index++;
            exchangeInts(&iArr[pos_index], &iArr[i]);
            //printArray(intArr, ARRAYSIZE);
        }
    }
    exchangeInts(&iArr[pos_index+1], &iArr[pos]);*/
    //printf("size=%d, pos_index=%d   \n", size, pos_index);
    //printArray(intArr, ARRAYSIZE);
    //
    int pos_index = PartitionExchange(iArr, size, size - 1);
    quickSort_exchange(iArr, pos_index + 1);
    quickSort_exchange(iArr + pos_index + 2, size - pos_index - 2);
    //printf("===end quick size=%d ===\n", size);

}

void countingSort(int * iArr, int size, int maxvalue)
{
    int * pcountArr = new int[maxvalue];
    bzero(pcountArr, (maxvalue)*sizeof(int));

    for (int i = 0; i < size; i++) {
        pcountArr[iArr[i]]++;
    }

    for (int i = 1; i < maxvalue; i++) {
        pcountArr[i] += pcountArr[i - 1];
    }

    int *out = new int[size];

    for (int i = size - 1; i >= 0; i--) {

        out[pcountArr[iArr[i]] - 1] = iArr[i];
        pcountArr[iArr[i]]--;
    }

    memcpy(iArr, out, size * sizeof(int));
    delete[] pcountArr;
    delete[] out;
}
int heapIndexLeft(int i)
{
    return 2 * i + 1;
}
int heapIndexRight(int i)
{
    return 2 * i + 2;
}
int heapParent(int *hArr, int i)
{
    return 0 == i ? hArr[0] : hArr[(i - 1) / 2];
}
void maxHeapify(int * hArr, int size, int i)
{
    int left = heapIndexLeft(i);
    int right = heapIndexRight(i);
    int largest = i;

    if (left < size && hArr[left] > hArr[i]) {
        largest = left;
    }

    if (right < size && hArr[right] > hArr[largest]) {
        largest = right;
    }

    if (largest != i) {
        exchangeInts(&hArr[largest], &hArr[i]);
        maxHeapify(hArr, size, largest);
    }
}
void makeHeap(int * hArr, int size)
{
    for (int i = (size) / 2; i >= 0; i--) {
        maxHeapify(hArr, size, i);
    }
}
void heapSort(int *hArr, int size)
{
    makeHeap(hArr, size);
    int isize = size;

    for (int i = size - 1; i >= 0; i--) {
        exchangeInts(&hArr[0], &hArr[i]);
        isize--;
        maxHeapify(hArr, isize, 0);
    }
}
void bucketSort(int * iArr, int size)
{
}
/////////////////end///////////////////////







