#ifndef MULTI_THREADED_TESTS_H_
#define MULTI_THREADED_TESTS_H_

#include <memory>

#include "testing.h"
#include "bst.h"

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

} // namespace MultiThreaded

#endif
