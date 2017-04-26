#ifndef COARSE_GRAINED_BST_H_
#define COARSE_GRAINED_BST_H_

#include "bst.h"

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
    struct node {
        int value;
        node* left;
        node* right;
    };

    enum direction { LEFT, RIGHT };

    pthread_mutex_t bst_lock;
    node* root;

    bool remove_node(node* current_node, node* parent, direction d, int val);

    static int find_min(node* n);
    static int num_children(node* n);
    static void free_node(node* current_node);
    static bool insert_node(node* current_node, int val, node* new_node);
    static bool search_node(node* current_node, int val);
    static int fill_inorder(node* current_node, int* out, int i);
};

#endif
