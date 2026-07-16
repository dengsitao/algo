#include <stdio.h>
#include <stdlib.h>
#include "fcntl.h"
#include "errno.h"
#include "linux/videodev2.h"
#include <sys/ioctl.h>
#include "unistd.h"
#include <string.h>
#include "v4l2_funcs.h"

char DEVICE[128] = "/dev/video0";

void mainloop(int fd)
{
	int count=10;

	while(count-- > 0) {
		//for (;;) {
			fd_set fds;
			struct timeval tc;
			int result=0;
			FD_ZERO(&fds);
			FD_SET(fd, &fds);

			tc.tv_sec = 2;
			tc.tv_usec = 0;

			result = select(fd+1, &fds, NULL, NULL, &tc);

			if (0 > result) {
				if (EINTR == errno) continue;
				err_print("main loop select error");
				return;
			}
			if (0 == result ) {
				err_print("select time out");
				return;
			}
			if (0>processFrame(fd)) {
				err_print("processFrame failed\n");
				return;
			}
		//}

	}
}

int main(int argc, char * argv[])
{
	if (argc >1) {
		if (strlen(argv[1])<128){
			strcpy(DEVICE, argv[1]);
			printf("DEVICE set to %s\n", DEVICE);
		}
	}
	int fd = openDevice(DEVICE);
	initCaptureDevice(fd);
	setCaptureFormat(fd, 1920, 1080);
	initCaptureBuffer(fd);
	startCapture(fd);
	mainloop(fd);
	stopCapture(fd);
	releaseDevice(fd);
	return 0;
}
