#include "single_threaded_tests.h"
#include "multi_threaded_tests.h"


int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    
    std::cout << "Starting single threaded tests...\n";
    SingleThreaded::test_all();
    std::cout << "Done!\nStarting multi threaded tests...\n";
    MultiThreaded::test_all();
    std::cout << "Done!\n";

    return 0;
}
