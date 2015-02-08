#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <string>
#include <stdexcept>

int main(int argc, char * argv[])
{
    std::string a;
    a.resize(100);
    //a.insert(0, "|");
    try{
    a.insert(15, "Abc");
    }catch (const std::out_of_range& oor)
    {
        printf("exception! %s\n", oor.what());
        }
    printf("a=%s\n",a.c_str());
    std::cout<<a<<std::endl;
    std::cout<<a.size()<<std::endl;
    std::cout<<a.find("A")<<std::endl;
    return 0;
}
