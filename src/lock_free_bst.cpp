#include <iostream>
#include <vector>
#include <queue>
#include <stack>

#include "lock_free_bst.h"

#define HERE std::cout << "Got to line " << __LINE__ << std::endl

BST::BST() {
    val inf0(true, 0);
    val inf1(true, 1);
    val inf2(true, 2);
    node* R = new node(inf2);
    node* S = new node(inf1);
    R->left = S;
    R->right = new node(inf2);
    S->left = new node(inf0);
    S->right = new node(inf1);

    root = R;
}

BST::~BST() {
    free_node(root);
}

void BST::free_node(node* current_node) {
    std::queue<node*> nodes;
    nodes.push(current_node);
    while (!nodes.empty()) {
        current_node = nodes.front(); nodes.pop();
        if (current_node == nullptr) continue;
        nodes.push(UNPACK_NODE(current_node->left));
        nodes.push(UNPACK_NODE(current_node->right));
        delete current_node;
    }
}

BST::seek_record BST::seek(val value) {

    seek_record record;
    record.ancestor = root;
    node* S = root;
    record.successor = S;
    record.parent = S;
    record.leaf = UNPACK_NODE(UNPACK_LEFT(S));

    node* parentField = record.parent;
    node* currentField = record.leaf;
    node* current = UNPACK_NODE(currentField);

    while (current != nullptr) {
        if (!GET_TAG(parentField)) {
            record.ancestor = record.parent;
            record.successor = record.leaf;
        }
        record.parent = record.leaf;
        record.leaf = current;

        parentField = currentField;
        if (value < current->value) {
            currentField = UNPACK_LEFT(current);
        } else {
            currentField = UNPACK_RIGHT(current);
        }
        current = UNPACK_NODE(currentField);
    }
    return record;
}

bool BST::insert(int x) {
    // std::cout << "Inserting: " << x << std::endl;
    val v(x);
    seek_record record;
    while (true) {
        record = seek(v);
        if (record.leaf->value == v) {
            // x is in the tree already
            return false;
        } else {
            node* parent = record.parent;
            node* leaf = record.leaf;

            node* new_leaf = new node(v);
            val k = leaf->value;
            node* new_internal;
            if (k < v) {
                new_internal = new node(v, leaf, new_leaf);
            } else {
                new_internal = new node(k, new_leaf, leaf);
            }

            node** child_addr;
            if (v < parent->value) {
                child_addr = &(parent->left);
            } else {
                child_addr = &(parent->right);
            }

            if (__sync_bool_compare_and_swap(child_addr, leaf, new_internal)) {
                // std::cout << "  Inserted new_internal: " << new_internal->value.value <<
                //            "\n               new_leaf: " << new_leaf->value.value << "\n";
                return true;
            } else {
                // TODO: cleanup
                delete new_leaf;
                delete new_internal;
            }
        }
    }
}

bool BST::remove(int x) {
    val v(x);
    return false;
}

bool BST::contains(int x) {
    val v(x);
    seek_record record = seek(v);
    return record.leaf->value == v;
}

std::vector<int> BST::in_order_traversal() {
    std::vector<int> out;
    std::stack<node*> nodes;
    nodes.push(root);
    node* current_node;
    while(!nodes.empty()) {
        current_node = nodes.top(); nodes.pop();
        if (UNPACK_NODE(current_node->left) == nullptr) {
            if (!current_node->value.inf)
                out.push_back(current_node->value.value);
        } else {
            nodes.push(current_node->right);
            nodes.push(current_node->left);
        }
    }
    return out;
}

void BST::cleanup(val value, seek_record record) {}

void BST::do_seek_test(int x, int* got_vals) {
    val v(x);
    seek_record record = seek(v);
    got_vals[0] = ((record.ancestor)->value).value;
    got_vals[1] = ((record.successor)->value).value;
    got_vals[2] = ((record.parent)->value).value;
    got_vals[3] = ((record.leaf)->value).value;
}

int BST::make_seek_test_bst(int* expected_vals) {
    // emulating Figure 2 from http://dl.acm.org/citation.cfm?id=2555256

    val a(0);
    val A(1);
    val b(2);
    val B(3);
    val c(10);
    val C(11);
    val D(5);
    val E(7);
    val F(9);
    val G(8);
    val H(12);
    val I(4);
    val J(6);

    node* nG = new node(G);
    node* nc = new node(c);
    node* nJ = new node(J);
    node* nF = new node(F, nG, nc);
    node* nE = new node(E, nJ, nF);
    node* nI = new node(I);
    node* nD = new node(D, nI, nE);
    node* nH = new node(H);
    node* nb = new node(b);
    node* nC = new node(C, nD, nH);
    node* na = new node(a);
    node* nB = new node(B, nb, nC);
    node* nA = new node(A, na, nB);

    FLAG(nC->right);
    TAG(nC->left);
    FLAG(nD->left);
    TAG(nD->right);
    FLAG(nE->left);
    TAG(nE->right);

    node* S = UNPACK_LEFT(root);
    node* sent = UNPACK_LEFT(S);
    PACK_LEFT(sent, nA);

    expected_vals[0] = B.value;
    expected_vals[1] = C.value;
    expected_vals[2] = F.value;
    expected_vals[3] = G.value;

    return G.value;
}
