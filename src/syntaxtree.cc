#include <stdexcept>
#include <string>
#include <iostream>
#include <parsetree.h>
#include <syntaxtree.h>
#include <node.h>


// Default constructor
SyntaxTree::SyntaxTree() {}

// Constructor that builds a SyntaxTree from a ParseTree
SyntaxTree::SyntaxTree(ParseTree * pTree) {
    // Get all nodes from the ParseTree
    auto originalNodes = pTree->getNodes();
    int nodeCount = originalNodes.size();

    // Clone each node from the ParseTree
    for (int idx = 0; idx < nodeCount; idx++) {
        Node * originalNode = originalNodes[idx];
        Node * clonedNode = new Node();

        // Copy data from the original node to the cloned node
        clonedNode->index = originalNode->index;
        clonedNode->info = std::make_pair(originalNode->info.first, originalNode->info.second);

        // Set the parent of the cloned node
        clonedNode->parentNode = (originalNode->parentNode == nullptr) ? nullptr : nodes[originalNode->parentNode->index];

        // Add the cloned node to the SyntaxTree
        nodes.push_back(clonedNode);
    }

    // Set the root of the SyntaxTree
    root = nodes[0];

    // Set the children of each cloned node
    for (int idx = 0; idx < nodeCount; idx++) {
        Node * originalNode = originalNodes[idx];
        Node * clonedNode = nodes[idx];

        // Set the left, right, and single child of the cloned node
        clonedNode->leftNode = (originalNode->leftNode == nullptr) ? nullptr : nodes[originalNode->leftNode->index];
        clonedNode->rightNode = (originalNode->rightNode == nullptr) ? nullptr : nodes[originalNode->rightNode->index];
        clonedNode->singleNode = (originalNode->singleNode == nullptr) ? nullptr : nodes[originalNode->singleNode->index];
    }
    // Clean up the SyntaxTree
    clean();
}


Node * SyntaxTree::getRoot() {
    return root;
}

//clean up the SyntaxTree
void SyntaxTree::clean() {
    for (int i = 1; i < nodes.size(); i++) {
        Node * node = nodes[i];
        if (node->singleNode != nullptr && node->info.first != "BACKSLASH" && node->info.first != "EMPTY") {
            node->singleNode->parentNode = node->parentNode;
            if (node->parentNode->leftNode == node){
                node->parentNode->leftNode = node->singleNode;
                node->info.first = "EMPTY";
            }
            else if (node->parentNode->rightNode == node) {
                node->parentNode->rightNode = node->singleNode;
                node->info.first = "EMPTY";
            }
            else if (node->parentNode->singleNode == node) {
                node->parentNode->singleNode = node->singleNode;
                node->info.first = "EMPTY";
            }
        } 
        if (node->info.first == "EXPR1") {
            if (node->leftNode == nullptr && node->rightNode == nullptr) {
                node->parentNode->singleNode = node->parentNode->leftNode;
                node->parentNode->leftNode = nullptr;
                node->parentNode->rightNode = nullptr;
                node->info.first = "EMPTY";
            }
        }
    }

    for (int i = 1; i < nodes.size(); i++) {
        Node * node = nodes[i];
        if (node->singleNode != nullptr && node->info.first != "BACKSLASH" && node->info.first != "EMPTY") {
            node->singleNode->parentNode = node->parentNode;
            if (node->parentNode->leftNode == node){
                node->parentNode->leftNode = node->singleNode;
                node->info.first = "EMPTY";
            }
            else if (node->parentNode->rightNode == node) {
                node->parentNode->rightNode = node->singleNode;
                node->info.first = "EMPTY";
            }
            else if (node->parentNode->singleNode == node) {
                node->parentNode->singleNode = node->singleNode;
                node->info.first = "EMPTY";
            }
        } 
    }
    for (int i = 0; i < nodes.size(); i++) {
        Node * node = nodes[i];
        if (node->info.first == "EXPR" || node->info.first == "EXPR1") {
            node->info.first = "SPACE";
            node->info.second = " ";
        } 
    }
}

// Method to perform beta reduction on the syntax tree
void SyntaxTree::reduce() {
    int counter = 0;
    // Keep reducing the tree while possible, up to a maximum of 1000 times
    while (possibleBetaReduction(root) && counter < 1000) {
        std::cout << "start: ";
        bool reductionDone = false;
        betaReduction(root, reductionDone);// Perform the beta reduction
        counter++;
    }

    if (possibleBetaReduction(root)) 
        throw std::runtime_error("Maximum number of reductions steps (1000) reached.");
}

// Method to check if beta reduction is possible on a given node
bool SyntaxTree::possibleBetaReduction(Node * node) {
    if (node == nullptr) return false;

    // If the left child of the node is a "BACKSLASH" and the right child has a variable, beta reduction is possible
    if (node->leftNode != nullptr && node->leftNode->info.first == "BACKSLASH" ) {
        if (hasVariable(node->rightNode)) {
            return true;
        }
    }
    // Recursively check if beta reduction is possible on the children of the node
    return (possibleBetaReduction(node->leftNode) || possibleBetaReduction(node->rightNode) || possibleBetaReduction(node->singleNode));
}

