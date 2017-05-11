#include "fine_grained_rw_bst.h"

#include <iostream>

#define HERE std::cout << "Got to line " << __LINE__ << std::endl;

BST::BST() {
    root = std::make_unique<Node>(0);
}

BST::~BST() {
}

bool BST::insert(int value) {
    pthread_rwlock_wrlock(&root->rwlock);

    if (root->right == nullptr) { // Arbitrarily pick right.
        root->right = std::make_unique<Node>(value);
        pthread_rwlock_unlock(&root->rwlock);
        return true;
    }

    // Otherwise, we know that there is an element in the tree, so we perform
    // basically the same insert as before.
    pthread_rwlock_wrlock(&root->right->rwlock);
    pthread_rwlock_unlock(&root->rwlock);

    Node* curr = root->right.get();
    Node* next = nullptr;

    while (true) {
        if (value == curr->value) { // Already in tree!
            pthread_rwlock_unlock(&curr->rwlock);
            return false;
        } else if (value < curr->value) {
            if (curr->left == nullptr) {
                curr->left = std::make_unique<Node>(value);
                pthread_rwlock_unlock(&curr->rwlock);
                return true;
            } else {
                next = curr->left.get();
            }
        } else { // value > curr->value
            if (curr->right == nullptr) {
                curr->right = std::make_unique<Node>(value);
                pthread_rwlock_unlock(&curr->rwlock);
                return true;
            } else {
                next = curr->right.get();
            }
        }

        pthread_rwlock_wrlock(&next->rwlock);
        pthread_rwlock_unlock(&curr->rwlock);
        curr = next;
    }
}


bool BST::contains(int value) {
    // Always start by locking the root
    pthread_rwlock_rdlock(&root->rwlock);

    if (root->right == nullptr) {
        pthread_rwlock_unlock(&root->rwlock);
        return false;
    }

    pthread_rwlock_rdlock(&root->right->rwlock);

    Node* curr = root->right.get();
    Node* next = nullptr;

    pthread_rwlock_unlock(&root->rwlock);

    // We're sure that the "root" exists, so start searching from it.
    while (true) {
        if (value == curr->value) { // Found it!
            pthread_rwlock_unlock(&curr->rwlock);
            return true;
        } else if (value < curr->value) {
            if (curr->left == nullptr) {
                pthread_rwlock_unlock(&curr->rwlock);
                return false;
            }  else {
                next = curr->left.get();
            }
        } else { // value > curr->value
            if (curr->right == nullptr) {
                pthread_rwlock_unlock(&curr->rwlock);
                return false;
            } else {
                next = curr->right.get();
            }
        }

        pthread_rwlock_rdlock(&next->rwlock);
        pthread_rwlock_unlock(&curr->rwlock);
        curr = next;
    }
}


bool BST::remove(int value) {
    // Start by locking the root node.
    pthread_rwlock_wrlock(&root->rwlock);
    if (root->right == nullptr) { // Nothing in the tree
        pthread_rwlock_unlock(&root->rwlock);
        return false;
    }

    pthread_rwlock_wrlock(&root->right->rwlock);

    // Otherwise, at least the root exists. We can lock the current and parent
    // easily, since the parent can start off as a dummy node.
    Node* parent = root.get();
    Node* curr = root->right.get();
    Node* next = nullptr;

    while (true) {
        if (value < curr->value) {
            if (curr->left == nullptr) {
                pthread_rwlock_unlock(&curr->rwlock);
                pthread_rwlock_unlock(&parent->rwlock);
                return false;
            } else {
                next = curr->left.get();
            }
        } else if (value > curr->value) {
            if (curr->right == nullptr) {
                pthread_rwlock_unlock(&curr->rwlock);
                pthread_rwlock_unlock(&parent->rwlock);
                return false;
            } else {
                next = curr->right.get();
            }
        } else { // value == curr->value (remove curr)
            if (curr->left == nullptr && curr->right == nullptr) { // 0 children
                if (curr == parent->left.get()) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }

                pthread_rwlock_unlock(&parent->rwlock);
                return true;
            } else if (curr->left == nullptr || curr->right == nullptr) { // 1
                if (curr == parent->left.get()) {
                    parent->left = std::move(curr->left == nullptr ? curr->right : curr->left);
                } else {
                    parent->right = std::move(curr->left == nullptr ? curr->right : curr->left);
                }

                pthread_rwlock_unlock(&parent->rwlock);
                return true;
            } else { // children on either side
                // To do two child removal, we pick the right subtree and then
                // find the minimum element on that subtree. The node is then
                // deleted, and the value from that node is placed in the
                // current node to avoid moving things around.
                Node* curr_parent = curr;
                Node* curr_min = curr->right.get();
                pthread_rwlock_wrlock(&curr_min->rwlock);
                pthread_rwlock_unlock(&parent->rwlock);

                // We'll always have a lock on curr, curr_parent, and curr_min.
                while (true) {
                    if (curr_min->left == nullptr) { // Minimum element
                        curr->value = curr_min->value;
                        if (curr_parent == curr) {
                            curr_parent->right = curr_min->right == nullptr ? 
                                nullptr : std::move(curr_min->right);
                        } else {
                            curr_parent->left = curr_min->right == nullptr ? 
                                nullptr : std::move(curr_min->right);
                            pthread_rwlock_unlock(&curr_parent->rwlock);
                        }

                        pthread_rwlock_unlock(&curr->rwlock);
                        return true;
                    }

                    // If we haven't found the minimum element, continue
                    // traversing.
                    pthread_rwlock_wrlock(&curr_min->left->rwlock);
                    if (curr_parent != curr) {
                        pthread_rwlock_unlock(&curr_parent->rwlock);
                    }
                    curr_parent = curr_min;
                    curr_min = curr_min->left.get();
                }
            }
        }

        pthread_rwlock_wrlock(&next->rwlock);
        pthread_rwlock_unlock(&parent->rwlock);
        parent = curr;
        curr = next;
    }
}


BST::Node::Node(int value) : value(value) {
    pthread_rwlock_init(&rwlock, nullptr);
}
