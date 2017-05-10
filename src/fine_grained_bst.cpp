#include "fine_grained_bst.h"

#include <iostream>

BST::BST() : root(nullptr) {
    pthread_mutex_init(&lock, nullptr);
}

BST::~BST() {
    pthread_mutex_destroy(&lock);
}


bool BST::insert(int value) {
    // Always lock the bst object first, before making any changes.
    pthread_mutex_lock(&lock);

    if (root == nullptr) { // Add first element to the list
        root = std::make_unique<Node>(value);
        //std::cout << "Adding " << value << " to tree as root!" << std::endl;
        pthread_mutex_unlock(&lock);
        return true;
    }

    pthread_mutex_lock(&root->lock);
    pthread_mutex_unlock(&lock);

    Node* curr = root.get();
    Node* next = nullptr;

    while (true) {
        if (value < curr->value) { // Search on the left of this subtree
            if (curr->left == nullptr) {
                curr->left = std::make_unique<Node>(value);
                pthread_mutex_unlock(&curr->lock);
                //std::cout << "Successfully added " << value << " to tree!" << std::endl;
                return true;
            } else { // A node exists on the left, so "recursively" add from there
                next = curr->left.get();
            }
        } else if (value > curr->value) {
            if (curr->right == nullptr) {
                curr->right = std::make_unique<Node>(value);
                pthread_mutex_unlock(&curr->lock);
                //std::cout << "Successfully added " << value << " to tree!" << std::endl;
                return true;
            } else {
                next = curr->right.get();
            }
        } else {
            //std::cout << "Value " << value << " is already in the tree!" << std::endl;
            pthread_mutex_unlock(&curr->lock);
            return false;
        }

        pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&curr->lock);
        curr = next;
    }
}


bool BST::contains(int value) {
    //std::cout << "Contains called with " << value << std::endl;

    // Always lock the bst object first, before doing anything to it.
    pthread_mutex_lock(&lock);
    if (root == nullptr) {
        pthread_mutex_unlock(&lock);
        return false;
    }

    pthread_mutex_lock(&root->lock);
    pthread_mutex_unlock(&lock);

    Node* curr = root.get();
    Node* next = nullptr;

    // We're sure that the root exists, so we can start searching from it.
    while (true) {
        if (value == curr->value) {
            //std::cout << "Value " << value << "found!" << std::endl;
            pthread_mutex_unlock(&curr->lock);
            return true;
        } else if (value < curr->value) {
            // Either the value is in the left tree, or not here.
            if (curr->left == nullptr) {
                pthread_mutex_unlock(&curr->lock);
                return false;
            } else {
                next = curr->left.get();
            }
        } else { // value > curr->value
            if (curr->right == nullptr) {
                pthread_mutex_unlock(&curr->lock);
                return false;
            } else {
                next = curr->right.get();
            }
        }

        pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&curr->lock);
        curr = next;
    }

    // Uhhh ....
    return false;
}


bool BST::remove(int value) {
    std::cout << "Remove called with " << value << std::endl;
    return false;
}


BST::Node::Node(int value) : value(value) {
    pthread_mutex_init(&lock, nullptr);
}
