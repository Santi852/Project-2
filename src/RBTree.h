
#pragma once
#include "Car.h"
#include <vector>
using namespace std;

enum Color { RED, BLACK };

struct RBNode {
    Car car;
    double key;       // price
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    RBNode(Car c) : car(c), key(c.price), color(RED),
                    left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
    RBNode* root;
    RBNode* NIL; 

    void rotateLeft(RBNode* x);
    void rotateRight(RBNode* x);
    void fixInsert(RBNode* z);
    void rangeHelper(RBNode* node, double low, double high, vector<Car>& result);

public:
    RBTree();
    void insert(Car c);
    vector<Car> rangeSearch(double low, double high);
};