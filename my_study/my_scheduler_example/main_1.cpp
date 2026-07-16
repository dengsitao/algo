#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include <ostream>
#include <iostream>
#include <pthread.h>

bool running = true;

class Job
{
public:
	int mJobId;
	int mAtime;
	int mBtime;
	//void *(pFunc)(int atime, int btime);
	void pFunc(int rTime) { 
		while(mBtime>0 && rTime>0) {
			printf("id[%d] atime=%d btime=%d\n",mJobId, mAtime, mBtime);
			sleep(1);
			mBtime--;
			rTime--;
		}
	}
	Job * next;

	//Job(int id, int atime, int btime, void *(func)(int, int))
		//: mJobId(id), mAtime(atime), mBtime(btime), pFunc(func) {}
	Job(int id, int atime, int btime)
		: mJobId(id), mAtime(atime), mBtime(btime), next(NULL) {}
};

Job * jobHead=NULL;
Job * jobTail=jobHead;

pthread_mutex_t mLock;
Job * findLBJob()
{
	pthread_mutex_lock(&mLock);	
	Job * pHead=jobHead;
	Job * pSelected=pHead;
	while(pHead!=NULL) {
		if (pSelected->mBtime > pHead->mBtime) {
			pSelected = pHead;
		}
		pHead = pHead->next;
	}
	pthread_mutex_unlock(&mLock);	
	return pSelected;

}

void insertJob(Job * pJob)
{
	if (NULL == pJob) {
		std::cout<<"ERROR: cannot insert null job"<<std::endl;
		return;
	}
	
	pthread_mutex_lock(&mLock);	
	if (NULL == jobHead) {
		jobHead = pJob;
		jobTail = jobHead;
		printf("insert first pJob[%p] to list, pHead=%p, pTail=%p\n", pJob, jobHead, jobTail);
	}
	else {
		jobTail->next=pJob;
		jobTail=pJob;
		printf("insert pJob[%p] to list, pHead=%p, pTail=%p\n", pJob, jobHead, jobTail);
	}
	pthread_mutex_unlock(&mLock);	

}

void removeJob(Job * pJob)
{
	pthread_mutex_lock(&mLock);	
	if (NULL == pJob || jobHead == NULL) {
		std::cout<<"ERROR: cannot remove job"<<pJob<<" from "<<jobHead<<std::endl;
		pthread_mutex_unlock(&mLock);
		return;
	}
	printf("remove pJob[%p] from list, pHead=%p, pTail=%p\n", pJob, jobHead, jobTail);
	if (jobHead == pJob && jobTail!=pJob) {//the first job in list is done
		//printf("assigning jobHead[%p] to pJob[%p]\n", jobHead, pJob);
		jobHead = pJob->next;
		delete pJob;
	}
	else if (jobHead == pJob && jobTail == pJob) {/*the only one job in list*/
		jobHead=NULL;
		jobTail=NULL;
		delete pJob;
	}
	else {
		Job * pHead=jobHead;
		while(pHead!=NULL && pHead->next!=NULL) {
			if (pJob==pHead->next) {
				pHead->next = pHead->next->next;
				if (jobTail == pJob) {
					jobTail = pHead;
				}
				delete pJob;
				pthread_mutex_unlock(&mLock);	
				return;
			}
			pHead=pHead->next;
		}
	}
	pthread_mutex_unlock(&mLock);	

}

void LBF()//least Burst Time
{
	while(running) {
		//go through all tasks and find a task with least burst time.
		Job *mJob = findLBJob();

		//run it.
		if (mJob!=NULL) {
			mJob->pFunc(mJob->mBtime);
		}
		else {
			std::cout<<"no available job"<<std::endl;
			return;
		}
		//after finish, remove it from task list, go back searching.
	
		removeJob(mJob);
	}
}

void RR()
{
	Job *mJob = jobHead;
	while(mJob!=NULL) {
		if (mJob!=NULL) {
			mJob->pFunc(1);
		}
		Job * pCur=mJob;
		if (mJob->next!=NULL) {
			mJob=mJob->next;
		}
		else if (jobHead!=NULL) {
			mJob=jobHead;
		}
		else {
			mJob=NULL;
		}
		if (pCur->mBtime==0) {
			removeJob(pCur);
		}

	}

}

void *createJobs(void *)
{
	Job *job1=new Job(1, 0, 5);
	Job *job2=new Job(2, 3, 6);
	Job *job3=new Job(3, 4, 2);
	Job *job4=new Job(4, 6, 3);
	printf("start insert Job\n");
	insertJob(job1);
	sleep(3);
	insertJob(job2);
	sleep(1);
	insertJob(job3);
	sleep(2);
	insertJob(job4);
	return 0;
}

void *runJobs(void*)
{
	//LBF();
	RR();
	return 0;
}

int main(int argc, char * argv[])
{
	pthread_t pid1, pid2;
	pthread_mutex_init(&mLock, NULL);

	pthread_create(&pid1, NULL, createJobs, NULL);
	usleep(300);
	pthread_create(&pid2, NULL, runJobs, NULL);

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	pthread_mutex_destroy(&mLock);


	return 0;
}
