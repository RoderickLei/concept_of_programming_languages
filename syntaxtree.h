#ifndef SYNTAXTREE
#define SYNTAXTREE

#include <parsetree.h>
#include <vector>
#include <node.h>

//This is a tree structure used for parsing an expression. 
//It's designed in such a way that brackets are already excluded.

class SyntaxTree{
    public:
        //Constructors
        SyntaxTree();
        SyntaxTree(ParseTree * parseTree); //Constructor that takes a parsetree as input

        Node * getRoot(); //returns the root of the parse tree

        void add(Node * node); //adds node to parse tree
        void remove(Node * node); //removes node from the parse tree
        void cleanUpTree(); //removes all nodes that are not used in the parse tree
        void reduce(); //reduces the parse tree to a minimal form
        void betaReduction(Node * node, bool & reductionDone); //reduces the parse tree to a minimal form
        bool capturedVariable(Node * node, std::string variable); 
        bool hasVariable(Node * node); //checks if the node has a variable
        void alfaConversion(Node *node, std::string oldCharacter, std::string newCharacter); 
        void replaceVar(Node * node, std::string variable, Node * substitution);
        Node * copyTree(Node * root, Node * parent);
        bool possibleBetaReduction(Node * node);
        void print(Node * node) const; //prints the parse tree

    private:
        Node *root = nullptr;
        std::vector<Node *> nodes;
        int size = -1;
};

#endif