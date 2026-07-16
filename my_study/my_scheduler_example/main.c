#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "errno.h"
#include "string.h"
#include "sys/wait.h"
#include <pthread.h>

struct pcb_t
{
	unsigned int m_pid;
	unsigned int m_aTime;
	unsigned int m_bTime;
	unsigned int m_priority;
	void (*pFunc)(int pic, int runTime);
};

struct job
{
	struct pcb_t * m_pPCB;
	struct job * m_next;
};

struct job * pHead = NULL;
struct job * pTail = NULL;
static int pidBase = 1;

struct pcb_t * createPCB(int aTime, int bTime, int priority, void (*func)(int, int))
{
	struct pcb_t * p = malloc(sizeof(struct pcb_t));
	p->m_pid=pidBase++;/*not thread safe, need alloc/free mechanism*/
	p->m_aTime=aTime;
	p->m_bTime=bTime;
	p->m_priority=priority;
	p->pFunc=func;
}

void releasePCB(struct pcb_t * p)
{
	printf("release job[%d] created aTime=%d bTime=%d priority=%d \n", p->m_pid, p->m_aTime, p->m_bTime, p->m_priority);
	free(p);
	p=NULL;
}

void runJobFunc(int pid, int runTime, void (*pFunc)(int, int))
{
	for (;runTime>0;runTime--) {
		pFunc(pid, runTime);
		sleep(1);
	}

}

void jobFunc(int pid, int runTime)
{
	printf("in job %d run %d\n", pid, runTime);
}
//TODO: operations on jobs list needs lock.
void createOneJob(int aTime, int bTime, int priority, void(*func)(int, int))
{
	struct pcb_t * p = createPCB(aTime, bTime, priority, func);
	printf("new job[%d] created aTime=%d bTime=%d priority=%d \n", p->m_pid, aTime, bTime, priority);
	if (pHead==NULL){
		pHead = malloc(sizeof(struct job));
		pHead->m_pPCB=p;
		pHead->m_next=NULL;
		pTail=pHead;
		printf("first one, add to head %d\n", p->m_pid);
	}
	else {
		struct job * pNewJob = malloc(sizeof(struct job));
		pNewJob->m_pPCB = p;
		pTail->m_next=pNewJob;
		pTail=pTail->m_next;
		printf("added to tail %d\n", p->m_pid);
	}

}

void releaseOneJob(struct pcb_t * pPCB)
{
	if (pPCB==NULL || pHead==NULL) {
		printf("%s invalid args pPCB=%p, pHead=%p\n", __func__,pPCB, pHead);
		return;
	}
	struct job * pJob = pHead;
	if (pJob->m_pPCB == pPCB) {
		if (pTail == pHead) { //last = first
			printf("free last one in list: %d\n", pHead->m_pPCB->m_pid);
			releasePCB(pHead->m_pPCB);
			free(pHead);
			pTail = NULL;
			pHead=NULL;
		}
		else {
			printf("free head in list: %d\n", pHead->m_pPCB->m_pid);
			struct job * pTemp = pHead;
			pHead=pHead->m_next;
			releasePCB(pPCB);
			free(pTemp);
		}
		return;
	}
	while (pJob!=NULL) {
		if (pJob->m_next==NULL) {
			printf("%s didn't find this PCB[%d], something's wrong\n", __func__, pPCB->m_pid);
			return;
		}
		if (pJob->m_next->m_pPCB==pPCB){
			struct job * pTemp = pJob->m_next;
			pJob->m_next = pJob->m_next->m_next;
			releasePCB(pPCB);
			free(pTemp);
			return;
		}
		pJob=pJob->m_next;
	}
	
		
}

void *createJobs()
{
	//create jobs waiting to be dispatched.
	createOneJob(0, 5, 7, jobFunc);
	sleep(1);
	createOneJob(1, 3, 7, jobFunc);
	sleep(2);
	createOneJob(3, 2, 7, jobFunc);
	sleep(1);
	createOneJob(4, 6, 7, jobFunc);
	
}


struct job * getShortestJob()
{
	if (pHead == NULL) return NULL;

	struct job * pCurJob=pHead;
	struct job * pSelectedJob=pCurJob;
	struct pcb_t *pSelectedPCB=pSelectedJob->m_pPCB;
	while (pCurJob!=NULL) {
		struct pcb_t *p=pCurJob->m_pPCB;
		if (pSelectedPCB->m_bTime > p->m_bTime) {
			pSelectedJob = pCurJob;
		}
		pCurJob=pCurJob->m_next;
	}
	return pSelectedJob;
		


}

void *SFJ()
{
	printf("Enter SFJ pHead=%p pHead->m_pPCB=%p\n", pHead, pHead?pHead->m_pPCB:NULL);
	while(pHead!=NULL && pHead->m_pPCB!=NULL){
		struct job * p=getShortestJob();
		printf("get shortest Job %d, bTime=%d\n", p->m_pPCB->m_pid, p->m_pPCB->m_bTime);
		//run it
		//p->m_pPCB->pFunc(p->m_pPCB->m_pid, p->m_pPCB->m_bTime);
		runJobFunc(p->m_pPCB->m_pid, p->m_pPCB->m_bTime, p->m_pPCB->pFunc);
		//it is done, release it
		releaseOneJob(p->m_pPCB);
	}
	printf("Exit SFJ\n");
	
}

void *SRTF()
{
	printf("Enter SRTF pHead=%p pHead->m_pPCB=%p\n", pHead, pHead?pHead->m_pPCB:NULL);
	while(pHead!=NULL && pHead->m_pPCB!=NULL) {
		struct job * p=getShortestJob();
		runJobFunc(p->m_pPCB->m_pid, 1, p->m_pPCB->pFunc);
		p->m_pPCB->m_bTime--;
		printf("[SRTF]: job %d remaining time %d\n", p->m_pPCB->m_pid, p->m_pPCB->m_bTime);
		if (p->m_pPCB->m_bTime==0) {
			releaseOneJob(p->m_pPCB);
		}
	}
	printf("Exit SRTF pHead=%p pHead->m_pPCB=%p\n", pHead, pHead?pHead->m_pPCB:NULL);
}

int main(int argc, char * argv[])
{
	//TODO: Fork doesn't share pHead.
	//pid_t pid=fork();
	//if (pid <0) {
		//printf("fork failed: %d %s\n", errno, strerror(errno));
	//}else if (pid == 0) {
		//createJobs();
	//}else {
		//usleep(500);
		//SFJ();
	//}
	//waitpid(pid, NULL, 0);

	pthread_t pid1, pid2;
	
	pthread_create(&pid1, NULL, createJobs, NULL);
	usleep(500);
	//pthread_create(&pid2, NULL, SFJ, NULL);
	pthread_create(&pid2, NULL, SRTF, NULL);

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);



}
