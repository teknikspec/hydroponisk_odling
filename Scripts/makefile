# Makefile for read_serial program

CC = g++
CFLAGS = -Wall -std=c++11
LIBS = -lserial -I/usr/include/libserial

SRC = fetch_data.cpp
TARGET = fetch_data

all: 
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)
