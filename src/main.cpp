#include <iostream>
#include "scanner.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: blazescript <path>\n";
        return 1;
    }

    Scanner scanner = Scanner::fromFile(argv[1]);
    std::vector<Token> tokens = scanner.scanTokens();

    for (const Token& token : tokens) {
        std::cout << token.toString() << "\n";
    }

    Parser parser(tokens);
    try {
        Program program = parser.parse();
        std::cout << "Parsing succeeded: " << program.statements.size() << " statement(s), "
                   << program.functions.size() << " function(s) parsed.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed: " << e.what() << "\n";
        return 1;
    }
}