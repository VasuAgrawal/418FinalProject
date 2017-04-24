#include "bst.h"

CoarseGrainedBST::CoarseGrainedBST () {
    root = nullptr;
    pthread_mutex_init(&bst_lock, nullptr);
}

void CoarseGrainedBST::insert(int x) {
    pthread_mutex_lock(&bst_lock);

    //TODO

    pthread_mutex_unlock(&bst_lock);
}

bool CoarseGrainedBST::remove(int x) {
    pthread_mutex_lock(&bst_lock);

    //TODO

    pthread_mutex_unlock(&bst_lock);
    return false;
}

bool CoarseGrainedBST::contains(int x) {
    pthread_mutex_lock(&bst_lock);

    //TODO

    pthread_mutex_unlock(&bst_lock);
    return false;
}

int* CoarseGrainedBST::in_order_traversal(int* size) {
    pthread_mutex_lock(&bst_lock);

    //TODO

    pthread_mutex_unlock(&bst_lock);
    *size = 0;
    return nullptr;
}