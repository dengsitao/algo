#include <stdio.h>
#include <stdlib.h>
#include "social_movies.h"
/*
Assumptions: 1. movies are identical to each other.
                    2. customer id are identical
                    2. function calls are thread safe.(it is ensured by caller)
                    3.
*/

MovieInfo::MovieInfo(int id, std::string title)
{
    m_Name = title;
    m_Id = id;
}

MovieInfo::~MovieInfo()
{
}

bool MovieInfo::operator==(const MovieInfo& mi2)
{
    return (this->m_Id == mi2.m_Id);
}
bool MovieInfo::operator>(const MovieInfo& mi2)
{
    return (this->m_Id > mi2.m_Id);
}
bool MovieInfo::operator<(const MovieInfo& mi2)
{
    return (this->m_Id < mi2.m_Id);
}

CustomerInfo::CustomerInfo(int id, std::string name)
{
    m_Name = name;
    m_Id = id;
}

CustomerInfo::~CustomerInfo()
{
}

bool CustomerInfo::operator==(const CustomerInfo& mi2)
{
    return (this->m_Id == mi2.m_Id);
}
bool CustomerInfo::operator>(const CustomerInfo& mi2)
{
    return (this->m_Id > mi2.m_Id);
}
bool CustomerInfo::operator<(const CustomerInfo& mi2)
{
    return (this->m_Id < mi2.m_Id);
}

template<class TypeData, class TypeContainer>
ResultCode addtoContainer(TypeData data, TypeContainer & container)
{
    //printf("addtoContainer: add id=%d name=%s to container\n", data->m_Id, data->m_Name.c_str());
    typename TypeContainer::iterator itr = container.begin();

    while (itr != container.end() && (*itr) < (data)) {
        itr++;
    }

    if (itr != container.end() && (*itr) == (data)) {
        printf("addtoContainer failed end [%d]or has same[%d]\n", itr != container.end(), (*itr) == (data));
        return FAILURE;
    }

    container.insert(itr, data);
    return SUCCESS;
}

ResultCode CustomerInfo::addLikedMovie(MovieInfo * mInfo)
{
    return addtoContainer(mInfo, m_likedMovies);
}
ResultCode CustomerInfo::addFriend(CustomerInfo * cInfo)
{
    return addtoContainer(cInfo, m_friends);
}



SocialMovies::SocialMovies()
{

}

SocialMovies::~SocialMovies()
{

}


ResultCode SocialMovies::addMovie(int movieId, const std::string& title)
{
    return addtoContainer(new MovieInfo(movieId, title), m_movieList);
}

template<class TypeData, class TypeContainer, class CompFunc>
typename TypeContainer::iterator findInContainer(TypeData data, TypeContainer & container, CompFunc func)
{
    typename TypeContainer::iterator itr = container.begin();

    while (itr != container.end() && !(*func)(data, (*itr))) {
        itr++;
    }

    return itr;
}

template<class Type>
bool isIdSame(int id, Type data)
{
    return (id == data->m_Id);
}

// Gets the title of the given movie
ResultCode SocialMovies::lookupMovie(int movieId, std::string& movieTitle)
{
    std::list<MovieInfo*>::iterator itr = findInContainer(movieId, m_movieList, isIdSame<MovieInfo*>);

    if (itr == m_movieList.end()) {
        return FAILURE;
    } else {
        movieTitle = (*itr)->m_Name;
        return SUCCESS;
    }
}

// Defines a customer ID to name mapping in the system
ResultCode SocialMovies::addCustomer(int customerId, const std::string& name)
{
    return addtoContainer(new CustomerInfo(customerId, name), m_customerList);
}

// Gets the name of the given customer
ResultCode SocialMovies::lookupCustomer(int customerId, std::string& customerName)
{
    std::list<CustomerInfo*>::iterator itr = findInContainer(customerId, m_customerList, isIdSame<CustomerInfo*>);

    if (itr == m_customerList.end()) {
        return FAILURE;
    } else {
        customerName = (*itr)->m_Name;
        return SUCCESS;
    }

}

// Record that a movie was "liked" by the given customer
ResultCode SocialMovies::addLikedMovie(int customerId, int movieId)
{
    std::list<MovieInfo*>::iterator movie_itr = findInContainer(movieId, m_movieList, isIdSame<MovieInfo*>);
    std::list<CustomerInfo*>::iterator customer_itr = findInContainer(customerId, m_customerList, isIdSame<CustomerInfo*>);


    if (movie_itr == m_movieList.end() || customer_itr == m_customerList.end()) {
        return FAILURE;
    } else {
        return (*customer_itr)->addLikedMovie((*movie_itr));
    }
}

// Associate two customers as being friends
ResultCode SocialMovies::addFriend(int customerId1, int customerId2)
{
    std::list<CustomerInfo*>::iterator itr1 = findInContainer(customerId1, m_customerList, isIdSame<CustomerInfo*>);
    std::list<CustomerInfo*>::iterator itr2 = findInContainer(customerId2, m_customerList, isIdSame<CustomerInfo*>);

    if (itr1 == m_customerList.end() || itr2 == m_customerList.end()) {
        return FAILURE;
    } else {
        //printf("addFriend, %s and %s now friends\n", (*itr1)->m_Name.c_str(), (*itr2)->m_Name.c_str());
        (*itr1)->addFriend((*itr2));
        (*itr2)->addFriend((*itr1));
        return SUCCESS;
    }
}

