#include "single_threaded_tests.h"


bool test_single_add() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(1));
    EXPECT(bst->contains(1));
    EXIT_TEST;
}

bool test_triple_add() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    bst->insert(1);
    bst->insert(0);
    bst->insert(2);
    EXPECT(bst->contains(1));
    EXPECT(bst->contains(0));
    EXPECT(bst->contains(2));
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

bool test_repeated_add() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    for (int i = 0; i < 10; ++i) {
        bst->insert(1);
        EXPECT(bst->contains(1));
    }

    bst->remove(1);
    EXPECT_NOT(bst->contains(1));
    EXIT_TEST;
}
