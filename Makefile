CC     := gcc
TARGET := libdglog.so
SRCS   := $(wildcard *.c)

all: ${TARGET}

$(TARGET): $(SRCS)
	$(CC) -shared -fPIC -o $@ $^ -g

.PHONY: clean
clean:
	rm -f *.o $(TARGET)
