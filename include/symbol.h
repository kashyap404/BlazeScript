#pragma once
#include "type.h"
#include <string>

enum class SymbolKind { VARIABLE, FUNCTION, PARAMETER };

struct Symbol {
    std::string name;
    Type* type;
    SymbolKind kind;
    int line;
    int column;
};