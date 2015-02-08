#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <string>
#include <iostream>
static const int MAXRAND=100; 
static const int SIZE=10;
int input[SIZE];

int generateRandInt(int randMax)
{
    return rand()%randMax;
}
void generateInput()
{
    time_t time_;
    time(&time_);
    srand(time_);
    for (int count=0; count<SIZE; count++)
    {
        input[count]=generateRandInt(MAXRAND);
    }
}

static const char itoaArr[10]={'0','1','2','3','4','5','6','7','8','9'};
const char int2Char(int iv)
{
   if(iv<0 || iv>9)
        return (itoaArr[0]);
   return (itoaArr[iv]);
}
void genOutput(std::string &output)
{
    int marker[10][10][10];
    for (int d1=0;d1<10;d1++)
        for (int d2=1;d2<10;d2++)
            for(int d3=0;d3<10;d3++)
            {
                marker[d1][d2][d3]=0;
            }
    for(int i=0;i<SIZE;i++)
    {
        int s1=input[i]>100?input[i]/100:0;
        int s2=input[i]>10?(input[i]-s1*100)/10:0;
        int s3=(input[i]-s1*100-s2*10);
        marker[s1][s2][s3]++;
        printf("marking marker[%d][%d][%d]=%d\n", s1,s2,s3, marker[s1][s2][s3]);
    }
    bool foundFirst=false;
    /*for (int d1=1;d1<10&&!foundFirst;d1++)
        for (int d2=1;d2<10&&!foundFirst;d2++)
            for(int d3=0;d3<10;d3++)
            {
                if(marker[d1][d2][d3]>0)
                {
                    output.push_back(int2Char(d1));
                    output.push_back(int2Char(d2));
                    output.push_back(int2Char(d3));
                    marker[d1][d2][d3]--;
                    printf("Found first int: push_back marker[%d][%d][%d] to %s\n",d1,d2,d3,output.c_str()); 
                    foundFirst=true;
                    break;
                }
            }*/
    for (int d1=0;d1<10;d1++)
        for (int d2=1;d2<10;d2++)
            for(int d3=0;d3<10;d3++)
            {
                for(int lc=0;lc<marker[d1][d2][d3];lc++)
                {
                    if(d1>0)
                        output.push_back(int2Char(d1));
                    if(d1>0||(d1==0&&d2>0))
                        output.push_back(int2Char(d2));
                    //if(d3>0)
                    output.push_back(int2Char(d3));
                    marker[d1][d2][d3]--;
                    printf("[lc=%d][m=%d]push_back marker[%d][%d][%d] to %s\n",lc,marker[d1][d2][d3],d1,d2,d3,output.c_str()); 
                }
            }
}
int main(int argc, char * argv[])
{
    generateInput();
    for(int i=0;i<SIZE;++i)
    {
        std::cout<<" "<<input[i];
    }
    std::cout<<std::endl;
    std::string output;
    genOutput(output);
    std::cout<<output<<std::endl;
    
}
