#include <stdio.h>
#include <stdlib.h>
#include "fcntl.h"
#include "errno.h"
#include "linux/videodev2.h"
#include <sys/ioctl.h>
#include "unistd.h"
#include <string.h>
#include "sys/mman.h"


#include "v4l2_funcs.h"

struct my_buffer_t {
	unsigned char * pBuffer;
	unsigned int length;
};

#define LENGTH 1920*1080*3/2
#define NUM_BUFF 4
static struct my_buffer_t mbuffer[NUM_BUFF];
static unsigned int mFrameNo=0;

void write2file(const void * pImage, unsigned int size, char * fileName);
void process_frame(int fd);

int openDevice(char * dev_name)
{
	int fd = open(dev_name, O_RDWR);
	if (fd < 0){
		err_print("unable to open device: %s", dev_name);
		return -1;
	}
	printf("%s success\n", __func__);
	return fd;
}

int queryCaps(int fd, struct v4l2_capability * caps)
{
	printf("query caps on device fd=%d, output caps = %p\n", fd, caps);
	if (fd <0) return -1;
	if (caps == NULL) return -1;
	int ret = ioctl(fd, VIDIOC_QUERYCAP, caps);
	if (ret !=0) {
		err_print("unable to query caps for %d", fd);
		return -1;
	}
	printf("%s success\n", __func__);
	return 0;
	

}	
int initCaptureDevice(int fd)
{
	printf("init device fd=%d\n", fd);
	if (fd <0) return -1;
	//query caps check if capture capability presents
	struct v4l2_capability caps={0};
	if (queryCaps(fd, &caps) <0) {
		err_print("%s query caps failed\n", __func__);
		return -1;
	}
	printCapability(&caps);
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (0>ioctl(fd, VIDIOC_G_FMT, &fmt)) {
		err_print("%s error while get format\n", __func__);
		return -1;
	}
	printCaptureFormat(&fmt);
	printf("%s success\n", __func__);
	return 0;
}

int initCaptureBuffer(int fd)
{
	int i=0;
	struct v4l2_requestbuffers req;
	memset(&req, 0, sizeof(struct v4l2_requestbuffers));
	req.count=4;
	req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory=V4L2_MEMORY_MMAP;

	if (0>ioctl(fd, VIDIOC_REQBUFS, &req)) {
		err_print("[ERROR]: failed to request buffer");
		return -1;
	}

	printf("Successfully requested buffer: count=%d\n", req.count);

	for (i=0;i<req.count;i++) {
		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(struct v4l2_buffer));

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (0>ioctl(fd,VIDIOC_QUERYBUF, &buf)) {
			err_print("error while query buffer");
			return -1;
		}
		mbuffer[i].pBuffer = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
		if (MAP_FAILED == mbuffer[i].pBuffer) {
			err_print("error while mmap buffer");
			return -1;
		}
	}

		 

	printf("%s success\n", __func__);
	return 0;
}

int setCaptureFormat(int fd, int width, int height)
{
	printf("fd=%d\n", fd);
	if (fd <0) return -1;
	//query caps check if capture capability presents
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width=width;
	fmt.fmt.pix.height=height;
	if (0>ioctl(fd, VIDIOC_S_FMT, &fmt)) {
		err_print("%s error while set format\n", __func__);
		return -1;
	}
	printCaptureFormat(&fmt);
	printf("%s success\n", __func__);
	return 0;
}
int releaseDevice(int fd)
{
	int i=0;
	for (i=0;i<NUM_BUFF;i++){
		if (0>munmap(mbuffer[i].pBuffer, mbuffer[i].length)) {
			err_print("unable to munmap");
		}
	}
	close(fd);
	printf("%s success\n", __func__);
	return 0;
}

int stopCapture(int fd)
{
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (0>ioctl(fd, VIDIOC_STREAMOFF, &type)) {
		err_print("unable to stream off");
		return -1;
	}
	printf("%s success\n", __func__);
	return 0;
}

int startCapture(int fd)
{
	int i=0;
	enum v4l2_buf_type type;
	for (i=0;i<NUM_BUFF;i++){
		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(struct v4l2_buffer));

		buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (0>ioctl(fd, VIDIOC_QBUF, &buf)) {
			err_print("QBUF[%d] failed", i);
			return -1;
		}
	}
	type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (0>ioctl(fd, VIDIOC_STREAMON, &type)) {
		err_print("stream on failed");
		return -1;
	}
	printf("Start capture successfully\n");
	return 0;

}


