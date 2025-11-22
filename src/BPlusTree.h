#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Word.h"

class BPlusTree {
public:
    struct Node {
        bool isLeaf;
        std::vector<std::string> keys; // keys (romaji strings) stored in the node
        std::vector<Node*> children; // only for internal nodes
        std::vector<Word*> values; // only for leaf nodes
        Node* next; // pointer to the next leaf node
        Node(bool leaf = false) : isLeaf(leaf), next(nullptr) {}; // node constructor
    };
    Node* root;
    int t; // minimum degree (defines the range for number of keys)
    
    /*======== Constructor ========*/
    BPlusTree(int degree) : root(nullptr), t(degree) {} // with default degree 

    /*======== Destructor ========*/
    ~BPlusTree() {freeNode(root);} // to free memory

    /*======== Operations ========*/
    void insert(Word* word); // insert a word into the B+ tree
    std::vector<Word*> prefixSearch(const std::string& prefix); // search for words with a given romaji prefix
    Word* exactSearch(const std::string& key); // search for a word with an exact romaji match

private: 
    void freeNode(Node* node) {
        if (!node) return;

        // if not a leaf, delete children
        if (!node->isLeaf) {
            for (Node* child : node->children) {
                freeNode(child);
            }
        }
        delete node;
    }

    /*======== Helper Functions ========*/
    void splitChild(Node* parent, int ind, Node* child);
    void insertNonFull(Node* node, const std::string& key, Word* word);
    std::string nextPrefix(const std::string& prefix);
    std::vector<Word*> rangeSearch(const std::string& lower, const std::string& upper);
};
