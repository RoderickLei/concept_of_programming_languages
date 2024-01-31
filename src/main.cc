#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <parsetree.h>
#include <syntaxtree.h>
#include <token.h>
#include <lexer.h>


bool openReadFile(std::ifstream &readFile, std::string readFileName) {
  readFile.open(readFileName);
  
  if (!readFile.good()) { // file couldn't be opened
    std::cout << "File: " << readFileName << " could not be opened" << std::endl;
    std::cerr << "File: " << readFileName << " could not be opened" << std::endl;
    readFile.close();
    return false;
  }

  return true;
}

/**
 * Analyzes and parses each expression from the file.
 *
 * @param result A string containing the read expression.
 * @param tokenString An object storing token strings for each read expression. Each token is paired with its corresponding substring from the expression.
 *
 * @return A boolean indicating if an error was encountered.
 *
 * @throws runtime_error If the expression is incorrect.
 *
 * @exceptsafe Provides a strong exception guarantee.
 */

bool checkExpression(std::string const & result, Token * tokenString) {
    int errorLine = 1;
	
	try {
      // create a string of tokens form the original expression
		tokenString->tokenize(result);

		for (int i = 0; (unsigned int) i < tokenString->getToken().size(); i++) {
            // Feed the token string into the lexer
			Lexer *l = new Lexer(tokenString->getToken()[i]); 

            // Perform lexical analysis on the token string
            // Provide an empty parse tree to be populated
			ParseTree * parseTree = new ParseTree();

			// and build it while doing the lexical analysis
			l->expr(parseTree, nullptr);

            SyntaxTree * abstractSyntaxTree = new SyntaxTree(parseTree);
            abstractSyntaxTree->reduce();
            abstractSyntaxTree->print(abstractSyntaxTree->getRoot());

			// keep track wich expression is wrong in a multi expression file
			errorLine++;
		}
	} catch (const std::runtime_error& error) {
		// Place your code for handling errors here
		std::cout << error.what() << "\nFound at expression " << errorLine << std::endl;
		std::cerr << error.what() << "\nFound at expression " << errorLine << std::endl;
		return true;
    }

	return false;
}

int main(int argc, char* argv[]) {
    std::ifstream lambdaExpressionReadFile;
    if (!openReadFile(lambdaExpressionReadFile, argv[argc - 1])) return 1;
    
    std::string result = "";
    if (lambdaExpressionReadFile.is_open()) {
        char symbol;
        // Only reads in and accepts the first expression. Even if file
        // has multiple expressions.
        while (lambdaExpressionReadFile.get(symbol) && symbol != '\n') {
            result += symbol;
        }
    }
    
    Token * tokenString = new Token();

	if (checkExpression(result, tokenString)) {
        delete tokenString;
        return 1;
    }
    
    delete tokenString;

    return 0;
}