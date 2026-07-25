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

    Parser parser(tokens);
    try {
        std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
        std::cout << "Parsing succeeded: " << statements.size()
                   << " expression statements parsed.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed: " << e.what() << "\n";
        return 1;
    }
}