// Method to create a copy of a tree given its root
Node * SyntaxTree::copyTree(Node * root, Node * parent) {
    if (root == nullptr) return nullptr;
        
    // Create a new node and copy the information from the root to the new node    
    Node * copy = new Node();
    copy->info = root->info;
    copy->index = root->index;
    copy->leftNode = copyTree(root->leftNode, copy);
    copy->rightNode = copyTree(root->rightNode, copy);
    copy->singleNode = copyTree(root->singleNode, copy);
    copy->parentNode = parent;
    return copy;
}

// Method to perform beta reduction on a given node
void SyntaxTree::betaReduction(Node * node, bool & reductionDone) {
    // If the node is null or reduction is already done, return
    if (node == nullptr) return;
    if (reductionDone) return;

        std::cout << std::endl;
        std::cout << "before replaceVar: ";
        print(node->leftNode);
    if (node->leftNode != nullptr && node->leftNode->info.first == "BACKSLASH") {
        if (capturedVariable(node->rightNode, node->leftNode->info.second)) {
            // we do have a problem when all single characters are already in use!
            std::string newCharacter = "a";

            while(capturedVariable(node->leftNode, newCharacter)) newCharacter[0]++;
            alfaConversion(node->leftNode, node->leftNode->info.second, newCharacter);
        }
    
        std::cout << std::endl;
        std::cout << "before replaceVar: ";
        print(root);
        replaceVar(node->leftNode->singleNode, node->leftNode->info.second, node->rightNode);
        std::cout << "after replaceVar: ";
        print(root);
        
        node->leftNode->singleNode->parentNode = node->parentNode;
        if (node->parentNode != nullptr) {
            if (node->parentNode->leftNode == node)
                node->parentNode->leftNode = node->leftNode->singleNode;
            else if (node->parentNode->rightNode == node)
                node->parentNode->rightNode = node->leftNode->singleNode;
            else if (node->parentNode->singleNode == node)
                node->parentNode->singleNode = node->leftNode->singleNode;
        }
        node = node->leftNode->singleNode;

        if (node->parentNode == nullptr) {
            root = node;
        }

        reductionDone = true; // Indicate that reduction is done
        return;
    }

    // Recursively perform beta reduction on the children of the node
    betaReduction(node->singleNode, reductionDone);
    betaReduction(node->leftNode, reductionDone);
    betaReduction(node->rightNode, reductionDone);
}

// Method to check if a given variable is captured by a given node
bool SyntaxTree::capturedVariable(Node * node, std::string variable) {
    if (node == nullptr) return false;
    if (node->info.first == "VARIABLE" && node->info.second == variable) return true;
    return (capturedVariable(node->leftNode, variable) || capturedVariable(node->rightNode, variable) || capturedVariable(node->singleNode, variable));
}

// Method to replace a given variable in a node with a substitution
void SyntaxTree::replaceVar(Node * node, std::string variable, Node * substitution) {
    if (node == nullptr) return;

    std::cout << "#" << node->info.first << ", " << node->info.second << ", " << variable  << "#"<< std::endl;
    print(root);
    std::cout << std::endl; 
    if (node->info.first == "VARIABLE" && node->info.second == variable) {
        Node * tempSubstitution = copyTree(substitution, nullptr); // Create a copy of the substitution
        print(tempSubstitution->parentNode);
        std::cout << std::endl; 
        // Replace the node with the substitution in the parent node
        tempSubstitution->parentNode = node->parentNode;
        if (node->parentNode->leftNode == node)
            node->parentNode->leftNode = tempSubstitution;
        else if (node->parentNode->rightNode == node)
            node->parentNode->rightNode = tempSubstitution;
        else if (node->parentNode->singleNode == node)
            node->parentNode->singleNode = tempSubstitution;

        node = tempSubstitution;
        print(root);
        std::cout << std::endl; 
        return;
    }
    replaceVar(node->singleNode, variable, substitution);
    replaceVar(node->leftNode, variable, substitution);
    replaceVar(node->rightNode, variable, substitution);
}

// Method to check if a node has a variable
bool SyntaxTree::hasVariable(Node * node) {
    if (node == nullptr) return false;
    if (node->info.first == "VARIABLE") {
        return true;
    }
    return (hasVariable(node->leftNode) || hasVariable(node->rightNode) || hasVariable(node->singleNode));
}

// Method to perform alpha conversion on a node
void SyntaxTree::alfaConversion(Node * node, std::string oldCharacter, std::string newCharacter) {
    if (node == nullptr) return;
    if ((node->info.first == "VARIABLE" || node->info.first == "BACKSLASH") && node->info.second == oldCharacter) node->info.second = newCharacter;

    alfaConversion(node->singleNode, oldCharacter, newCharacter);
    alfaConversion(node->leftNode, oldCharacter, newCharacter);
    alfaConversion(node->rightNode, oldCharacter, newCharacter);
}

// Method to print a node
void SyntaxTree::print(Node * node) const {
    if (node == nullptr) return;

    int parentIndex = (node->parentNode == nullptr) ? -1 : node->parentNode->index;

    if (node->singleNode != nullptr) {
        if (node->info.first == "BACKSLASH") {
            std::cout << "(" ;
            std::cout << "\\" << node->info.second << " ";
            print(node->singleNode);
            std::cout << ")" ;
        } else {
            print(node->singleNode);
            std::cout << node->info.second;
        }
    } else {
        std::cout << "(" ;
        print(node->leftNode);
        std::cout << node->info.second;
        print(node->rightNode);
        std::cout << ")" ;
    }
    if (node == root) std::cout << std::endl;
}

