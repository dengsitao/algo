#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include "fcntl.h"
#include "string.h"

#define err_print(fmt, ...) {printf("[ERROR %d %s][%s] "fmt"\n",errno, strerror(errno), __func__, ##__VA_ARGS__);}

void * getMappedMemory(int fd, unsigned int size)
{
	if (0 > fd || 0>=size) {
		err_print("invalid arguement fd=%d size=%d",fd, size);
		return NULL;
	}
	void * p = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (NULL == p) {
		err_print("failed to mmap");
		return NULL;
	}
	return p;
}

int openMem(char * fileName, int create)
{
	if (NULL == fileName) {
		err_print("no file name");
		return -1;
	}
	int flags=O_RDWR|O_TRUNC;
	if (create>0) {
		printf("create file\n");
		flags |= O_CREAT;
	}	
	int fd = open(fileName, flags, 00666);
	if (0 > fd) {
		err_print("open failed");
	}
	if (lseek(fd, 1024, SEEK_SET) == -1)
    	{
        	err_print("Error calling lseek()");
    	}
	if (0 > write(fd, "",1)) {
		err_print("Error write");
	}
	return fd;
}

void releaseMem(int fd, void * p, unsigned int size)
{
	if (0>fd || 0>=size || NULL==p) {
		err_print("invalid arguement fd=%d size=%u p=%p", fd, size, p);
		return;
	}
	if (0> munmap(p, size)) {
		err_print("munmap failed");
	}
	if (0> close(fd)) {
		err_print("close failed");
	}
	
}

int main(int argc, char * argv[])
{
	int fd1 = openMem("/tmp/my_shr_mem", 1);
	unsigned char * pMem1 = getMappedMemory(fd1, 1024);
	int fd2 = openMem("/tmp/my_shr_mem", 0);
	unsigned char * pMem2 = getMappedMemory(fd2, 1024);
	
	pid_t child_pid=fork();
	if (child_pid<0) {
		err_print("fork failed");
	}
	else if (child_pid==0) {
		sleep(1);
		printf("in child %d, pMem[1]=%c pMem[1023]=%c\n", getpid(), pMem1[1], pMem1[1023]);
		exit(42);
	}
	else {
		printf("In parent %d\n", getpid());
		memset(pMem2, 'g', 1024);
		pMem2[1]='a';
	}
	int status=0;
	waitpid(child_pid, &status, 0);
	printf("child finished with status %d\n", WEXITSTATUS(status));
	releaseMem(fd1, pMem1, 1024);
	releaseMem(fd2, pMem2, 1024);

	return 0;
}
