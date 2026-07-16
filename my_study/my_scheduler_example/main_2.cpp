#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include <pthread.h>

int i=0;
class Job 
{
public:
	int mId;
	int mPriority;
	int mAtime;
	int mBtime;
	Job * next;
	void pFunc() {
		while (mBtime>0) {
			printf("Job[%d] [%d][%d] bTime=%d\n", mId, mPriority, mAtime, mBtime);
			sleep(1);
			mBtime--;
		}
	}
	Job(int id, int priority, int atime, int btime) : mId(id), mPriority(priority), mAtime(atime), mBtime(btime), next(NULL) {}
};

Job * pJobHead = NULL;
pthread_mutex_t mLock;

Job * findShortestJob()
{
	pthread_mutex_lock(&mLock);
	if (pJobHead==NULL){
		pthread_mutex_unlock(&mLock);
	       	return NULL;
	}

	Job * pJobItr = pJobHead;
	Job * pShortest=pJobHead;
	while(pJobItr!=NULL) {
		if (pJobItr->mBtime < pShortest->mBtime) {
			pShortest = pJobItr;
		}
		pJobItr=pJobItr->next;
	}
	pthread_mutex_unlock(&mLock);
	return pShortest;
}

void removeJob(Job * pjob)
{
	pthread_mutex_lock(&mLock);
	if (pJobHead == NULL) {
		pthread_mutex_unlock(&mLock);
		printf("[ERROR]:pJobHead is NULL\n");
		return;
	}
	if (pjob == NULL) {
		pthread_mutex_unlock(&mLock);
		printf("[Warning]: trying to remove a null job\n");
		return;
	}
	printf("removing Job[%d][%d][%d][%d]\n", pjob->mId, pjob->mPriority, pjob->mAtime, pjob->mBtime);
	if (pJobHead == pjob){// remove the head one
		pJobHead=pJobHead->next;
		delete pjob;
		pjob=NULL;
		pthread_mutex_unlock(&mLock);
		return;
	}
	Job * pJobItr = pJobHead;
	while (pJobItr->next != NULL) {
		//find it
		if (pJobItr->next == pjob) {
			pJobItr->next = pJobItr->next->next;
			delete pjob;
			pjob=NULL;
			pthread_mutex_unlock(&mLock);
			return;
		}
		pJobItr=pJobItr->next;
	}
	pthread_mutex_unlock(&mLock);
	printf("[ERROR]: can't remove a job[%p][%d][%d][%d] not in list\n", pjob, pjob->mId, pjob->mAtime,pjob->mBtime);
}

void addJob(Job *pjob)
{
	printf("Adding Job[%d][%d][%d][%d]\n", pjob->mId, pjob->mPriority, pjob->mAtime, pjob->mBtime);
	pthread_mutex_lock(&mLock);
	if (pJobHead==NULL) {
		pJobHead=pjob;
		pthread_mutex_unlock(&mLock);
		return;
	}
	else {
		Job * pJobItr = pJobHead;
		while(pJobItr->next!=NULL) {
			pJobItr=pJobItr->next;
		}
		pJobItr->next = pjob;
	}
	pthread_mutex_unlock(&mLock);
}

void *SJF(void*)
{
	while(true) {
		//find shortest btime job
		Job * pCurJob = findShortestJob();
		if (NULL == pCurJob) {
			printf("no job available\n");
			return NULL;
		}
		//run it
		pCurJob->pFunc();
		//remove it
		removeJob(pCurJob);
	}
}

void *createOneJob(void* arg) ;

void *createJobs(void*)
{
	printf("i=%d\n",i);
	Job * job1 = new Job(/*id*/1,/*priority*/1,/*aTime*/ 0,/*bTime*/ 5);
	pthread_t pid1;
	pthread_create(&pid1, NULL, createOneJob, (void*)job1);
	//addJob(job1);
	//sleep(1);
	Job * job2 = new Job(/*id*/2,/*priority*/1,/*aTime*/ 1,/*bTime*/ 3);
	pthread_t pid2;
	pthread_create(&pid2, NULL, createOneJob, (void*)job2);
	//addJob(job2);
	//sleep(2);
	Job * job3 = new Job(/*id*/3,/*priority*/1,/*aTime*/ 3,/*bTime*/ 2);
	pthread_t pid3;
	pthread_create(&pid3, NULL, createOneJob, (void*)job3);
	//addJob(job3);
	//sleep(3);
	Job * job4 = new Job(/*id*/4,/*priority*/1,/*aTime*/ 6,/*bTime*/ 6);
	pthread_t pid4;
	pthread_create(&pid4, NULL, createOneJob, (void*)job4);
	//addJob(job4);
	
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	pthread_join(pid3, NULL);
	pthread_join(pid4, NULL);
	return NULL;
}

void *createOneJob(void* arg) 
{
	Job *mjob = static_cast<Job*>(arg);
	if (mjob == NULL) {
		printf("[ERROR]: invalid argument\n");
		return NULL;
	}
	sleep(mjob->mAtime);
	addJob(mjob);
	return NULL;
}

int main(int argc, char * argv[]) 
{
	pthread_t pid1, pid2;
	pthread_mutex_init(&mLock, NULL);
	
	pthread_create(&pid1, NULL, createJobs, NULL);
	usleep(500);
	pthread_create(&pid2, NULL, SJF, NULL);

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	pthread_mutex_destroy(&mLock);

	
	return 0;
}
