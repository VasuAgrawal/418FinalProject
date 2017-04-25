#include <iostream>
#include <memory>

#include "bst.h"

#ifdef COARSE
#include "coarse_grained_bst.h"
#elif FINE
#include "fine_grained_bst.h"
#else
#error Implementation not specified. Valid implementations are "COARSE".
#endif

#define N 20

#define INIT_TEST \
        std::cout << "Starting test " << __func__ << "!" << std::endl; \
        bool passed = true;

#define EXIT_TEST \
        if (passed) { \
            std::cout << "Test " << __func__ << " passed!" << std::endl; \
        } else { \
            std::cout << "Test " << __func__ << " FAILED!" << std::endl; \
        } \
        std::cout << std::endl; \
        return passed;

#define EXPECT(x) passed &= expect((x), __LINE__)
#define EXPECT_NOT(x) passed &= expect((!x), __LINE__)

inline bool expect(bool result, int line) {
  if (!result) {
    std::cout << "  EXPECT at line " << line << " returned false." << std::endl;
  }
   
  return result;
}


bool test_single_add() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    bst->insert(1);
    EXPECT(bst->contains(1));
    EXIT_TEST;
}

bool test_single_add_remove() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    bst->insert(0);
    EXPECT(bst->contains(0));
    EXPECT(bst->remove(0));
    EXPECT_NOT(bst->contains(0));
    EXIT_TEST;
}

bool test_bad_remove() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT_NOT(bst->remove(0));
    EXPECT_NOT(bst->remove(1));
    bst->insert(0);
    EXPECT(bst->remove(0));
    EXPECT_NOT(bst->remove(0));
    EXIT_TEST;
}

//void

//void test_2_threads() {
    //pth
//}

//void print_array(int* a, int n) {
    //std::cout << "[";
    //for (int i = 0; i < n - 1; ++i) {
        //std::cout << a[i] << ", ";
    //}
    //if (n > 0)
        //std::cout << a[n - 1];
    //std::cout << "]\n";
//}

int main(int argc, char** argv) {

    test_single_add();
    test_single_add_remove();
    test_bad_remove();
    
    //// Perform correctness testing first.
    //srand(42);
    //std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();

    //for (int i = 0; i < N; ++i) {
        //int x = rand() % 50;
        //std::cout << "Inserting " << x << ".\n";
        //bst->insert(x);
    //}

    //int size;
    //int* inorder = bst->in_order_traversal(&size);

    //print_array(inorder, size);

    //delete inorder;
    
    return 0;
}
