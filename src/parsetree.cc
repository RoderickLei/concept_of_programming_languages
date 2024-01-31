#include <stdexcept>
#include <string>
#include <iostream>
#include <parsetree.h>

//Default constructor
ParseTree::ParseTree() {
}

// Function to get the root node of the parse tree
Node * ParseTree::getRoot() {
    return this->root;
}

// Function to get all nodes of the parse tree
std::vector<Node *> ParseTree::getNodes() {
    return this->nodes;
}

// Function to add a node to the parse tree
void ParseTree::add(Node * node){
    this->size++; // Increment the size of the parse tree
    node->index = this->size; // Assign an index to the node

    // If the parse tree is empty, make the new node the root
    if ((int) this->nodes.size() == 0) {
        root = node;
        nodes.push_back(node);
        return;
    }

    // Add the node to the list of nodes
    nodes.push_back(node);

    if (node->parentNode->info.first == "EXPR") {
        if (node->info.first == "LEXPR") 
            node->parentNode->leftNode = node;
        else if (node->info.first == "EXPR1") 
            node->parentNode->rightNode = node;
    } else if (node->parentNode->info.first == "EXPR1") {
        if (node->info.first == "LEXPR") 
            node->parentNode->leftNode = node;
        else if (node->info.first == "EXPR1") 
            node->parentNode->rightNode = node;
    } else if (node->parentNode->info.first == "LEXPR") {
        if (node->info.first == "PEXPR") 
            node->parentNode->singleNode = node;
        else if (node->info.first == "BACKSLASH") 
            node->parentNode->singleNode = node;
    } else if (node->parentNode->info.first == "PEXPR") {
        if (node->info.first == "VARIABLE") 
            node->parentNode->singleNode = node;
        else if (node->info.first == "EXPR") 
            node->parentNode->singleNode = node;
    } else if (node->parentNode->info.first == "BACKSLASH") {
            node->parentNode->singleNode = node;
    } 
}

// Method to print the parse tree
void ParseTree::print() const {
    if ((int) nodes.size() == 0) {
        std::cout << "tree is empty" << std::endl;
        return;
    }

    // Print all nodes in the parse tree
    std::cout << "----------------------------------- Vector -----------------------------------" << std::endl;
    for (int i = 0; i < (int) nodes.size(); i++) {
        std::cout << "(" << nodes[i]->info.first << ", " << nodes[i]->info.second << ")" << std::endl;
    }


    std::cout << "----------------------------------- Tree -----------------------------------" << std::endl;
    // push root onto stack
    std::vector<Node *> stack;
    stack.push_back(root);
    while (!stack.empty()) {
        Node * currentNode = stack[0];
        
        int parentIndex = -1;
        if (currentNode->parentNode != nullptr) {
            parentIndex = currentNode->parentNode->index;
        }
        // int currentIndex = currentNode->index;
        std::cout << "parent: " << parentIndex << " - (" << currentNode->info.first << ", " << currentNode->info.second << ") - node address: " << currentNode->index << std::endl;
        // do something
        if (currentNode->leftNode != nullptr) 
            stack.push_back(currentNode->leftNode);

        if (currentNode->rightNode != nullptr) 
            stack.push_back(currentNode->rightNode);
            
        if (currentNode->singleNode != nullptr) 
            stack.push_back(currentNode->singleNode);

        stack.erase(stack.begin());
    }
	std::cout << std::endl;
}
