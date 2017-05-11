#ifndef LOCK_FREE_BST_H_
#define LOCK_FREE_BST_H_

#include <atomic>
#include <vector>

#include "bst.h"

#define PTR(x) (reinterpret_cast<node*>(x))
#define LNG(x) (reinterpret_cast<unsigned long>(x))
#define BL(x)  (LNG(x) != 0)

#define PACK_CHILD(p,c,d) ((p)->d = PTR(LNG(c) | (LNG((p)->d) & 3)))
#define PACK_LEFT(p,c) PACK_CHILD(p,c,left)
#define PACK_RIGHT(p,c) PACK_CHILD(p,c,right)

#define PACK_NODE(address,src,dst) (PTR(LNG(address) | 2*src | dst))
#define UNPACK_NODE(nodeptr) (PTR(LNG(nodeptr) & (~3)))
#define UNPACK_LEFT(nodeptr) (UNPACK_NODE(nodeptr)->left)
#define UNPACK_RIGHT(nodeptr) (UNPACK_NODE(nodeptr)->right)

#define GET_TAG(nodeptr) (BL(LNG(nodeptr) & 1))
#define GET_FLAG(nodeptr) (BL((LNG(nodeptr) >> 1) & 1))
#define TAG(nodeptr) (nodeptr = PTR(LNG(nodeptr) | 1))
#define FLAG(nodeptr) (nodeptr = PTR(LNG(nodeptr) | 2))
#define UNTAG(nodeptr) (nodeptr = PTR(LNG(nodeptr) & (~1)))
#define UNFLAG(nodeptr) (nodeptr = PTR(LNG(nodeptr) & (~2)))

class BST : public BinarySearchTree {
public:
    bool insert(int x);
    bool remove(int x);
    bool contains(int x);
    std::vector<int> in_order_traversal();

    BST();
    ~BST();

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    int make_seek_test_bst(int* expected_vals);
    void do_seek_test(int x, int* got_vals);

private:
    struct val {
        bool inf;
        int value;
        val (int value) : inf(false), value(value) {}
        val (bool inf, int value) : inf(inf), value(value) {}

        bool operator<(const val& rhs) {
            return (rhs.inf && !inf) || (value < rhs.value);
        }
        bool operator==(const val& rhs) {
            return inf == rhs.inf && value == rhs.value;
        }
    };

    struct node {
        val value;
        node* left;
        node* right;

        node(val value) : value(value), left(nullptr), right(nullptr) {}
        node(val value, node* left, node* right) :
            value(value), left(left), right(right) {}
    };

    struct seek_record {
        node* ancestor;
        node* successor;
        node* parent;
        node* leaf;
    };

    node* root;

    seek_record seek(val value);
    void cleanup(val value, seek_record record);
    
    static void free_node(node* node);
};

#endif
