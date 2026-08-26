#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "ast_printer.h"
#include "codegen.h"
#include "parser.h"
#include "scanner.h"
#include "semantic_analyzer.h"

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " [options] <file>\n\n"
              << "Options:\n"
              << "  -t,  --tokens      Print tokens\n"
              << "  -ast, --ast        Print AST\n"
              << "  -s,  --semantic    Run semantic analysis\n"
              << "  -ir, --ir          Emit LLVM IR to stdout (pure IR only)\n"
              << "  -o <file>          Write IR to <file> instead of stdout (with -ir)\n"
              << "  -h,  --help        Show this help\n";
}

int main(int argc, char* argv[]) {
    bool printTokens = false;
    bool printAst = false;
    bool runSemantic = false;
    bool generateIR = false;
    std::string filename;
    std::string outputFile;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-t" || arg == "--tokens") {
            printTokens = true;
        } else if (arg == "-ast" || arg == "--ast") {
            printAst = true;
        } else if (arg == "-s" || arg == "--semantic") {
            runSemantic = true;
        } else if (arg == "-ir" || arg == "--ir") {
            generateIR = true;
        } else if (arg == "-o") {
            if (i + 1 >= argc) {
                std::cerr << "Error: -o requires a filename\n";
                return 1;
            }
            outputFile = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        } else {
            if (!filename.empty()) {
                std::cerr << "Only one input file is allowed.\n";
                return 1;
            }
            filename = arg;
        }
    }

    if (filename.empty()) {
        std::cerr << "Error: no input file provided.\n";
        printUsage(argv[0]);
        return 1;
    }

    if (!printTokens && !printAst && !runSemantic && !generateIR) {
        runSemantic = true;
    }

    Scanner scanner = Scanner::fromFile(filename);
    std::vector<Token> tokens = scanner.scanTokens();

    if (printTokens) {
        std::cout << "--- Tokens ---\n";
        for (const Token& token : tokens) {
            std::cout << token.toString() << "\n";
        }
        std::cout << "\n";
    }

    Parser parser(tokens);
    Program program;

    try {
        program = parser.parse();
        std::cerr << "Parsing succeeded: " << program.statements.size()
                  << " statement(s), " << program.functions.size()
                  << " function(s).\n\n";
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed: " << e.what() << "\n";
        return 1;
    }

    if (printAst) {
        std::cout << "--- AST ---\n";
        AstPrinter printer;
        printer.print(program, std::cout);
        std::cout << "\n";
    }

    if (runSemantic || generateIR) {
        std::cerr << "--- Semantic Analysis ---\n";

        SemanticAnalyzer analyzer;
        analyzer.analyze(program);

        if (analyzer.hadError()) {
            std::cerr << "Semantic analysis failed due to type/scope errors.\n";
            return 1;
        }

        std::cerr << "Semantic analysis passed! Types and scopes are valid.\n\n";
    }

    if (generateIR) {
<<<<<<< HEAD
        CodeGen codegen;
=======
        std::cout << "--- LLVM IR ---\n";

        CodeGen codegen(filename);
>>>>>>> 0f5eac2 (fix(codegen): use input filename for module name instead of hardcoded string)
        codegen.generate(program);

        if (outputFile.empty()) {
            codegen.dump();
        } else {
            codegen.dumpToFile(outputFile);
            std::cerr << "LLVM IR written to " << outputFile << "\n";
        }
    }

    return 0;
}   