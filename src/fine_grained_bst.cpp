#include "fine_grained_bst.h"
#include "CycleTimer.h"

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
                return true;
            } else { // A node exists on the left, so "recursively" add from there
                next = curr->left.get();
            }
        } else if (value > curr->value) {
            if (curr->right == nullptr) {
                curr->right = std::make_unique<Node>(value);
                pthread_mutex_unlock(&curr->lock);
                return true;
            } else {
                next = curr->right.get();
            }
        } else {
            pthread_mutex_unlock(&curr->lock);
            return false;
        }

        pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&curr->lock);
        curr = next;
    }
}


bool BST::contains(int value) {
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
}


// Removes the root, assuming that there's already a lock on the root and the
// object. Will clean up all locks as necessary.
bool BST::remove_root() {
    // Root is the only thing in the tree, so set back to nullptr.
    if (root->left == nullptr && root->right == nullptr) {
        pthread_mutex_unlock(&root->lock);
        root = nullptr;
        pthread_mutex_unlock(&lock);
        return true;
    }

    // Root only has a single child
    if (root->left == nullptr || root->right == nullptr) {
        // TODO: Might need to unlock the mutex before deleting it?
        root = std::move(root->right == nullptr ? root->left : root->right); 
        pthread_mutex_unlock(&lock);
        return true;
    }

    // Root has multiple children. Go through and find the smallest element
    // in the right subtree, delete it, and use that value as the new root.
    Node* parent = root.get();
    Node* curr = root->right.get();
    pthread_mutex_lock(&curr->lock);
    pthread_mutex_unlock(&lock);

    while (true) {
        if (curr->left == nullptr) { // Minimum element
            root->value = curr->value;
            if (parent == root.get()) {
                parent->right = curr->right == nullptr ? nullptr : std::move(curr->right);
            } else {
                parent->left = curr->right == nullptr ? nullptr : std::move(curr->right);
                pthread_mutex_unlock(&parent->lock);
            }

            pthread_mutex_unlock(&root->lock);
            return true;
        }

        // If we haven't found the minimum element, continue traversing.
        pthread_mutex_lock(&curr->left->lock);
        if (parent != root.get()) {
            pthread_mutex_unlock(&parent->lock);
        }
        parent = curr;
        curr = curr->left.get();
    }
}


bool BST::remove(int value) {

    // Always lock the object itself first, before making any changes.
    pthread_mutex_lock(&lock);
    if (root == nullptr) { // Nothing to delete
        pthread_mutex_unlock(&lock);
        return false;
    } 
    
    pthread_mutex_lock(&root->lock);
    if (root->value == value) { // root is being deleted
        return remove_root();
    }

    // At this point, we're sure the root isn't the one being deleted. There is
    // still a lock on the root and the object, but since the root certainly
    // isn't going to be modified the lock on the object isn't necessary
    // anymore.
    pthread_mutex_unlock(&lock);

    // Now we go through the tree and do the same thing we were doing earlier.
    Node* curr = root.get();
    Node* parent = nullptr;
    Node* next = nullptr;

    // At any point here, we'll have a lock on curr and parent.
    while (true) {
        if (value < curr->value) {
            if (curr->left == nullptr) { // The value doesn't exist
                pthread_mutex_unlock(&curr->lock);
                if (parent) {
                    pthread_mutex_unlock(&parent->lock);
                }
                return false;
            } else {
                next = curr->left.get();
            }

        } else if (value > curr->value) {
            if (curr->right == nullptr) { // The value doesn't exist
                pthread_mutex_unlock(&curr->lock);
                if (parent) {
                    pthread_mutex_unlock(&parent->lock);
                }
                return false;
            } else {
                next = curr->right.get();
            }

        } else { // value == curr->value (remove curr).
            if (curr->left == nullptr && curr->right == nullptr) {
                // No children
                if (curr == parent->left.get()) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }

                if (parent) {
                    pthread_mutex_unlock(&parent->lock);
                }
                return true;

            } else if (curr->left == nullptr || curr->right == nullptr) {
                // One child
                if (curr == parent->left.get()) {
                    parent->left = std::move(curr->left == nullptr ? curr->right : curr->left);
                } else {
                    parent->right = std::move(curr->left == nullptr ? curr->right : curr->left);
                }
    
                if (parent) {
                    pthread_mutex_unlock(&parent->lock);
                }
                return true;

            } else {
                // Two children
                // To do the two child replacement, in a manner consistent with
                // the root case above, we pick the right subtree and then find
                // the minimum element on the subtree. That node is deleted, and
                // the current node's value is replaced with that value so no
                // pointers need to be moved around.
               
                Node* curr_parent = curr;
                Node* curr_min = curr->right.get();
                pthread_mutex_lock(&curr_min->lock);
                pthread_mutex_unlock(&parent->lock);

                // Now we'll always have a lock on curr, parent (which may or
                // may not be curr), and curr_min.
                while (true) {
                    if (curr_min->left == nullptr) { // Minimum element
                        curr->value = curr_min->value;
                        if (curr_parent == curr) {
                            curr_parent->right = curr_min->right == nullptr ? nullptr : std::move(curr_min->right);
                        } else {
                            curr_parent->left = curr_min->right == nullptr ? nullptr : std::move(curr_min->right);
                            pthread_mutex_unlock(&curr_parent->lock);
                        }

                        pthread_mutex_unlock(&curr->lock);
                        return true;
                    }

                    // If we haven't found the minimum element, continue
                    // traversing.
                    pthread_mutex_lock(&curr_min->left->lock);
                    if (curr_parent != curr) {
                        pthread_mutex_unlock(&curr_parent->lock);
                    }
                    curr_parent = curr_min;
                    curr_min = curr_min->left.get();
                }
            }
        }

        pthread_mutex_lock(&next->lock);
        if (parent) {
            pthread_mutex_unlock(&parent->lock);
        }
        parent = curr;
        curr = next;
    }
}


BST::Node::Node(int value) : value(value) {
    pthread_mutex_init(&lock, nullptr);
}
