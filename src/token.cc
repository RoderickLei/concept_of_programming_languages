#include "token.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <string>

//default constructor
Token::Token() {
    this->tokenStrings = {{}};
}

// Method to print a node
std::vector<std::vector<std::pair<std::string, std::string>>> Token::getToken() {
    return tokenStrings;
}

// Method to tokenize an input string
void Token::tokenize(std::string const & input) {
    // Iterate over each character in the input
    for (int i = 0; (unsigned int) i < input.length(); i++) {
        std::vector<std::pair<std::string, std::string>> expressionTokenString = {};
        // Switch on the current character
        switch (input[i]) {
            // Ignore spaces
            case ' ':
                break;
            case '\r':
            case '\n':
                tokenStrings.push_back(expressionTokenString);
                break;
            case '(':
                tokenStrings.back().push_back(std::make_pair("BRACKET_OPEN", "("));
                break;
            case ')':
                tokenStrings.back().push_back(std::make_pair("BRACKET_CLOSE", ")"));
                break;
            case '\\':
                tokenStrings.back().push_back(std::make_pair("BACKSLASH", "\\"));
                break;
            default: 
                std::string variable = helpVariable(i, input);
                tokenStrings.back().push_back(std::make_pair("VARIABLE", variable));
        }
        // case '\u03BB':
    }
}

// Helper method to handle variables
std::string Token::helpVariable(int &i, std::string const & input) {
    //error if the variable does not start with a letter
    if (!(input[i]  >= 'A' && input[i] <= 'Z') && !(input[i]  >= 'a' && input[i] <= 'z')) {
        throw std::runtime_error("Error: Variable not starting with letter from latin alphabet");
    }

    // Initialize an empty string for the variable
    std::string variableString = "";
    // Concatenate all consecutive letters or numbers characters to form the variable
    while ((input[i]  >= 'A' && input[i] <= 'Z') || (input[i]  >= 'a' && input[i] <= 'z') || (input[i]  >= '0' && input[i] <= '9')) {
        variableString += input[i];
        i++;
    }

    i--;// Lower i to correct for the extra increment in the loop

    return variableString; 
}

// Method to print the tokenized strings
void Token::print() {
    std::string s;

    for (int i = 0; (unsigned int) i < tokenStrings.size(); i++) {
        for (int j = 0; (unsigned int) j < tokenStrings[i].size(); j++) {
            s = tokenStrings[i][j].second;
            std::cout << s;

            if (s != "(" && s != "\\") {
                if ((unsigned int) j+1 < tokenStrings[i].size() &&
                    tokenStrings[i][j+1].second != ")" && tokenStrings[i][j+1].second != "(")
                    std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

//Method to disambiguate all expression strings
void Token::disambiguateStrings() {
    int j = 0;
    int varCount = 0;

    for (int i = 0; (unsigned int) i < getToken().size(); i++) {
        disambiguateString(i, j, varCount);
        j = 0;
        varCount = 0;
    }
}

//Method to disambiguate an expression string line
void Token::disambiguateString(int& i, int& j, int& varCount) {
    if (varCount >= 3) {
        encloseBrackets(i, j, varCount);
    }
    if ((unsigned int) j < tokenStrings[i].size() && tokenStrings[i][j].first == "VARIABLE") { 
        j++;
        varCount++;
        disambiguateString(i, j, varCount);
    }
    else {
        varCount = 0;
        
        if ((unsigned int) j < tokenStrings[i].size() && tokenStrings[i][j].first == "BACKSLASH") {
            tokenStrings[i].insert(tokenStrings[i].begin() + j, std::make_pair("BRACKET_OPEN", "("));
            tokenStrings[i].insert(tokenStrings[i].end(), std::make_pair("BRACKET_CLOSE", ")")); //lambda means everything else after backslash
            j += 2;
            disambiguateString(i, j, varCount);
        }
        j += 2;
    } 
}

// Method to enclose expressions in brackets
void Token::encloseBrackets(int i, int& j, int varCount) {
    int add = 0;

    if (varCount >= 4)
        add = varCount - 3;
    
    if (tokenStrings[i][j - varCount - add].first == "BACKSLASH")
        return;
    tokenStrings[i].insert(tokenStrings[i].begin() + j-varCount-add, std::make_pair("BRACKET_OPEN", "("));
    tokenStrings[i].insert(tokenStrings[i].begin() + j, std::make_pair("BRACKET_CLOSE", ")"));

    j += 2;
}