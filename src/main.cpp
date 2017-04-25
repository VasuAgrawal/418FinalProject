#include <iostream>

#include "bst.h"

#ifdef COARSE
#include "coarse_grained_bst.h"
#elif FINE
#include "fine_grained_bst.h"
#else
#error Implementation not specified. Valid implementations are "COARSE", "FINE".
#endif

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

    BinarySearchTree* bst = new BST();

    for (int i = 0; i < N; ++i) {
        int x = rand() % 50;
        std::cout << "Inserting " << x << ".\n";
        bst->insert(x);
    }

    int size;
    int* inorder = bst->in_order_traversal(&size);

    print_array(inorder, size);

    delete inorder;
    delete bst;

    return 0;
}
