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
#elif LOCKFREE
#include "lock_free_bst.h"
#else
#error Implementation not specified. Valid implementations are "COARSE", "FINE", "LOCKFREE".
#endif

namespace MultiThreaded {

void test_all();

struct ThreadInfo {
    size_t tid;
    size_t thread_count;
    std::shared_ptr<BinarySearchTree> bst;

    ThreadInfo(size_t tid, size_t thread_count,
            std::shared_ptr<BinarySearchTree> bst) :
        tid(tid), thread_count(thread_count), bst(bst) {}
};

bool test_n_threads(size_t n);
bool test_1_threads();
bool test_2_threads();
bool test_4_threads();
bool test_8_threads();
bool test_16_threads(); 

struct ThreadLargeInfo {
    size_t tid;
    size_t thread_count;
    std::shared_ptr<BinarySearchTree> bst;
    std::default_random_engine gen;
    std::vector<int> choices;
    std::vector<std::vector<bool>>* contains;

    ThreadLargeInfo(size_t tid, size_t thread_count,
            std::shared_ptr<BinarySearchTree> bst,
            std::default_random_engine gen,
            std::vector<int> choices,
            std::vector<std::vector<bool>>* contains) :
        tid(tid), thread_count(thread_count), bst(bst),
        gen(gen), choices(choices), contains(contains) {}
};

bool test_n_threads_large(size_t n, float contention);
bool test_1_threads_large(float contention);
bool test_2_threads_large(float contention);
bool test_4_threads_large(float contention);
bool test_8_threads_large(float contention);
bool test_16_threads_large(float contention); 

} // namespace MultiThreaded

#endif
