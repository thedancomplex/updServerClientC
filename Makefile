default: all

FLAGS := -I./include -g --std=gnu99
CXXFLAGS := -I./include -g

CC := gcc
CXX := g++

BINARIES := udpServer udpClient
all : $(BINARIES)

LIBS := -lach -lrt -lm -lc

udpServer: src/udpServer.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

udpClient: src/udpClient.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BINARIES) src/*.o
