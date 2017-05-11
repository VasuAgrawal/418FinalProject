CC=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11 -pthread -O3
BUILDDIR=bin
TEST_SRCS=src/single_threaded_tests.cpp src/multi_threaded_tests.cpp src/main.cpp

all: directory coarse fine lockfree

directory:
	mkdir -p $(BUILDDIR)

COARSE_FILES = src/coarse_grained_bst.cpp
coarse:
	$(CC) $(CXXFLAGS) -o $@ $(COARSE_FILES) $(TEST_SRCS) -D COARSE
	mv $@ $(BUILDDIR)


FINE_FILES = src/fine_grained_bst.cpp
fine:
	$(CC) $(CXXFLAGS) -o $@ $(FINE_FILES) $(TEST_SRCS) -D FINE
	mv $@ $(BUILDDIR)


LF_FILES = src/lock_free_bst.cpp
lockfree:
	$(CC) $(CXXFLAGS) -o $@ $(LF_FILES) $(TEST_SRCS) -D LOCKFREE
	mv $@ $(BUILDDIR)

lf: lockfree

clean:
	rm -rf $(BUILDDIR)
