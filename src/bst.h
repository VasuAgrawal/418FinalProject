#ifndef BST_H_
#define BST_H_

class BinarySearchTree {
public:
    virtual void insert(int x) = 0;
    virtual bool remove(int x) = 0;
    virtual bool contains(int x) = 0;
    virtual int in_order_traversal(int* out) = 0;
};

#endif