#include "stdlib.h"
#include "stdio.h"
#include <list>
#include <algorithm>

template<class TypeData, class TypeContainer, class compFunc, class delFunc>
typename TypeContainer::iterator deleteOneFromContainer(TypeData key, TypeContainer & container, compFunc isSame, delFunc del)
{
    //printf("addtoContainer: add id=%d name=%s to container\n", data->m_Id, data->m_Name.c_str());
    typename TypeContainer::iterator itr = container.begin();

    while (itr != container.end() && !(*isSame)((*itr), key)) {
        itr++;
    }

    if (itr == container.end()) {
        printf("deleteFromContainer failed\n");
        return container.end();
    }

    (*del)(itr);
    return container.erase(itr);
}

template<class TypeContainer, class delFunc>
typename TypeContainer::iterator deleteAllFromContainer(TypeContainer & container, delFunc del)
{
    //printf("addtoContainer: add id=%d name=%s to container\n", data->m_Id, data->m_Name.c_str());
    typename TypeContainer::iterator itr = container.begin();

    while (itr != container.end()) {
        (*del)(*itr);
        itr = container.erase(itr);
    }
}

class A
{
public:
    A() {
        printf("Construct A() id=%d %p\n", m_id, this);
    };
    A(int id) {
        m_id = id;
        printf("Construct A(int) id=%d %p\n", m_id, this);
    };
    virtual ~A() {
        printf("Destruct A id=%d %p\n", m_id, this);
    };
    int m_id;
    bool operator==(const A& a) {
        return (this->m_id == a.m_id);
    };
};
class Aderived: public A
{
public:
    Aderived() {
        printf("Construct Aderived() id=%d %p\n", m_id, this);
    };
    ~Aderived() {
        printf("destructor of Aderived~\n");
    };
};
void printA(const A* a)
{
    printf("a.m_id=%d\n", a->m_id);
}
void deleteA(A * a)
{
    if (a != NULL)
        delete a;
}
int main(int argc, char * argv[])
{
    //std::list<A*> alist;
    //alist.push_back(new A(7));
    //alist.push_back(new A(17));
    //alist.push_back(new A(27));

    //std::remove(alist.begin(), alist.end(), (A*)&a1);
    //deleteAllFromContainer(alist,deleteA);
    //std::for_each(alist.begin(), alist.end(), printA);
    printf("========\n");
    A *a = new A(1);
    delete a;
    printf("========\n");
    Aderived * ad = new Aderived();
    delete ad;
    printf("========\n");
    A * a3 = new Aderived();
    delete a3;
    printf("========\n");

    return 0;
}
