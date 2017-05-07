#ifndef FINE_GRAINED_BST_H_
#define FINE_GRAINED_BST_H_

#include "bst.h"

#include <memory>

class BST : public BinarySearchTree {
public:
    void insert(int x);
    bool remove(int x);
    bool contains(int x);
    int* in_order_traversal(int* size);

    BST();
    ~BST();

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

private:
    struct Node {
        pthread_mutex_t lock;
        int value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(int value);
    };

    std::unique_ptr<Node> root;
    pthread_mutex_t lock;
};

#endif
