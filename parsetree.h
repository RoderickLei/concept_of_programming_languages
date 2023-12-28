#ifndef PARSETREE
#define PARSETREE

#include <vector>
#include <node.h>

// This is a tree structure used for parsing an expression. 
// It's designed in such a way that brackets are already excluded.

class ParseTree {
    public:
        //Constructor
        ParseTree();
        Node * getRoot(); //returns the root of the parse tree

        std::vector<Node *> getNodes(); //returns all nodes in the parse tree
        void add(Node * node); //adds node to parse tree
        void remove(Node * node); //removes node from the parse tree
        void print() const; // prints the parse tree

    private:
        Node * root = nullptr;
        std::vector<Node *> nodes;
        int size = -1;
};

#endif