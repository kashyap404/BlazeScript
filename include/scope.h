#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "symbol.h"

class SymbolTable {
public:
    SymbolTable();   //push global scope 

    void beginScope();
    void endScope();

    bool declare(std::unique_ptr<Symbol> symbol);
    Symbol* resolve(const std::string& name) const;

    bool isDeclaredInCurrentScope(const std::string& name) const;

private:
    std::vector<std::unordered_map<std::string, std::unique_ptr<Symbol>>> scopes_;
};