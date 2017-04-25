#include <memory>

#include "testing.h"
#include "bst.h"

#ifdef COARSE
#include "coarse_grained_bst.h"
#elif FINE
#include "fine_grained_bst.h"
#else
#error Implementation not specified. Valid implementations are "COARSE".
#endif

struct ThreadInfo {
    size_t tid;
    std::shared_ptr<BinarySearchTree> bst;

    ThreadInfo(size_t tid, std::shared_ptr<BinarySearchTree> bst) : tid(tid),
        bst(bst) {}
};

bool test_2_threads();
