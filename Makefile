CFILE += xsocketd.c
HEADER += xsocketd.h
CC = gcc
FLAGS = -shared -fPIC -o
TARGET = libxsocketd.so 
HEADERDIR = /usr/include
LIBDIR = /usr/lib

all:
	$(CC) $(CFILE) $(FLAGS) $(TARGET)

install:
	@cp $(TARGET) $(LIBDIR)
	@cp $(HEADER) $(HEADERDIR) 

.PHONY:clean
clean:
	@rm -rf $(TARGET) 
