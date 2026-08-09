#include "scope.h"

SymbolTable::SymbolTable() {
    beginScope();
}

void SymbolTable::beginScope() {
    scopes_.push_back(std::unordered_map<std::string, std::unique_ptr<Symbol>>());
}

void SymbolTable::endScope() {
    if (!scopes_.empty()) {
        scopes_.pop_back();
    }
}

bool SymbolTable::declare(std::unique_ptr<Symbol> symbol) {
    if (scopes_.empty()) return false;
    
    auto& currentScope = scopes_.back();
    if (currentScope.find(symbol->name) != currentScope.end()) {
        return false;
    }
    
    currentScope[symbol->name] = std::move(symbol);
    return true;
}

Symbol* SymbolTable::resolve(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second.get();
        }
    }
    return nullptr;
}

bool SymbolTable::isDeclaredInCurrentScope(const std::string& name) const {
    if (scopes_.empty()) return false;
    return scopes_.back().find(name) != scopes_.back().end();
}