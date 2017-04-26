CC=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++14 -pthread
BUILDDIR=bin
TEST_SRCS=src/single_threaded_tests.cpp src/multi_threaded_tests.cpp src/main.cpp

all: directory coarse

directory:
	mkdir -p $(BUILDDIR)

IMPL_SRC = src/coarse_grained_bst.cpp
coarse:
	$(CC) $(CXXFLAGS) -o coarse $(IMPL_SRC) $(TEST_SRCS) -D COARSE
	mv coarse $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
