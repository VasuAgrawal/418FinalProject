#include "bst.h"

CoarseGrainedBST::CoarseGrainedBST () {
    root = nullptr;
    pthread_mutex_init(&bst_lock, nullptr);
}

void CoarseGrainedBST::insert_node(
        node* current_node, int val, node* new_node) {
    if (val < current_node->value) {
        if (current_node->left == nullptr)
            current_node->left = new_node;
        else
            insert_node(current_node->left, val, new_node);
    } else if (val > current_node->value) {
        if (current_node->right == nullptr)
            current_node->right = new_node;
        else
            insert_node(current_node->right, val, new_node);
    }
}

void CoarseGrainedBST::insert(int x) {
    pthread_mutex_lock(&bst_lock);

    node* new_node = new node;
    new_node->value = x;
    new_node->left = nullptr;
    new_node->right = nullptr;

    if (root == nullptr) {
        root = new_node;
    } else {
        insert_node(root, x, new_node);
    }

    pthread_mutex_unlock(&bst_lock);
}

bool CoarseGrainedBST::remove(int x) {
    pthread_mutex_lock(&bst_lock);

    //TODO

    pthread_mutex_unlock(&bst_lock);
    return false;
}

bool CoarseGrainedBST::search_node(node* current_node, int val) {
    if (current_node == nullptr)
        return false;
    else if (val == current_node->value)
        return true;
    else if (val < current_node->value)
        return search_node(current_node->left, val);
    else
        return search_node(current_node->right, val);
}

bool CoarseGrainedBST::contains(int x) {
    pthread_mutex_lock(&bst_lock);

    bool result = search_node(root, x);

    pthread_mutex_unlock(&bst_lock);
    return result;
}

int* CoarseGrainedBST::in_order_traversal(int* size) {
    pthread_mutex_lock(&bst_lock);

    //TODO

    pthread_mutex_unlock(&bst_lock);
    *size = 0;
    return nullptr;
}