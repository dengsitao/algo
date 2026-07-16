#ifndef __V4LL2FUNCS_H__
#define __V4LL2FUNCS_H__

#include "linux/videodev2.h"
#define err_print(fmt, ...) {printf("[ERROR %d %s] "fmt"\n",errno, strerror(errno), ##__VA_ARGS__);}

void printCapability(const struct v4l2_capability * const caps);
void printCaptureFormat(const struct v4l2_format * const fmt);

int openDevice(char * dev_name);
int queryCaps(int fd, struct v4l2_capability * caps);
int initCaptureDevice(int fd);
int initCaptureBuffer(int fd);
int releaseDevice(int fd);
int setCaptureFormat(int fd, int width, int height);
int startCapture(int fd);
int stopCapture(int fd);
int processFrame(int fd);

#endif