int compareTwoCustomer(CustomerInfo & ct1, CustomerInfo & ct2)
{
    int count = 0;
    printf("[compareTwoCustomer][%s][%s]\n", ct1.m_Name.c_str(), ct2.m_Name.c_str());

    if (ct1.m_Id == ct2.m_Id) {
        return 0;
    }

    for (std::list<MovieInfo*>::iterator itr1 = ct1.m_likedMovies.begin(); itr1 != ct1.m_likedMovies.end(); itr1++) {
        for (std::list<MovieInfo*>::iterator itr2 = ct2.m_likedMovies.begin(); itr2 != ct2.m_likedMovies.end(); itr2++) {
            if ((*itr1)->m_Id == (*itr2)->m_Id) {
                count++;
                break;
            }
        }
    }

    return count;
}
// Gets the ID of the customer who has the most "likes" in common with the given
// customer.  The returned customer must be reachable by at most <maxDistance> friend links.
ResultCode SocialMovies::getSimilarCustomerInNetwork(int customerId, int maxDistance, int& similarCustomerId)
{
    std::list<CustomerInfo*>::iterator customer_itr = findInContainer(customerId, m_customerList, isIdSame<CustomerInfo*>);

    if (customer_itr == m_customerList.end()) {
        printf("[getSimilarCustomerInNetwork]: no customer found\n");
        return FAILURE;
    }


    int maxSameMovieCount = 0;
    CustomerInfo *cus = (*customer_itr);
    std::list<CustomerInfo*>::iterator itr = customer_itr;

    for (int i = 0; i < maxDistance; i++) {
        if (itr != m_customerList.end()) {
            int sameMovieCount = compareTwoCustomer(*cus, *(*itr));

            if (sameMovieCount > maxSameMovieCount) {
                printf("[getSimilarCustomerInNetwork][backward]:[%d] cus[%s] and cus[%s] has similar %d is more than current %d\n", i, cus->m_Name.c_str(), (*itr)->m_Name.c_str(), sameMovieCount, maxSameMovieCount);
                similarCustomerId = (*itr)->m_Id;
                maxSameMovieCount = sameMovieCount;
            }

            itr++;
        }
    }

    itr = customer_itr;

    for (int i = 0; i < maxDistance; i++) {
        if (itr != m_customerList.begin()) {
            int sameMovieCount = compareTwoCustomer(*cus, *(*itr));

            if (sameMovieCount > maxSameMovieCount) {
                printf("[getSimilarCustomerInNetwork][forward]: [%d] cus[%s] and cus[%s] has similar %d is more than current %d\n", i, cus->m_Name.c_str(), (*itr)->m_Name.c_str(), sameMovieCount, maxSameMovieCount);
                similarCustomerId = (*itr)->m_Id;
                maxSameMovieCount = sameMovieCount;
            }

            itr--;
        }
    }

    return SUCCESS;
}

bool isThisMovieLiked(CustomerInfo & cus, int movieId)
{
    for (std::list<MovieInfo*>::iterator itr = cus.m_likedMovies.begin(); itr != cus.m_likedMovies.end(); itr++) {
        if (movieId == (*itr)->m_Id) {
            return true;
        }
    }
}
// Gets the IDs of the <maximumResults> movies with the highest total number of "likes".  All movies
// returned must have a "like" count >= the like count of every non-returned movie.
ResultCode SocialMovies::getMostLikedMovies(int maximumResults, std::vector<int>& mostLikedMovieIds)
{
    int maxLikedMovieId = -1;
    int maxLikedMovieCount = 0;

    for (std::list<MovieInfo*>::iterator mitr = m_movieList.begin(); mitr != m_movieList.end(); mitr++) {
        int likedCount = 0;

        for (std::list<CustomerInfo*>::iterator citr = m_customerList.begin(); citr != m_customerList.end(); citr++) {
            likedCount += isThisMovieLiked(*(*citr), (*mitr)->m_Id);
        }

        if (likedCount > maximumResults) {
            mostLikedMovieIds.push_back((*mitr)->m_Id);
        }
    }

    return SUCCESS;
}


int main(int argc, char * argv[])
{
    SocialMovies sm;
    char name[128];

    for (int i = 0; i < 10; i++) {
        sprintf(name, "customer-%d", i);
        sm.addCustomer(i, name);
    }

    for (int i = 0; i < 10; i++) {
        sprintf(name, "movie-%d", i);
        sm.addMovie(i, name);
    }

    printf("==========begin add friend=====\n");
    sm.addFriend(1, 2);
    sm.addFriend(1, 3);
    sm.addFriend(2, 4);
    printf("==========end add friend=====\n");
    sm.addLikedMovie(1, 5);
    sm.addLikedMovie(2, 5);
    sm.addLikedMovie(3, 5);
    sm.addLikedMovie(4, 5);
    sm.addLikedMovie(1, 4);
    sm.addLikedMovie(2, 3);
    sm.addLikedMovie(3, 4);
    int similarCusId = -1;
    sm.getSimilarCustomerInNetwork(1, 10, similarCusId);
    printf("similarCusId=%d\n", similarCusId);

    std::vector<int> mostLikedMovieIds;
    sm.getMostLikedMovies(3, mostLikedMovieIds);

    for (int i = 0; i < mostLikedMovieIds.size(); i++) {
        printf("most liked movies are: %d \n", mostLikedMovieIds[i]);
    }

    return 0;
}

