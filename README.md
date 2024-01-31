# concept_of_programming_languages

The program is expected to function correctly with a minor exception. In cases where the error message reads "Error: Input string not fully parsed," the program will display the message as "Error: Missing variable" instead. It is assumed that lambdas adhere to left-association precedence.

To illustrate, if we encounter a sequence like <var> <expr>, and <expr> is composed of multiple <var> instances such as <var> <var> <var>, we assume the parentheses should be interpreted as (<var> <var> <var>) <var>).

All test files in .txt format are located within the test folder.

Here's a brief overview of how the program operates: Initially, it constructs a parse tree during lexical analysis. Subsequently, it utilizes this parse tree to build an abstract syntax tree. The program then invokes the reduce() function within the abstract syntax tree, which employs alpha conversions and beta reductions (up to 1000 times) to obtain a reduced tree. Finally, the print function of the abstract syntax tree is called to display the abstract syntax tree in a standard format.

To run the program, follow these steps:

1. Execute the command: make
2. Run the program with a specific filename using: ./lexer #filename