//=======================HELPER FUNC==========================
int processFrame(int fd)
{
	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(struct v4l2_buffer));
	buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory=V4L2_MEMORY_MMAP;
	if (0>ioctl(fd, VIDIOC_DQBUF, &buf)) {
		if (errno == EAGAIN) {
			return 0; //not counted as a failure
		}
		err_print("dqbuf failed");
		return -1;
	}
	mFrameNo++;
	printf("get buffer:frame number=%d index=%d start=%p bytesused=%u\n",
		       mFrameNo, buf.index, mbuffer[buf.index].pBuffer, buf.bytesused);
	mbuffer[buf.index].length=buf.bytesused;
	char filename[64];
	memset(filename, '\0', 64);
	sprintf(filename, "image-%d.yuv", mFrameNo);
	write2file(mbuffer[buf.index].pBuffer, buf.bytesused, filename);

	//return buffer
	if (0>ioctl(fd, VIDIOC_QBUF, &buf)) {
		err_print("qbuf failed");
		return -1;
	}
	return 0;

}
void write2file(const void * pImage, unsigned int size, char * fileName)
{
	if (NULL==pImage||NULL==fileName) {
		err_print("Invalid write file param: %s %s", pImage, fileName);
		return;
	}
	FILE * fp = fopen(fileName, "wb");
	fwrite(pImage, size, 1, fp);
	fflush(fp);
	fclose(fp);

}
void printCapability(const struct v4l2_capability * const caps)
{
	if (NULL==caps) {
		printf("[%s]ERROR, NULL ptr\n", __func__);
		return;
	}
	printf("Device capability:\n \
			driver: %s \n \
			card %s \n \
			bus_info: %s \n \
			version %u \n \
			capabilities 0x%x \n\
			device_cap 0x%x \n \
			reserved 0x%x 0x%x 0x%x\n",
			caps->driver, caps->card, caps->bus_info,
			caps->version, caps->capabilities, caps->device_caps, 
			caps->reserved[0], caps->reserved[1], caps->reserved[2]);
	printf("capability:\n \
			video capture: 0x%x\n \
			video output: 0x%x \n \
			video overlay 0x%x \n \
			vbi capture 0x%x \n \
			vbi output 0x%x \n \
			sliced vbi capture 0x%x \n \
			sliced vbi outptu 0x%x \n \
			rds capture 0x%x \n \
			video output overlay 0x%x \n \
			hw freq seek 0x%x \n \
			rds output 0x%x \n ",
			caps->capabilities&V4L2_CAP_VIDEO_CAPTURE,
			caps->capabilities&V4L2_CAP_VIDEO_OUTPUT,
			caps->capabilities&V4L2_CAP_VIDEO_OVERLAY,
			caps->capabilities&V4L2_CAP_VBI_CAPTURE,
			caps->capabilities&V4L2_CAP_VBI_OUTPUT,
			caps->capabilities&V4L2_CAP_SLICED_VBI_CAPTURE,
			caps->capabilities&V4L2_CAP_SLICED_VBI_OUTPUT,
			caps->capabilities&V4L2_CAP_RDS_CAPTURE,
			caps->capabilities&V4L2_CAP_VIDEO_OUTPUT_OVERLAY,
			caps->capabilities&V4L2_CAP_HW_FREQ_SEEK,
			caps->capabilities&V4L2_CAP_RDS_OUTPUT);
}

void printCaptureFormat(const struct v4l2_format * const fmt)
{
	if (NULL==fmt) {
		printf("[%s]ERROR, NULL ptr\n", __func__);
		return;
	}
	printf("V4l2 format(%p):type=0x%x format= \n \
			width=%d height=%d pixelformat=0x%x field=%d bytesperline=%d \n \
			sizeimage=%d colorspace=0x%x priv=0x%x flags=0x%x ycvcr_enc=0x%x \n \
			quantization=%d xfer_func=%d\n",
		fmt, fmt->type, 
		fmt->fmt.pix.width, fmt->fmt.pix.height, fmt->fmt.pix.pixelformat, fmt->fmt.pix.field,
		fmt->fmt.pix.bytesperline, fmt->fmt.pix.sizeimage, fmt->fmt.pix.colorspace, fmt->fmt.pix.priv,
		fmt->fmt.pix.flags, fmt->fmt.pix.ycbcr_enc, fmt->fmt.pix.quantization, fmt->fmt.pix.xfer_func
	      );

}




