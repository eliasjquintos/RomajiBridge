#include <algorithm>
#include <vector>
#include "BPlusTree.h"
#include "Word.h"

/* Description: B+ Tree Implementation with C++
 * Author: Geeks for Geeks
 * Date: 07/23/2025
 * URL: https://www.geeksforgeeks.org/cpp/cpp-program-to-implement-b-plus-tree/
 *
 * Credits to this article for helping me understand proper implementation of B+ tree.
 * The code was referenced in order to fit the needs of this project, such as performing exact search and prefix search.
 *
*/


/* Description: Insertion in a B+ Tree
 * Author: Geeks for Geeks
 * Date: 07/15/2025
 * URL: https://www.geeksforgeeks.org/dsa/insertion-in-a-b-tree/
 *
 * Credits to this article for helping me understand how to insert keys with multiple values in a B+ tree.
 * The code was referenced in order to fit the needs of this project, specifically with the insert, insertNonFull, exactSearch, splitChild, and rangeSearch functions.
 *
*/

/*======== Operations ========*/
void BPlusTree::insert(Word* word) {
    std::string key = word->getRomaji();

    // if tree is empty, create root
    if (root == nullptr) {
        root = new Node(true);
        root->keys.push_back(key);
        root->values.push_back({ word }); // handle duplicate keys by appending to vector
        return;
    }

    // if root is full, split
    if (root->keys.size() == static_cast<size_t>(2 * t - 1)) { // changed to size_t instead of int
        Node* newRoot = new Node(false);
        newRoot->children.push_back(root);
        splitChild(newRoot, 0, root);
        root = newRoot;
    }

    insertNonFull(root, key, word);
}

std::vector<Word*> BPlusTree::prefixSearch(const std::string& prefix) {
    std::string lower = prefix;
    std::string upper = nextPrefix(prefix);
    return rangeSearch(lower, upper);
}

Word* BPlusTree::exactSearch(const std::string& key) {
    Node* current = root;
    while (current != nullptr) {
        size_t i = 0; // changed to size_t instead of int
        while (i < current->keys.size() && key > current->keys[i]) {
            i++;
        }
        if(i < current->keys.size() && key == current->keys[i]) {return current->values[i].empty() ? nullptr : current->values[i][0];} // value found
        if (current->isLeaf) {return nullptr;} // reached leaf node, not found
        current = current->children[i];
    }
    return nullptr; // not found
}

/*======== Helper Functions ========*/
void BPlusTree::insertNonFull(Node* node, const std::string& key, Word* word) {
    // case 1: leaf node
    if (node->isLeaf) {
        // find position to insert
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key); // lower_bound for sorted insert
        int ind = it - node->keys.begin();

        // insert key and value
        // if key already exists, append to values vector
        if (it != node->keys.end() && *it == key) {
            node->values[ind - 1].push_back(word);
        } 
        
        // otherwise, insert new key and value into new vector
        else {
            node->keys.insert(it, key);
            node->values.insert(node->values.begin() + ind, { word });
        }

        return;
    }

    // case 2: internal node
    int i = node->keys.size() - 1;
    while (i >= 0 && key < node->keys[i]) {
        i--;
    }
    i++; // child index to descend

    // if child is full, split
    if(node->children[i]->keys.size() == static_cast<size_t>(2 * t - 1)) {
        splitChild(node, i, node->children[i]);
        // after split, decide which child to descend
        if (key > node->keys[i]) {
            i++;
        }
    }

    // recursively call
    insertNonFull(node->children[i], key, word);
}

void BPlusTree::splitChild(Node* parent, int ind, Node* child) {
    Node* newChild = new Node(child->isLeaf);

    // insert new child pointer into parent
    parent->children.insert(parent->children.begin() + ind + 1, newChild);
    parent->keys.insert(parent->keys.begin() + ind, child->keys[t - 1]);

    // move keys and values to new child
    newChild->keys.assign(child->keys.begin() + t, child->keys.end());
    child->keys.resize(t - 1);

    // if leaf, move values
    if (child->isLeaf) {
        newChild->values.assign(child->values.begin() + t, child->values.end());
        child->values.resize(t - 1);

        // update leaf node links
        newChild->next = child->next;
        child->next = newChild;
    } else {
        // if internal, move children pointers
        newChild->children.assign(child->children.begin() + t, child->children.end());
        child->children.resize(t);
    }
}

std::string BPlusTree::nextPrefix(const std::string& prefix) {
    std::string r = prefix;
    
    // increment the last character
    for (int i = r.size() - 1; i >= 0; i--) {
        if ((unsigned char)r[i] != 255) {
            r[i] += 1;
            r.resize(i + 1);
            return r;
        }
    }
    return "";
}

std::vector<Word*> BPlusTree::rangeSearch(const std::string& lower, const std::string& upper) {
    std::vector<Word*> result;
    Node* curr = root;

    // traverse to the appropriate leaf node
    while (curr && !curr->isLeaf) {
        size_t i = 0; // changed to size_t instead of int
        while (i < curr->keys.size() && lower >= curr->keys[i]) {
            i++;
        }
        curr = curr->children[i];
    }

    // traverse leaf nodes and collect results
    while (curr) {
        for (size_t i = 0; i < curr->keys.size(); i++) { // changed to size_t instead of int
            
            // if results surpass the upper bound
            if (curr->keys[i] >= upper) {
                return result;
            } 
            
            // if key is within range
            if (curr->keys[i] >= lower) {
                for (Word* w : curr->values[i]) {
                    result.push_back(w); // NOW handles multiple Word* for duplicate keys
                }
            }
        }
        curr = curr->next;
    }

    return result;
}

