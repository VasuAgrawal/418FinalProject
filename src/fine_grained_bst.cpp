#include "fine_grained_bst.h"

#include <iostream>

BST::BST() : root(nullptr) {
    pthread_mutex_init(&lock, nullptr);
}

BST::~BST() {
    pthread_mutex_destroy(&lock);
}


void BST::insert(int value) {
    std::cout << "Insert called with " << value << std::endl;
    pthread_mutex_lock(&lock);
    if (root == nullptr) { // Adding the first element
        root = std::make_unique<Node>(value);
        std::cout << "Adding " << value << " to tree!" << std::endl;
        ++size;
        pthread_mutex_unlock(&lock);
        return;
    }
    
    // Otherwise, the element already exists, so traverse the tree until we find
    // the place to add it.
    pthread_mutex_lock(&root->lock); 
    pthread_mutex_unlock(&lock);

    Node* ptr = root.get();
    Node* next = nullptr;

    while (true) {
        if (value < ptr->value) {
            if (ptr->left == nullptr) {
                ptr->left = std::make_unique<Node>(value);
                std::cout << "Adding " << value << " to tree!" << std::endl;
                pthread_mutex_unlock(&ptr->lock);
                return;
            }  else {
                next = ptr->left.get();
            }
        } else if (value > ptr->value) {
            if (ptr->right == nullptr) {
                ptr->right = std::make_unique<Node>(value);
                std::cout << "Adding " << value << " to tree!" << std::endl;
                pthread_mutex_unlock(&ptr->lock);
                return;
            } else {
                next = ptr->right.get();
            }
        } else { // Equal, don't do anything.
            pthread_mutex_unlock(&ptr->lock);
            return;
        }
            
        pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&ptr->lock);
        ptr = next;
    }
}

bool BST::remove(int value) {
    std::cout << "Remove called with " << value << std::endl;
    return false;
}

bool BST::contains(int value) {
    std::cout << "Contains called with " << value << std::endl;
    return false;
}

int* BST::in_order_traversal(int* size) {
    (void)size;

    std::cout << "In order traversal called!" << std::endl;
    return nullptr;
}

BST::Node::Node(int value) : value(value) {
}
