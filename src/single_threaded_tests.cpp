#include "single_threaded_tests.h"


// Add a single value, and check that it was added.
bool test_single_add() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(1));
    EXPECT(bst->contains(1));
    EXIT_TEST;
}


// Add three values, check that they were all added successfully.
bool test_triple_add() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(1));
    EXPECT(bst->insert(0));
    EXPECT(bst->insert(2));
    EXPECT(bst->contains(1));
    EXPECT(bst->contains(0));
    EXPECT(bst->contains(2));
    EXIT_TEST;
}


// Add a value, check it exists, and then remove it, repeatedly. This is mostly
// to check that the root add / remove is working correctly.
bool test_single_add_remove() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    for (int i = -10; i < 10; ++i) {
        EXPECT(bst->insert(i));
        EXPECT(bst->contains(i));
        EXPECT(bst->remove(i));
        EXPECT_NOT(bst->contains(i));
    }
    EXIT_TEST;
}


// Attempt to remove a value multiple times.
bool test_bad_remove() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT_NOT(bst->remove(0));
    EXPECT_NOT(bst->remove(1));
    EXPECT(bst->insert(0));
    EXPECT(bst->remove(0));
    EXPECT_NOT(bst->remove(0));
    EXIT_TEST;
}


// Add a value multiple times, check it only gets added the first time.
bool test_repeated_add() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT_NOT(bst->contains(1));
    EXPECT(bst->insert(1));
    EXPECT(bst->contains(1));
    for (int i = 0; i < 10; ++i) {
        EXPECT_NOT(bst->insert(1));
        EXPECT(bst->contains(1));
    }
    EXIT_TEST;
}


// Attempt to remove many values which don't exist.
bool test_nonexistent_remove() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    for (int i = -2; i < 10; ++i) {
        EXPECT_NOT(bst->remove(i));
    }
    EXIT_TEST;
}


// Attempt to remove a leaf node from the bst (that is not the root).
bool test_leaf_remove() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(1));
    EXPECT(bst->insert(0));

    EXPECT(bst->remove(0));

    EXPECT(bst->contains(1));
    EXPECT_NOT(bst->contains(0));
    EXIT_TEST;
}


// Attempt to remove a node with a single child
bool test_single_child_remove() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(0));
    EXPECT(bst->insert(-1));
    EXPECT(bst->insert(-2));

    EXPECT(bst->remove(-1));

    EXPECT(bst->contains(0));
    EXPECT_NOT(bst->contains(-1));
    EXPECT(bst->contains(-2));
    EXIT_TEST;
}


// Attempt to remove a node which has two children.
bool test_double_child_remove() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(0));
    EXPECT(bst->insert(2));
    EXPECT(bst->insert(1));
    EXPECT(bst->insert(3));

    EXPECT(bst->remove(2));

    EXPECT(bst->contains(0));
    EXPECT_NOT(bst->contains(2));
    EXPECT(bst->contains(1));
    EXPECT(bst->contains(3));
    EXIT_TEST;
}


// Remove the root node when it has a single child.
bool test_single_child_remove_root() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(0));
    EXPECT(bst->insert(1));

    EXPECT(bst->remove(0));

    EXPECT_NOT(bst->contains(0));
    EXPECT(bst->contains(1));
    EXIT_TEST;
}


// Remove the root when it has multiple children
bool test_double_child_remove_root() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(0));
    EXPECT(bst->insert(-1));
    EXPECT(bst->insert(2));

    EXPECT(bst->remove(0));
    
    EXPECT_NOT(bst->contains(0));
    EXPECT(bst->contains(-1));
    EXPECT(bst->contains(2));
    EXIT_TEST;
}


// Make the min of the nodes on the right go a few levels down.
bool test_double_child_remove_root_deep() {
    INIT_TEST;
    std::unique_ptr<BinarySearchTree> bst = std::make_unique<BST>();
    EXPECT(bst->insert(0));
    EXPECT(bst->insert(-1));
    EXPECT(bst->insert(10));
    EXPECT(bst->insert(20));
    EXPECT(bst->insert(5));
    EXPECT(bst->insert(8));

    EXPECT(bst->remove(0));
    
    EXPECT_NOT(bst->contains(0));
    EXPECT(bst->contains(-1));
    EXPECT(bst->contains(10));
    EXPECT(bst->contains(20));
    EXPECT(bst->contains(5));
    EXPECT(bst->contains(8));



    EXIT_TEST;

}
