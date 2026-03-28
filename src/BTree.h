#pragma once
#include "Car.h"
#include <vector>
using namespace std;

struct BTreeNode {
    bool leaf;              //this is true when node is leaf
    vector<double> keys;        // prices
    vector<Car> cars;   // cars that match
    vector<BTreeNode*> children;    // child pointers
    BTreeNode(bool isLeaf);


};

class BTree {
    BTreeNode* root;
    int t; // this is the min degree

    void splitChild(BTreeNode* parent, int index);
    void insertNonFull(BTreeNode* node, Car c);
    void rangeHelper(BTreeNode* node, double low, double high, vector<Car> &result);

public:
    BTree();
    void insert(Car c);
    vector<Car> rangeSearch(double low, double high);



};