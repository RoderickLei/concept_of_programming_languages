#ifndef TOKEN
#define TOKEN

#include <vector>
#include <string>

class Token {
    public:
        Token(); //Constructor
        std::vector<std::vector<std::pair<std::string, std::string>>> getToken();
        void tokenize(std::string const & input); //tokenizes the input string
        std::string helpVariable(int &i, std::string const & input); //create variable token
        void disambiguateString(int& i, int& j, int& varCount); //disambiguates an expression string line
        void disambiguateStrings(); //disambiguates all expression strings
        void encloseBrackets(int i, int& j, int varCount); //puts brackets around expressions
        void print();

    private:
        int index = 0;
        std::vector<std::vector<std::pair<std::string, std::string>>> tokenStrings;
};

#endif