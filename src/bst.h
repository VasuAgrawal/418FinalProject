#ifndef BST_H_
#define BST_H_

#include <pthread.h>

class BinarySearchTree {
public:
    
    BinarySearchTree() : size(0) {};
    virtual ~BinarySearchTree() = default;

    virtual bool insert(int x) = 0;
    virtual bool remove(int x) = 0;
    virtual bool contains(int x) = 0;

    //// returns the in-order traversal, stores the number of nodes in size
    //virtual int* in_order_traversal(int* size) = 0;
protected:
    int size;
};

#endif
