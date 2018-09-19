CC     := gcc
TARGET := libspdglog.so
SRCS   := $(wildcard *.c)

all: ${TARGET}

$(TARGET): $(SRCS)
	$(CC) -shared -fPIC -o $@ $^ -g
	@\cp $@ /usr/lib64/

.PHONY: clean
clean:
	rm -f *.o $(TARGET)
