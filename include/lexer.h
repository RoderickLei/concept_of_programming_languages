#ifndef LEXER
#define LEXER

#include <vector>
#include <string>
#include <parsetree.h>
#include <node.h>

//lexically analyses lambda expressions
class Lexer {
    public:
        //constructors
        Lexer();
        Lexer(std::vector<std::pair<std::string, std::string>> expression);
        void expr(ParseTree * parseTree, Node * parentNode);
        void expr1(ParseTree * parseTree, Node * parentNode);
        void lexpr(ParseTree * parseTree, Node * parentNode);
        void pexpr(ParseTree * parseTree, Node * parentNode);
        bool isEmpty(); //checks if we have went through the entire string
        std::pair<std::string, std::string> currentToken();
        void counter(); //increment token index

    private:
        int index = 0;
        int openBrackets = 0;
        std::vector<std::pair<std::string, std::string>> expression;
};

#endif