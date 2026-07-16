#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include <string.h>
#include <errno.h>
#include "unistd.h"
#include <pthread.h>
#include "sys/types.h"
#include "time.h"

#define DEBUG 0
#define INFO  1
#define WARN  2
#define ERROR 4
int m_debug_level=DEBUG;
#define LOGD(fmt, ...) if (m_debug_level<=DEBUG) {printf(fmt, ##__VA_ARGS__);}
#define LOGI(fmt, ...) if (m_debug_level<=INFO) {printf(fmt, ##__VA_ARGS__);}
#define LOGW(fmt, ...) if (m_debug_level<=WARN) {printf(fmt, ##__VA_ARGS__);}
#define LOGE(fmt, ...) if (m_debug_level<=ERROR) {printf(fmt, ##__VA_ARGS__);}
#define TRUE 1
#define FALSE 0
//TODO: make it thread safe

pthread_mutex_t m_lock;

struct mem_meta_t
{
	size_t m_mem_size;
	struct mem_meta_t * next;
	void * m_p_addr;
	int m_free;

} *my_mem_meta, *my_last_meta;

#define META_SIZE sizeof(struct mem_meta_t)

void * my_alloc_new_block(size_t mem_size)
{
	if (mem_size == 0) return NULL;
	void * p = sbrk(mem_size+META_SIZE);
	if (p == (void*)(-1)) {
		LOGE("ERROR on sbrk: %d %s\n", errno, strerror(errno));
		return NULL;
	}
	struct mem_meta_t * p_meta = p;
	p_meta->m_mem_size=mem_size;
	p_meta->m_free=FALSE;
	p_meta->m_p_addr=p+META_SIZE;
	if (my_last_meta ==NULL) {
		my_mem_meta=p_meta;
		my_last_meta=p_meta;
	}
	else {
		my_last_meta->next = p_meta;
		my_last_meta=p_meta;
	}
	return p_meta->m_p_addr;
		
}

void my_free(void * p)
{
	if (p==NULL) return;
	//release meta data
	pthread_mutex_lock(&m_lock);
	struct mem_meta_t * p_meta = p - META_SIZE;
	p_meta->m_free = TRUE;
	LOGD("[%lx]releasing p=%p addr=%p free=%d\n",pthread_self(), p_meta, p_meta->m_p_addr, p_meta->m_free);
	pthread_mutex_unlock(&m_lock);

}

struct mem_meta_t * my_find_available_block(size_t mem_size)
{
	struct mem_meta_t * p = my_mem_meta;
	while(p!=NULL) {
		LOGD("checking: p=%p, p->m_p_addr=%p free=%d\n", p, p->m_p_addr, p->m_free);
		if (p->m_free == TRUE && p->m_mem_size == mem_size) {
			return p;
		}
		p=p->next;
	}
	return NULL;
}

void *my_malloc(size_t mem_size)
{
	//lock
	pthread_mutex_lock(&m_lock);
	if (my_mem_meta == NULL) {
		LOGD("%s: first time enter, allocate size %lu + %lu\n", __func__, sizeof(struct mem_meta_t), mem_size);
		
		void * p = my_alloc_new_block(mem_size);
		if ( p==NULL ) {
			LOGE("[ERROR]: failed to create mem block\n");
			return NULL;
		}
		pthread_mutex_unlock(&m_lock);
		return p;
	}
	//now look for unused memblock
	LOGD("[%lx]now lock lock %p\n",pthread_self(),  &m_lock);
	struct mem_meta_t * p_meta = my_find_available_block(mem_size);
	if (p_meta != NULL)
	{
		LOGD("Found exiting freed block, use it %p, %p, %lu\n", p_meta, p_meta->m_p_addr, p_meta->m_mem_size);
		p_meta->m_mem_size=mem_size;
		p_meta->m_free=FALSE;
		pthread_mutex_unlock(&m_lock);
		return p_meta->m_p_addr;
	}
	// no unused memory block found.
	LOGD("no available block, allocate for %lu + %lu\n", __func__, sizeof(struct mem_meta_t), mem_size);
	void * p = my_alloc_new_block(mem_size);
	if ( p==NULL ) {
		LOGE("[ERROR]: failed to create mem block\n");
		pthread_mutex_unlock(&m_lock);
		return NULL;
	}
	//unlock
	LOGD("[%lx]now release lock %p\n",pthread_self(), &m_lock);
	pthread_mutex_unlock(&m_lock);
	return p;
	
	
}
//thread-func
void *test_malloc(void * arg)
{
	pthread_t my_id = pthread_self();
	LOGD("[%lx]Enter thread \n", (unsigned long)my_id);
	struct timespec start, end;
	long sec=0;
	long nanosec=0;
	clock_gettime(CLOCK_MONOTONIC, &start);
	for( int i=0;i<1000;i++) {
		void * p = my_malloc(1024);
		//LOGI("[%lx] malloced %p \n", (unsigned long)my_id, p);
		//sleep(1);
		my_free(p);
	}

	clock_gettime(CLOCK_MONOTONIC, &end);
	sec = end.tv_sec - start.tv_sec;
	nanosec = end.tv_nsec - start.tv_nsec;
	printf("[%lx]use %lu:%lu.%lu\n", (unsigned long)my_id, sec, nanosec/(1000*1000), nanosec/1000);
	return NULL;
}

