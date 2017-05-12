#ifndef MULTI_THREADED_TESTS_H_
#define MULTI_THREADED_TESTS_H_

#include <memory>
#include <random>

#include "testing.h"
#include "bst.h"
#include "utils.h"

#ifdef COARSE
#include "coarse_grained_bst.h"
#elif FINE
#include "fine_grained_bst.h"
#elif FINE_RW
#include "fine_grained_rw_bst.h"
#elif LOCKFREE
#include "lock_free_bst.h"
#else
#error Implementation not specified. Valid implementations are "COARSE", \
    "FINE", "FINE_RW", "LOCKFREE".
#endif

namespace MultiThreaded {

void test_all();


bool test_n_threads_large(size_t n, float contention);
struct ThreadWriteInfo {
    size_t tid;
    size_t thread_count;
    std::shared_ptr<BinarySearchTree> bst;
    std::default_random_engine gen;
    std::vector<int> choices;
    std::vector<std::vector<bool>>* contains;

    ThreadWriteInfo(size_t tid, size_t thread_count,
            std::shared_ptr<BinarySearchTree> bst,
            std::default_random_engine gen,
            std::vector<int> choices,
            std::vector<std::vector<bool>>* contains) :
        tid(tid), thread_count(thread_count), bst(bst),
        gen(gen), choices(choices), contains(contains) {}
};


bool test_n_threads_read(size_t n, float contention);
struct ThreadReadInfo {
    size_t tid;
    size_t thread_count;
    std::shared_ptr<BinarySearchTree> bst;
    std::vector<int> choices;

    ThreadReadInfo(size_t tid, size_t thread_count,
            std::shared_ptr<BinarySearchTree> bst,
            std::vector<int> choices) :
        tid(tid), thread_count(thread_count), bst(bst), choices(choices) {}
};


} // namespace MultiThreaded

#endif
