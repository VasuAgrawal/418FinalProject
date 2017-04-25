#include "multi_threaded_tests.h"

#include <vector>

void* test_2_threads_fn(void* info) {
    ThreadInfo* ti = reinterpret_cast<ThreadInfo*>(info);

    for (int i = ti->tid; i < 100; ++i) {
        ti->bst->insert(i);
    }

    return nullptr;
}

bool test_2_threads() {
    INIT_TEST;
    std::vector<pthread_t> threads;
    std::vector<ThreadInfo> thread_info;

    std::shared_ptr<BinarySearchTree> bst = std::make_shared<BST>();
    for(int i = 0; i < 2; ++i) {
        thread_info.emplace_back(i, bst);
        threads.emplace_back();
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_create(&threads[i], nullptr, test_2_threads_fn, &thread_info[i]);
    }

    for (auto& t : threads) {
        pthread_join(t, nullptr);
    }

    for (int i = 0; i < 100; ++i) {
        EXPECT(bst->contains(i));
    }
            
    EXIT_TEST;
}
