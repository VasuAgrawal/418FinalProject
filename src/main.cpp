#include <iostream>

#include "bst.h"

#define N 20

void print_array(int* a, int n) {
    std::cout << "[";
    for (int i = 0; i < n - 1; ++i) {
        std::cout << a[i] << ", ";
    }
    if (n > 0)
        std::cout << a[n - 1];
    std::cout << "]\n";
}

int main(int argc, char** argv) {
    srand(42);

    CoarseGrainedBST cg_bst;

    for (int i = 0; i < N; ++i) {
        int x = rand() % 50;
        std::cout << "Inserting " << x << ".\n";
        cg_bst.insert(x);
    }

    int size;
    int* inorder = cg_bst.in_order_traversal(&size);

    print_array(inorder, size);

    delete inorder;

    return 0;
}