#include <stdlib.h>
#include <stdio.h>
#include "unistd.h"
#include <pthread.h>
#include <unordered_map>

class Job
{
public:
	int mId;
	int mPriority;
	int mATime;
	int mBTime;
	Job * next;
	Job * prev;

	void pFunc() {
		printf("Job[%p][%d] p[%d] a[%d] b[%d]\n",
			this, mId, mPriority, mATime, mBTime);
		mBTime--;
		sleep(1);
	}
	Job(int id, int priority, int atime, int btime)
		:mId(id), mPriority(priority), mATime(atime), mBTime(btime), prev(NULL), next(NULL){}
	~Job() {
		//printf("desctructor: %d\n", mId);
	}
};

Job * pJobHead = NULL;
Job * pJobTail =pJobHead;
std::unordered_map<int, Job*> jobMap;
pthread_mutex_t mLock;

void addJob(Job * pjob)
{
	if (pjob==NULL) {
		printf("[ERROR] trying to add NULL job\n");
		return;
	}
	pthread_mutex_lock(&mLock);
	if (pJobHead == NULL) {
		pJobHead=pjob;
		pJobTail=pJobHead;
		jobMap.insert(std::pair<int, Job*>(pjob->mId, pjob));
		pthread_mutex_unlock(&mLock);
		return;
	}
	//find the tail of the jobs
	jobMap.insert(std::pair<int, Job*>(pjob->mId, pjob));
	pjob->prev=pJobTail;
	pJobTail->next=pjob;
	pJobTail=pJobTail->next;
	pthread_mutex_unlock(&mLock);
}

void removeJob(Job * pjob)
{
	if (pjob==NULL) return;
	pthread_mutex_lock(&mLock);
	if (pJobHead == NULL) {
		printf("[ERROR]: no job in job list\n");
		pthread_mutex_unlock(&mLock);
		return;
	}
	if (pJobHead == pjob) {
		//printf("removing [%d] next is %d\n", pJobHead->next->mId, pJobHead->next==NULL?-1:pJobHead->next->mId);
		if (pJobHead == pJobTail) {
			pJobTail = NULL;
			pJobHead=NULL;
		}
		else {
			pJobHead->next->prev=NULL;
			pJobHead = pJobHead->next;
		}
		jobMap.erase(pjob->mId);
		delete pjob;
		pjob=NULL;
		pthread_mutex_unlock(&mLock);
		return;
	}
	//look for the job in the list
	///Job * pItr = pJobHead;
	///while(pItr->next!=pjob) {
		//pItr=pItr->next;
	//}
	//find using map
	Job* pItr = jobMap.find(pjob->mId)->second;
	//found it.
	if (pItr!=NULL) {
		//printf("removing [%d] next is %d prev=%d\n", pItr==NULL?-1:pItr->mId, pItr->next==NULL?-1:pItr->next->mId, pItr->prev==NULL?-1:pItr->prev->mId);
		if (pItr == pJobTail) {
			pJobTail = pItr->prev;
		}
		else {
			pItr->prev->next=pItr->next;
			pItr->next->prev = pItr->prev;
		}
		jobMap.erase(pjob->mId);
		delete pjob;
		pjob=NULL;
	}
	else {
		printf("[ERROR]: couldn't find the job[%p][%d][%d][%d][%d] in joblist, can't remove\n"
				, pjob, pjob->mId, pjob->mPriority, pjob->mATime, pjob->mBTime);
	}
	pthread_mutex_unlock(&mLock);
}

void *RR(void *)
{
	while(true)
	{
		//loop every job in list
		Job * pItr = pJobHead;
		while (pItr!=NULL) {
			//run it for 1 slice
			pItr->pFunc();
			//if it is done, remove it
			if (pItr->mBTime <= 0) {
				Job *pDel = pItr;
				pItr=pItr->next;
				removeJob(pDel);
			}
			else {
				//printf("in loop pItr[%d] pItr->next=%p [%d]\n", pItr->mId, pItr->next, pItr->next==NULL?-1:pItr->next->mId);
				if (pItr->next == NULL) {
					pItr = pJobHead;
				}
				else {
					pItr=pItr->next;
				}
			}
			//go back
		}
		printf("All Job Done\n");
		return NULL;
	}
}

void *createJobs(void *)
{
	printf("creating job1\n");
	Job * job1 = new Job(/*id*/ 1, /*prioriy*/ 1, /*atime*/ 0, /*btime*/ 5);
	addJob(job1);
	sleep(1);
	printf("creating job2\n");
	Job * job2 = new Job(/*id*/ 2, /*prioriy*/ 1, /*atime*/ 1, /*btime*/ 3);
	addJob(job2);
	sleep(2);
	printf("creating job3\n");
	Job * job3 = new Job(/*id*/ 3, /*prioriy*/ 1, /*atime*/ 3, /*btime*/ 1);
	addJob(job3);
	sleep(1);
	printf("creating job4\n");
	Job * job4 = new Job(/*id*/ 4, /*prioriy*/ 1, /*atime*/ 4, /*btime*/ 2);
	addJob(job4);
	return NULL;
}


int main(int argc, char * argv[])
{
	pthread_t pid1, pid2;

	pthread_mutex_init(&mLock, NULL);
	pthread_create(&pid1, NULL, createJobs, NULL);
	usleep(500);
	pthread_create(&pid2, NULL, RR, NULL);

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	pthread_mutex_destroy(&mLock);
	return 0;
}
