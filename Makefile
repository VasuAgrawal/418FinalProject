CC=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11 -pthread -O3 -Wl,-rpath,/opt/gcc/4.9.2/lib64
BUILDDIR=bin
TEST_SRCS=src/single_threaded_tests.cpp src/multi_threaded_tests.cpp src/main.cpp

all: directory coarse fine fine_rw lockfree

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


RW_FILES = src/fine_grained_rw_bst.cpp
fine_rw:
	$(CC) $(CXXFLAGS) -o $@ $(RW_FILES) $(TEST_SRCS) -D FINE_RW
	mv $@ $(BUILDDIR)


LF_FILES = src/lock_free_bst.cpp
lockfree:
	$(CC) $(CXXFLAGS) -o $@ $(LF_FILES) $(TEST_SRCS) -D LOCKFREE
	mv $@ $(BUILDDIR)

lf: lockfree

clean:
	rm -rf $(BUILDDIR)
