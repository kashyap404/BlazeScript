#include <iostream>
#include "scanner.h"
#include "parser.h"
#include "ast_printer.h"  
#include "semantic_analyzer.h"

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

        std::cout << "\n--- AST ---\n";        
        AstPrinter printer;                     
        printer.print(program, std::cout);     

       
        std::cout << "\n--- Semantic Analysis ---\n";
        SemanticAnalyzer analyzer;
        analyzer.analyze(program);

        if (analyzer.hadError()) {
            std::cerr << "Semantic analysis failed due to type/scope errors.\n";
            return 1;
        }

        std::cout << "Semantic analysis passed! Types and scopes are valid.\n";
        // ----------------------------------------------------

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed: " << e.what() << "\n";
        return 1;
    }
}