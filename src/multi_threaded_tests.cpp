#include "CycleTimer.h"
#include "multi_threaded_tests.h"

#include <vector>
#include <random>
#include <algorithm>

namespace MultiThreaded {

void test_all() {
    test_1_threads();
    test_2_threads();
    test_4_threads();
    test_8_threads();
    test_16_threads();

    std::cout << "Threads,Contention,Time (ms)" << std::endl;
    for (int threads = 2; threads <= 32; threads *= 4) {
        for (float contention = 0; contention <= 1; contention += 1.0 / 4) {
            [threads, contention]() -> bool {
              INIT_TEST;
              test_n_threads_large(threads, contention);
              EXIT_TEST;
            }();
        }
    }
}

static int const kTestNThreadsCount = 10000;
void* test_n_threads_fn(void* info) {
    ThreadInfo* ti = reinterpret_cast<ThreadInfo*>(info);

    for (int i = ti->tid; i < kTestNThreadsCount; i += ti->thread_count) {
        ti->bst->insert(i);
    }

    return nullptr;
}


bool test_n_threads(size_t n) {
    bool passed = true;
    std::vector<pthread_t> threads;
    std::vector<ThreadInfo> thread_info;

    std::shared_ptr<BinarySearchTree> bst = std::make_shared<BST>();
    for(size_t i = 0; i < n; ++i) {
        thread_info.emplace_back(i, n, bst);
        threads.emplace_back();
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_create(&threads[i], nullptr, test_n_threads_fn, &thread_info[i]);
    }

    for (auto& t : threads) {
        pthread_join(t, nullptr);
    }

    for (int i = 0; i < kTestNThreadsCount; ++i) {
        EXPECT(bst->contains(i));
    }
           
    return passed;
}

bool test_1_threads()  { INIT_TEST; EXPECT(test_n_threads(1));  EXIT_TEST; }
bool test_2_threads()  { INIT_TEST; EXPECT(test_n_threads(2));  EXIT_TEST; }
bool test_4_threads()  { INIT_TEST; EXPECT(test_n_threads(4));  EXIT_TEST; }
bool test_8_threads()  { INIT_TEST; EXPECT(test_n_threads(8));  EXIT_TEST; }
bool test_16_threads() { INIT_TEST; EXPECT(test_n_threads(16)); EXIT_TEST; }


bool contains(std::vector<int> v, int x) {
    return std::find(v.begin(), v.end(), x) != v.end();
}

static int const kLarge = 1 << 23;
static int const operCount = 1 << 20;
void init_choices(std::vector<int>* choices, size_t thread_id, size_t nThreads, 
                  size_t nPerThread, float contention) {
    size_t chunkSize = (size_t) (nPerThread - (nPerThread - 1) * contention);

    for (int chunk = 0; chunk*chunkSize < kLarge; ++chunk) {
        int start = chunk * chunkSize * nThreads + chunkSize * thread_id;
        int end = start + chunkSize;
        for (int x = start; x < end; ++x) {
            choices->push_back(x);
            if (choices->size() >= nPerThread) return;
        }
    }
}

void* test_n_threads_large_fn(void* info) {
    ThreadLargeInfo* ti = reinterpret_cast<ThreadLargeInfo*>(info);
    const size_t nPerThread = kLarge / ti->thread_count;
    std::uniform_int_distribution<int> dist(0, nPerThread - 1);
    std::uniform_int_distribution<int> insRemove(0, 1);

    for (size_t x = 0; x < operCount / ti->thread_count; ++x) {
        bool ins = insRemove(ti->gen) == 1;
        int i = dist(ti->gen);
        int v = (ti->choices)[i];
        if (ins) {
            ti->bst->insert(v);
        } else {
            ti->bst->remove(v);
        }
        (*(ti->contains))[ti->tid][i] = ins;
    }

    return nullptr;
}

bool test_n_threads_large(size_t n, float contention) {
    bool passed = true;
    contention = std::min(1.0f, std::max(0.0f, contention));
    if (VERBOSE)
        std::cout << "\tWith contention: " << ANSI_COLOR_CYAN << contention <<
            ANSI_COLOR_RESET << "\n";

    std::vector<std::default_random_engine> generators;
    std::vector<std::vector<int>> choices;
    std::vector<std::vector<bool>> contains;

    std::shared_ptr<BinarySearchTree> bst = std::make_shared<BST>();
    std::vector<pthread_t> threads;
    std::vector<ThreadLargeInfo> thread_info;

    const size_t nPerThread = kLarge / n;

    for (size_t i = 0; i < n; ++i) {
        generators.emplace_back();
        choices.emplace_back();
        contains.emplace_back(nPerThread, false);
        init_choices(&(choices[i]), i, n, nPerThread, contention);

        threads.emplace_back();
        thread_info.emplace_back(i, n, bst, generators[i],
            choices[i], &contains);
    }

    double start_time = CycleTimer::currentSeconds();

    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_create(&threads[i], nullptr, test_n_threads_large_fn, &thread_info[i]);
    }

    for (auto& t : threads) {
        pthread_join(t, nullptr);
    }

    double end_time = CycleTimer::currentSeconds();

    //std::cout << n << " threads; contention: " << contention << std::endl <<
        //"\t" << (end_time - start_time)*1000 << " ms\n";

    // CSV mode
    std::cout << n << "," << contention << "," << 
      (end_time - start_time) * 1000 << std::endl;

    for (size_t i = 0; i < contains.size(); ++i) {
        for (size_t j = 0; j < contains[i].size(); ++j) {
            bool in = bst->contains(choices[i][j]);
            if (contains[i][j]) {
                EXPECT(in);
            } else {
                EXPECT_NOT(in);
            }
        }
    }

    return passed;
}

bool test_1_threads_large(float contention)  {
    INIT_TEST;
    EXPECT(test_n_threads_large(1, contention));
    EXIT_TEST;
}

bool test_2_threads_large(float contention)  {
    INIT_TEST;
    EXPECT(test_n_threads_large(2, contention));
    EXIT_TEST;
}

bool test_4_threads_large(float contention)  {
    INIT_TEST;
    EXPECT(test_n_threads_large(4, contention));
    EXIT_TEST;
}

bool test_8_threads_large(float contention)  {
    INIT_TEST;
    EXPECT(test_n_threads_large(8, contention));
    EXIT_TEST;
}

bool test_16_threads_large(float contention) {
    INIT_TEST;
    EXPECT(test_n_threads_large(16, contention));
    EXIT_TEST;
}


} // namespace MultiThreaded
