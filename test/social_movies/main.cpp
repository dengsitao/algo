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
    m_Title = title;
    m_Id = id;
}

MovieInfo::~MovieInfo()
{
}

CustomerInfo::CustomerInfo(int id, std::string name)
{
    m_Name = name;
    m_Id = id;
}

CustomerInfo::~CustomerInfo()
{
}

ResultCode CustomerInfo::addLikedMovie(int movieId)
{
    //m_likedMovieVec.insert(movieId);
    return SUCCESS;
}
ResultCode CustomerInfo::addFriend(int customerId)
{
    //m_friendVec.insert(customerId);
    return SUCCESS;
}



SocialMovies::SocialMovies()
{

}

SocialMovies::~SocialMovies()
{

}

ResultCode SocialMovies::addMovie(int movieId, const std::string& title)
{
    if (m_movieList.find(movieId) != m_movieList.end()) {
        return FAILURE;
    }

    m_movieList.insert(std::pair<int, MovieInfo>(movieId, MovieInfo(movieId, title)));
    return SUCCESS;
}

// Gets the title of the given movie
ResultCode SocialMovies::lookupMovie(int movieId, std::string& movieTitle)
{
    std::map<int, MovieInfo>::iterator itr = m_movieList.find(movieId);

    if (itr == m_movieList.end()) {
        return FAILURE;
    } else {
        movieTitle = itr->second.m_Title;
        return SUCCESS;
    }
}

// Defines a customer ID to name mapping in the system
ResultCode SocialMovies::addCustomer(int customerId, const std::string& name)
{
    if (m_customerList.find(customerId) != m_customerList.end()) {
        return FAILURE;
    }

    m_customerList.insert(std::pair<int, CustomerInfo>(customerId, CustomerInfo(customerId, name)));
    return SUCCESS;
}

// Gets the name of the given customer
ResultCode SocialMovies::lookupCustomer(int customerId, std::string& customerName)
{
    std::map<int, CustomerInfo>::iterator itr = m_customerList.find(customerId);

    if (itr == m_customerList.end()) {
        return FAILURE;
    } else {
        customerName = itr->second.m_Name;
        return SUCCESS;
    }

}

// Record that a movie was "liked" by the given customer
ResultCode SocialMovies::addLikedMovie(int customerId, int movieId)
{
    std::map<int, MovieInfo>::iterator movie_itr = m_movieList.find(movieId);
    std::map<int, CustomerInfo>::iterator customer_itr = m_customerList.find(customerId);

    if (movie_itr == m_movieList.end() || customer_itr == m_customerList.end()) {
        return FAILURE;
    } else {
        customer_itr->second.addLikedMovie(movieId);
        return SUCCESS;
    }
}

// Associate two customers as being friends
ResultCode SocialMovies::addFriend(int customerId1, int customerId2)
{
    std::map<int, CustomerInfo>::iterator itr1 = m_customerList.find(customerId1);
    std::map<int, CustomerInfo>::iterator itr2 = m_customerList.find(customerId2);

    if (itr1 == m_customerList.end() || itr2 == m_customerList.end()) {
        return FAILURE;
    } else {
        itr1->second.addFriend(customerId2);
        return SUCCESS;
    }
}

int compareTwoCustomer(CustomerInfo & ct1, CustomerInfo & ct2)
{
    int count = 0;

    for (int i = 0; i < ct1.m_likedMovieVec.size(); i++) {
        for (int j = 0; j < ct2.m_likedMovieVec.size(); j++) {
            if (ct1.m_likedMovieVec[i] == ct2.m_likedMovieVec[j]) {
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
    std::map<int, CustomerInfo>::iterator customer_itr = m_customerList.find(customerId);

    if (customer_itr == m_customerList.end()) {
        return FAILURE;
    }


    int maxSameMovieCount = 0;
    CustomerInfo cus = customer_itr->second;

    for (int i = 0; i < maxDistance; i++) {
        std::map<int, CustomerInfo>::iterator itr = customer_itr;

        if (++itr != m_customerList.end()) {
            int sameMovieCount = compareTwoCustomer(cus, itr->second);

            if (sameMovieCount > maxSameMovieCount) {
                similarCustomerId = itr->second.m_Id;
            }
        }
    }

    for (int i = 0; i < maxDistance; i++) {
        std::map<int, CustomerInfo>::iterator itr = customer_itr;

        if (--itr != m_customerList.begin()) {
            int sameMovieCount = compareTwoCustomer(cus, itr->second);

            if (sameMovieCount > maxSameMovieCount) {
                similarCustomerId = itr->second.m_Id;
            }
        }
    }

    return SUCCESS;
}

bool isThisMovieLiked(CustomerInfo & cus, int movieId)
{
    for (int i = 0; i < cus.m_likedMovieVec.size(); i++) {
        if (movieId == cus.m_likedMovieVec[i]) {
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

    for (std::map<int, MovieInfo>::iterator mitr = m_movieList.begin(); mitr != m_movieList.end(); mitr++) {
        int likedCount = 0;

        for (std::map<int, CustomerInfo>::iterator citr = m_customerList.begin(); citr != m_customerList.end(); citr++) {
            likedCount += isThisMovieLiked(citr->second, mitr->second.m_Id);
        }

        if (likedCount > maximumResults) {
            mostLikedMovieIds.push_back(mitr->second.m_Id);
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

    sm.addFriend(1, 2);
    sm.addFriend(1, 3);
    sm.addFriend(2, 4);
    sm.addLikedMovie(1, 5);
    sm.addLikedMovie(2, 5);
    sm.addLikedMovie(3, 5);
    sm.addLikedMovie(4, 5);
    sm.addLikedMovie(1, 4);
    sm.addLikedMovie(2, 3);
    sm.addLikedMovie(3, 7);
    int similarCusId = 0;
    sm.getSimilarCustomerInNetwork(1, 10, similarCusId);
    std::vector<int> mostLikedMovieIds;

    sm.getMostLikedMovies(3, mostLikedMovieIds);
    return 0;
}

