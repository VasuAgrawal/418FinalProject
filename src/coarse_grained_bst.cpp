#include <iostream>

#include "coarse_grained_bst.h"

BST::BST() : root(nullptr) {
    pthread_mutex_init(&bst_lock, nullptr);
}

void BST::free_node(node* current_node) {
    if (current_node == nullptr)
        return;
    free_node(current_node->left);
    free_node(current_node->right);
    delete current_node;
}

BST::~BST() {
    free_node(root);
}

bool BST::insert_node(
        node* current_node, int val, node* new_node) {
    if (val < current_node->value) {
        if (current_node->left == nullptr) {
            current_node->left = new_node;
            return true;
        } else {
            return insert_node(current_node->left, val, new_node);
        }
    } else if (val > current_node->value) {
        if (current_node->right == nullptr) {
            current_node->right = new_node;
            return true;
        } else {
            return insert_node(current_node->right, val, new_node);
        }
    }
    return false;
}

void BST::insert(int x) {
    pthread_mutex_lock(&bst_lock);

    node* new_node = new node;
    new_node->value = x;
    new_node->left = nullptr;
    new_node->right = nullptr;

    if (root == nullptr) {
        root = new_node;
        ++size;
    } else if (insert_node(root, x, new_node)) {
        ++size;
    }

    pthread_mutex_unlock(&bst_lock);
}

bool BST::remove(int x) {
    pthread_mutex_lock(&bst_lock);

    //TODO

    pthread_mutex_unlock(&bst_lock);
    return false;
}

bool BST::search_node(node* current_node, int val) {
    if (current_node == nullptr)
        return false;
    else if (val == current_node->value)
        return true;
    else if (val < current_node->value)
        return search_node(current_node->left, val);
    else
        return search_node(current_node->right, val);
}

bool BST::contains(int x) {
    pthread_mutex_lock(&bst_lock);

    bool result = search_node(root, x);

    pthread_mutex_unlock(&bst_lock);
    return result;
}

int BST::fill_inorder(node* current_node, int* out, int i) {
    if (current_node == nullptr)
        return i;
    i = fill_inorder(current_node->left, out, i);
    out[i] = current_node->value;
    return fill_inorder(current_node->right, out, ++i);
}

int* BST::in_order_traversal(int* size) {
    pthread_mutex_lock(&bst_lock);

    int* out = new int[this->size];
    fill_inorder(root, out, 0);

    pthread_mutex_unlock(&bst_lock);
    *size = this->size;
    return out;
}
