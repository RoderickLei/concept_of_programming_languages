#ifndef NODE
#define NODE

#include <string>

//node object used for the syntax tree and parse tree
struct Node{
    std::pair<std::string, std::string> info;
    Node *parentNode;
    Node *leftNode;
    Node *rightNode;
    Node *singleNode;
    int index;
};

#endif