#ifndef BST_H_
#define BST_H_

#include <pthread.h>

class BinarySearchTree {
public:
    virtual void insert(int x) = 0;
    virtual bool remove(int x) = 0;
    virtual bool contains(int x) = 0;

    // returns the in-order traversal, stores the number of nodes in size
    virtual int* in_order_traversal(int* size) = 0;
};

class CoarseGrainedBST : BinarySearchTree {
public:
    void insert(int x);
    bool remove(int x);
    bool contains(int x);
    int* in_order_traversal(int* size);

    CoarseGrainedBST();

private:
    struct node {
        int value;
        node* left;
        node* right;
    };

    pthread_mutex_t bst_lock;
    node* root;

    static void insert_node(node* current_node, int val, node* new_node);
    static bool search_node(node* current_node, int val);
};

class FineGrainedBST : BinarySearchTree {
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