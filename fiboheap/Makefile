CFLAGS := -g

SRC := fibo.cpp \
	fiboheap.cpp \
	heap.cpp \

TARGET := fibotest

OBJS := $(patsubst %.c,%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(SRC) $(CFLAGS) -o $(TARGET)

clean:
	rm -f *.o
	rm $(TARGET)
