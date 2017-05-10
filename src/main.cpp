#include "single_threaded_tests.h"
#include "multi_threaded_tests.h"


//void* test_2_threads_simple_thread_1(void* ptr) {
    //BinarySearchTree* bst = reinterpret_cast<BinarySearchTree*>(ptr);
    //for (int i = 0; i < 100; ++i) {
        //bst->insert(i); 
    //}
//}

//bool test_2_threads_simple() {
    //std::shared_ptr<BinarySearchTree> bst = std::make_shared<BST>(); 
    
//}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    test_single_add();
    test_triple_add();
    test_single_add_remove();
    test_bad_remove();
    test_repeated_add();
    test_nonexistent_remove();
    test_leaf_remove();
    test_single_child_remove();
    test_double_child_remove();
    test_single_child_remove_root();
    test_double_child_remove_root();

    test_1_threads();
    test_2_threads();
    test_4_threads();
    test_8_threads();
    test_16_threads();
    
    return 0;
}
