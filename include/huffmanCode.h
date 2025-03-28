#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include <string>
#include <bitset>

struct huffman_node {   
    int id;
    double prob;
    std::bitset<10> code; 
    int code_length;
    huffman_node* left; 
    huffman_node* right;
    huffman_node() {
        id = 0;
        prob = 0.0;
        code_length = 0;
        left = right = nullptr;
    }
};

typedef huffman_node* node_ptr;

class Huffman {

    protected:
        node_ptr node_array[1024];
        node_ptr child, parent, root;
        int id;
        class compare {
            public:
                bool operator()(const node_ptr& c1, const node_ptr& c2) const {
                    return c1->prob > c2->prob;
                }
        };
        void create_node_array();
        void traverse(node_ptr, std::bitset<10>);
        int binary_to_decimal(std::string&);
        std::string decimal_to_binary(int);
        inline void build_tree(std::string&, char);

    private:
        Huffman(std::string,std::string);
        void create_pq();
        void create_huffman_tree();
        void calculate_huffman_codes();
        void coding_save();
        void decoding_save();
        void recreate_huffman_tree();   
};

// struct compareNode {
//     bool operator()(const node_ptr& lhs, const node_ptr& rhs) const {
//         return lhs->prob > rhs->prob;
//     }
// };


#endif // HUFFMANCODE.H