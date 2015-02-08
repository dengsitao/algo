#ifndef SOCIAL_MOVIES_H
#define SOCIAL_MOVIES_H

#include <string>
#include <vector>
#include <list>
#include <deque>

// Enumeration of possible return values
enum ResultCode {

    // The function was successful 
    SUCCESS = 0, 

    // An error condition was encountered when executing the function
    FAILURE,
};

class MovieInfo
{
public:
	MovieInfo(int id, std::string title);
	virtual ~MovieInfo();
	bool operator==(const MovieInfo& mi2);
	bool operator>(const MovieInfo& mi2);
	bool operator<(const MovieInfo& mi2);
public:
	std::string m_Name;
	int m_Id;
};

class CustomerInfo
{
public:
	CustomerInfo(int id, std::string name);
	virtual ~CustomerInfo();

	ResultCode addLikedMovie(MovieInfo *mInfo);
	ResultCode addFriend(CustomerInfo *cInfo);
	bool operator==(const CustomerInfo& mi2);
	bool operator>(const CustomerInfo& mi2);
	bool operator<(const CustomerInfo& mi2);
	
public:
	std::string m_Name;
	int m_Id;
	std::deque<MovieInfo *> m_likedMovies;
	std::deque<CustomerInfo *> m_friends;
};


// Maintains a network of movies and customers
// All methods should return ResultCode::SUCCESS when the operation completes successfully or
// ResultCode::FAILURE when an error condition occurs.
class SocialMovies 
{
public:

    // Defines a movie ID to title mapping in the system
    ResultCode addMovie(int movieId, const std::string& title);

    // Gets the title of the given movie
    ResultCode lookupMovie(int movieId, std::string& movieTitle);

    // Defines a customer ID to name mapping in the system
    ResultCode addCustomer(int customerId, const std::string& name);

    // Gets the name of the given customer
    ResultCode lookupCustomer(int customerId, std::string& customerName);

    // Record that a movie was "liked" by the given customer
    ResultCode addLikedMovie(int customerId, int movieId);

    // Associate two customers as being friends
    ResultCode addFriend(int customerId1, int customerId2);

    // Gets the ID of the customer who has the most "likes" in common with the given
    // customer.  The returned customer must be reachable by at most <maxDistance> friend links.
    ResultCode getSimilarCustomerInNetwork(int customerId, int maxDistance, int& similarCustomerId);

    // Gets the IDs of the <maximumResults> movies with the highest total number of "likes".  All movies
    // returned must have a "like" count >= the like count of every non-returned movie.
    ResultCode getMostLikedMovies(int maximumResults, std::vector<int>& mostLikedMovieIds);

	SocialMovies();
	~SocialMovies();
private:
	std::deque<CustomerInfo *> m_customerList;
	std::deque<MovieInfo *> m_movieList;
};

#endif//SOCIAL_MOVIES_H

