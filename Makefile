CC=g++
CFLAGS=-Wall -Wextra -pedantic -std=c++11
BUILDDIR=bin

all: directory coarse

directory:
	mkdir -p $(BUILDDIR)

coarse:
	$(CC) $(CFLAGS) -o coarse src/coarse_grained_bst.cpp src/main.cpp -D COARSE
	mv coarse $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
