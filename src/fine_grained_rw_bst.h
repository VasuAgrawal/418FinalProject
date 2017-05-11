#ifndef FINE_GRAINED_RW_BST_H_
#define FINE_GRAINED_RW_BST_H_

#include "bst.h"

#include <memory>
#include "utils.h"

class BST : public BinarySearchTree {
public:
    bool insert(int x);
    bool remove(int x);
    bool contains(int x);

    BST();
    ~BST();

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

private:
    struct Node {
        pthread_rwlock_t rwlock;
        int value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(int value);
    };

    // Dummy node to serve as the root. This should help eliminate special
    // cases in a bunch of places.
    //Node root;
    std::unique_ptr<Node> root;
};

#endif
