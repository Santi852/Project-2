#include "RBTree.h"
using namespace std;
RBTree::RBTree() {
    NIL = new RBNode(Car());
    NIL->color = BLACK;
    NIL->left = nullptr;
    NIL->right = nullptr;
    root = NIL;
}

void RBTree::rotateLeft(RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;

    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void RBTree::rotateRight(RBNode* x) {
    RBNode* y = x->left;
    x->left = y->right;

    if (y->right != NIL)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}
void RBTree::fixInsert(RBNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right; // uncle

            if (y->color == RED) {
                // Case 1: uncle is red — recolor
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Case 2: uncle is black, z is right child — rotate left
                    z = z->parent;
                    rotateLeft(z);
                }
                // Case 3: uncle is black, z is left child — rotate right
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(z->parent->parent);
            }
        } else {
            // Mirror of above  parent is right child
            RBNode* y = z->parent->parent->left; // uncle

            if (y->color == RED) {
                // Case 1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    // Case 2
                    z = z->parent;
                    rotateRight(z);
                }
                // Case 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}
void RBTree::insert(Car c) {
    RBNode* z = new RBNode(c);
    z->left = NIL;
    z->right = NIL;

    RBNode* y = NIL;
    RBNode* x = root;

    // find the correct position
    while (x != NIL) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == NIL)
        root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->color = RED;
    fixInsert(z);
}
void RBTree::rangeHelper(RBNode* node, double low, double high, vector<Car>& result) {
    if (node == NIL)
        return;

    if (node->key > low)
        rangeHelper(node->left, low, high, result);

    if (node->key >= low && node->key <= high)
        result.push_back(node->car);

    if (node->key < high)
        rangeHelper(node->right, low, high, result);
}

vector<Car> RBTree::rangeSearch(double low, double high) {
    vector<Car> result;
    rangeHelper(root, low, high, result);
    return result;
}