//======================main===================
int main(int argc, char * argv[])
{
	printf(" Enter: argc=%d, sbrk(0)=%p\n",argc, sbrk(0));
	if (argc>=2) {
		LOGD("arcv[0]=%s, argv[1]=%s len=%d\n", argv[0], argv[1], strlen(argv[1]));
		switch (argv[1][0]) {
			case 'd':
				if (strlen(argv[1])>=3 && (argv[1][2]>=DEBUG+48 && argv[1][2]<=ERROR+48)) {
					m_debug_level=atoi(&(argv[1][2]));	
					LOGE("debug level set to %d %d\n",m_debug_level, argv[1][2]);
				}
				else {
					LOGE("USAGE: -d={0,1,2,3} stands for DEBUG, INFO, WARNING, ERROR (%s)\n", argv[1]);
				}
				break;
			default:
				LOGE("USAGE: -d={0,1,2,3} stands for DEBUG, INFO, WARNING, ERROR (%s)\n", argv[1]);
			}
	}
	//initialization
	pthread_mutex_init(&m_lock, NULL);
	//test single thread
	/*
	void * p1 = my_malloc(1024);
	printf("1 malloc got %p\n", p1);
	printf("now sbrk(0)=%p\n", sbrk(0));
	void * p2 = my_malloc(1024);
	printf("2 malloc got %p\n", p2);
	printf("now sbrk(0)=%p\n", sbrk(0));
	void * p3 = my_malloc(1024);
	printf("3 malloc got %p\n", p3);
	printf("now sbrk(0)=%p\n", sbrk(0));
	my_free(p2);
	void * p = my_malloc(1024);
	printf("4 malloc got %p\n", p);
	printf("now sbrk(0)=%p\n", sbrk(0));

	void * p4 = my_malloc(4096);
	printf("5 malloc got %p\n", p4);
	printf("now sbrk(0)=%p\n", sbrk(0));
	void * p5 = my_malloc(0);
	printf("6 malloc got %p\n", p5);
	printf("now sbrk(0)=%p\n", sbrk(0));
	void * p6 = my_malloc(-1);
	printf("7 malloc got %p\n", p6);
	printf("now sbrk(0)=%p\n", sbrk(0));
	my_free(p4);
	void * p7 = my_malloc(2048);
	printf("8 malloc got %p\n", p7);
	printf("now sbrk(0)=%p\n", sbrk(0));
	my_free(p1);
	my_free(p2);
	my_free(p3);
	my_free(p4);
	my_free(p5);
	my_free(p6);
	my_free(p7);
*/
	//test multi-thread
	printf("start test multi-thread\n");
	pthread_attr_t m_th_attr1;
	pthread_attr_t m_th_attr2;

	pthread_t m_th1;
	pthread_t m_th2;

	pthread_attr_init(&m_th_attr1);
	pthread_attr_init(&m_th_attr2);

	pthread_attr_setschedpolicy(&m_th_attr1, SCHED_FIFO);
	
	pthread_create(&m_th1, &m_th_attr1, &test_malloc, NULL);
	//pthread_create(&m_th2, NULL, &test_malloc, NULL);

	pthread_join(m_th1, NULL);
	//pthread_join(m_th2, NULL);

	pthread_mutex_destroy(&m_lock);
	pthread_attr_destroy(&m_th_attr1);
	pthread_attr_destroy(&m_th_attr2);
	
	return 0;
}

