#ifndef LOCK_FREE_BST_H_
#define LOCK_FREE_BST_H_

#include <atomic>
#include <vector>

#include "bst.h"

#define PACK_CHILD(p,c,d) ((p)->d = c | ((p)->d & 3))
#define PACK_LEFT(p,c) PACK_CHILD(p,c,left)
#define PACK_RIGHT(p,c) PACK_CHILD(p,c,right)

class BST : public BinarySearchTree {
public:
    bool insert(int x);
    bool remove(int x);
    bool contains(int x);
    std::vector<int> in_order_traversal();

    BST();
    ~BST();

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

private:
    struct node {
        int value;
        std::atomic<node*> left;
        std::atomic<node*> right;
    };
};

#endif
