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

#define GET_ADDR(nodeptr) (PTR(LNG(nodeptr) & (~(TAG_BIT | FLAG_BIT))))
#define GET_LEFT(nodeptr) (GET_ADDR(nodeptr)->left)
#define GET_RIGHT(nodeptr) (GET_ADDR(nodeptr)->right)

#define GET_TAG(nodeptr) (BL(LNG(nodeptr) & TAG_BIT))
#define GET_FLAG(nodeptr) (BL(LNG(nodeptr) & FLAG_BIT))
#define TAGGED(nodeptr) (PTR(LNG(nodeptr) | TAG_BIT))
#define FLAGGED(nodeptr) (PTR(LNG(nodeptr) | FLAG_BIT))
#define UNTAGGED(nodeptr) (PTR(LNG(nodeptr) & (~TAG_BIT)))
#define UNFLAGGED(nodeptr) (PTR(LNG(nodeptr) & (~FLAG_BIT)))

#define TAG(x) ((x) = TAGGED(x));
#define FLAG(x) ((x) = FLAGGED(x));

#define TAG_BIT 1
#define FLAG_BIT 2

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
    static bool val_leq(bool infx, int x, bool infy, int y);

private:
    struct val {
        bool inf;
        int value;
        val (int value) : inf(false), value(value) {}
        val (bool inf, int value) : inf(inf), value(value) {}

        bool operator<(const val& rhs) {
            if (rhs.inf && !inf) return true;
            else if (inf && !rhs.inf) return false;
            else return (value < rhs.value);
            // return (rhs.inf && !inf) || (value < rhs.value);
        }
        bool operator==(const val& rhs) {
            return inf == rhs.inf && value == rhs.value;
        }
        bool operator!=(const val& rhs) {
            return !(*this == rhs);
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
    bool cleanup(val value, seek_record record);

    // static void free_node(node* node);
};

#endif
