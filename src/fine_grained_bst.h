#ifndef FINE_GRAINED_BST_H_
#define FINE_GRAINED_BST_H_

#include "bst.h"

class BST : public BinarySearchTree {
public:
    void insert(int x);
    bool remove(int x);
    bool contains(int x);
    int* in_order_traversal(int* size);

    FineGrainedBST();

private:
    struct node {
        pthread_mutex_t lock;
        int value;
        node* left;
        node* right;
    };

    node* root;
};

#endif
