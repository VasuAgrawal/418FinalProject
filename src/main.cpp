#include "single_threaded_tests.h"
#include "multi_threaded_tests.h"


int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    
    SingleThreaded::test_all();
    MultiThreaded::test_all();

    return 0;
}
