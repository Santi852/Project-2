#include "BTree.h"
using namespace std;

BTreeNode::BTreeNode(bool isLeaf) {
    leaf = isLeaf; // initialize the leaf status

}

BTree::BTree() {
    root = nullptr; 
    t=3; // min degree is 3
}

void BTree::splitChild(BTreeNode* parent, int index) {
    BTreeNode* y= parent->children[index]; // node spliting
    BTreeNode* z = new BTreeNode(y->leaf); // new node

    double midKey = y->keys[t-1];
    Car midCar = y->cars[t-1];

    for (int j=t; j <(int)y->keys.size(); j++) {
        z->keys.push_back(y->keys[j]); //move key to new ndoe
        z->cars.push_back(y->cars[j]);  //move car to nwew node

    }

    if (!y->leaf) {
        for (int j=t; j <(int)y->children.size(); j++) {
            z->children.push_back(y->children[j]); // move cpointer to new node
        }
    }

    y->keys.resize(t-1); // resize old node
    y->cars.resize(t-1);

    if (!y->leaf){
        y->children.resize(t); 
    }

    parent->children.insert(parent->children.begin()+ index + 1, z);
    parent->keys.insert(parent->keys.begin()+ index, midKey); // move mid key to parent
    parent->cars.insert(parent->cars.begin()+ index, midCar); // move mid car to parent
}

// inserting new car into BTree
void BTree::insertNonFull(BTreeNode* node, Car c) {
    int i =(int)node->keys.size() - 1; // start from the last key

    if (node->leaf) {
        while (i >=0 && c.price < node ->keys[i]){i--;}
        // inserting new key and car into leaf
        node->keys.insert(node->keys.begin() + i + 1, c.price); 
        node->cars.insert(node->cars.begin() + i + 1, c);
    }
    else {
        while (i >=0 && c.price < node->keys[i]){i--;} // finde the chikld to inser
        i++; 
        // if the child is full, split it
        if (node->children[i]->keys.size()==2*t-1) {
            splitChild(node, i);  

            if (c.price >= node->keys[i]){i++;}
        }
        
        insertNonFull(node->children[i],c);
    }
    }



void BTree::insert(Car c){ // insert a new car into the BTree
    if (root == nullptr){ 
        root = new BTreeNode(true);
        root ->keys.push_back(c.price);
        root ->cars.push_back(c);
        return;
    
    }
    if (root->keys.size() == 2*t-1) { // if root is full, split it
        BTreeNode* s = new BTreeNode(false);
        s->children.push_back(root);
        splitChild(s, 0);
        root = s;
    }
    insertNonFull(root, c);
}
void BTree::rangeHelper(BTreeNode* node, double low, double high, vector<Car>& result){
    if (node == nullptr)
    return;
    //  number of keys in the node
    int n = (int)node->keys.size(); // number of keys in the node

    // traverse the keys and children
    for (int i=0; i<n; i++) {
        if (!node->leaf)
            rangeHelper(node->children[i],low,high,result);
        
        if (node->keys[i] >= low && node->keys[i] <= high) 
            result.push_back(node->cars[i]);
        }
     //if last key is less than H, will check last child   
    if (!node->leaf)
        rangeHelper(node->children[n],low,high,result);
}
//search range of the price and return cars
vector<Car> BTree::rangeSearch(double low, double high) {
    vector<Car> result;
    rangeHelper(root, low, high, result);
    return result;
}
