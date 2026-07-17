#include <iostream>
#include "scanner.h"

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

    return 0;
}