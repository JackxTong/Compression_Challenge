#include <iostream>
#include "huffmanCode.h"
#include "findProbabilities.h"

using namespace std;

unique_ptr<vector<pair<int, double>>> sortedProb = returnSortedProb();

void Huffman::create_node_array() {

    if (!sortedProb || sortedProb->empty()){
        cerr << "Error: Sorted Prob is empty or unititialised" << endl;
        return;
    }

    for (int i = 0; i < sortedProb->size(); ++i) {
        node_array[i] = new huffman_node;
        node_array[i]->id = (*sortedProb)[i].first;
        node_array[i]->prob = (*sortedProb)[i].second;
    }   
}

void Huffman::traverse(node_ptr node, bitset<10> code) {
    if (node->left == nullptr && node->right == nullptr) {
        node->code = code;
    }
    else {
        traverse(node->left, code + 0);
        traverse(node->right, code + 1);
    }
}

int Huffman::binary_to_decimal(string& in) {
    int result = 0;

}
