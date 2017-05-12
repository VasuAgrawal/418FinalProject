#include "CycleTimer.h"
#include "multi_threaded_tests.h"

#include <vector>
#include <random>
#include <algorithm>

namespace MultiThreaded {

static int const kLarge = 1 << 20;


void test_all() {
    // Write heavy testing
    std::cout << "Threads,Contention,Time (ms) (r/w)" << std::endl;
    for (int threads = 2; threads <= 32; threads *= 4) {
        for (float contention = 0; contention <= 1; contention += 1.0 / 4) {
            [threads, contention]() -> bool {
              INIT_TEST;
              test_n_threads_large(threads, contention - .00001);
              EXIT_TEST;
            }();
        }
    }


    // Read heavy testing
    //std::cout << "Threads,Contention,Time (ms) (Read-only)" << std::endl;
    //for (int threads = 1; threads <= 32; threads *= 2) {
        //for (float contention = 0; contention <= 1; contention += 1.0 / 4) {
            //[threads, contention]() -> bool {
                //INIT_TEST;
                //test_n_threads_read(threads, contention - .00001);
                //EXIT_TEST;
            //}();
        //}
    //}
}


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
    ThreadWriteInfo* ti = reinterpret_cast<ThreadWriteInfo*>(info);
    const size_t nPerThread = kLarge / ti->thread_count;
    std::uniform_int_distribution<int> dist(0, nPerThread - 1);
    std::uniform_int_distribution<int> insRemove(0, 1);

    for (size_t x = 0; x < kLarge / ti->thread_count; ++x) {
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

    srand(0); // Consistency!
    std::vector<std::default_random_engine> generators;
    std::vector<std::vector<int>> choices;
    std::vector<std::vector<bool>> contains;

    std::shared_ptr<BinarySearchTree> bst = std::make_shared<BST>();
    std::vector<pthread_t> threads;
    std::vector<ThreadWriteInfo> thread_info;

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


void* test_n_threads_read_fn(void* info) {
    ThreadReadInfo* ti = reinterpret_cast<ThreadReadInfo*>(info);

    for (const int elem : ti->choices) {
        ti->bst->contains(elem);
    }

    return nullptr;
}


bool test_n_threads_read(size_t n, float contention) {
    bool passed = true;
    contention = std::min(1.0f, std::max(0.0f, contention));
    if (VERBOSE)
        std::cout << "\tWith contention: " << ANSI_COLOR_CYAN << contention <<
            ANSI_COLOR_RESET << "\n";

    std::vector<std::vector<int>> choices;

    std::shared_ptr<BinarySearchTree> bst = std::make_shared<BST>();
    std::vector<pthread_t> threads;
    std::vector<ThreadReadInfo> thread_info;

    const size_t nPerThread = kLarge / n;
   
    srand(0); // Consistency!
    for (size_t i = 0; i < n; ++i) {
        choices.emplace_back();
        init_choices(&(choices[i]), i, n, nPerThread, contention);
        std::random_shuffle(choices[i].begin(), choices[i].end());

        threads.emplace_back();
        thread_info.emplace_back(i, n, bst, choices[i]);
    }

    // Each thread checks some number of elements. Only half of the elements are
    // going to be in the tree. We add a bunch of odd elements to a vector,
    // shuffle it, and then add it to the tree.
    std::vector<int> elements;
    for (int i = 0; i < kLarge; i += 2) {
        elements.push_back(i); 
    }
    std::random_shuffle(elements.begin(), elements.end());

    for (const int elem : elements) {
        EXPECT(bst->insert(elem));
    }

    double start_time = CycleTimer::currentSeconds();

    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_create(&threads[i], nullptr, test_n_threads_read_fn, &thread_info[i]);
    }

    for (auto& t : threads) {
        pthread_join(t, nullptr);
    }

    double end_time = CycleTimer::currentSeconds();

    // CSV style
    std::cout << n << "," << contention << "," <<
        (end_time - start_time) * 1000 << std::endl;

    return passed;
}

} // namespace MultiThreaded
