#include <stdexcept>
#include <string>
#include <iostream>
#include <parsetree.h>
#include <lexer.h>

// Default constructor
Lexer::Lexer(){}

// Constructor that initializes the expression
Lexer::Lexer(std::vector<std::pair<std::string, std::string>> expression){
    this->expression = expression;
}

// Function to parse an expression
void Lexer::expr(ParseTree * ast, Node * parentNode) {
    Node * tempNode = new Node();
    tempNode->info = std::make_pair("EXPR", " ");
    tempNode->parentNode = parentNode;
    ast->add(tempNode);

    lexpr(ast, tempNode);
    expr1(ast, tempNode);
}

// Function to parse an expression1
void Lexer::expr1(ParseTree * ast, Node * parentNode) {
    Node * tempNode = new Node();
    tempNode->info = std::make_pair("EXPR1", "");
    tempNode->parentNode = parentNode;
    ast->add(tempNode);

    if (!isEmpty() && !(currentToken().first == "BRACKET_CLOSE" && openBrackets > 0)) {
        lexpr(ast, tempNode);
        expr1(ast, tempNode);
    }
}

// Function to parse a lexpr
void Lexer::lexpr(ParseTree * ast, Node * parentNode){
    Node * tempNode = new Node();
    tempNode->info = std::make_pair("LEXPR", "");
    tempNode->parentNode = parentNode;
    ast->add(tempNode);

    if (!isEmpty() && currentToken().first == "BACKSLASH") {
        counter();
        if (!isEmpty() && currentToken().first == "VARIABLE") {
            Node * tempNode2 = new Node();
            tempNode2->info = std::make_pair("BACKSLASH", currentToken().second);
            tempNode2->parentNode = tempNode;
            ast->add(tempNode2);
            counter();
            if (isEmpty()) throw std::runtime_error("Error: Missing expression after lambda abstraction");

            lexpr(ast, tempNode2); 
        } else {
            throw std::runtime_error("Error: Missing variable after lambda");
        }
    } else {
        pexpr(ast, tempNode);
    }
}

// Function to parse a pexpr
void Lexer::pexpr(ParseTree * ast, Node * parentNode){
    Node * tempNode = new Node();
    tempNode->info = std::make_pair("PEXPR", "");
    tempNode->parentNode = parentNode;
    ast->add(tempNode);

    if (!isEmpty() && currentToken().first == "BRACKET_OPEN") {
        counter();
        openBrackets++;

        if (isEmpty() || (!isEmpty() && currentToken().first == "BRACKET_CLOSE")) 
            throw std::runtime_error("Error: Missing expression after opening parenthesis");
        
        expr(ast, tempNode);

        if (!isEmpty() && currentToken().first == "BRACKET_CLOSE") {
            counter();
            openBrackets--;
        } else {
            throw std::runtime_error("Error: Missing closing parenthesis");
        }

    } else if (!isEmpty() && currentToken().first == "VARIABLE") {
        Node * tempNode2 = new Node();
        tempNode2->info = std::make_pair("VARIABLE", currentToken().second);
        tempNode2->parentNode = tempNode;
        ast->add(tempNode2);
        counter();
    } else {
        throw std::runtime_error("Error: Missing variable");
    }
}

// Function to check if the expression is empty
bool Lexer::isEmpty() {
    return ((unsigned int) index >= expression.size());
}

// Function to get the current token
std::pair<std::string, std::string> Lexer::currentToken() {
    return expression[index];
}

// Function to increment the counter
void Lexer::counter() {
    index++;
}