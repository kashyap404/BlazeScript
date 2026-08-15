#pragma once
#include "stmt.h"
#include "token.h"
#include "type.h"
#include <memory>
#include <string>
#include <vector>

class Parameter {
public:
    Parameter(Token name, Type* type, int line, int column)
        : name(std::move(name)), type(type), line(line), column(column) {}

    Token name;
    Type* type;
    int line;
    int column;
};

class Prototype {
public:
    Prototype(Token name, std::vector<Parameter> params, Type* returnType, int line, int column)
        : name(std::move(name)), params(std::move(params)), returnType(returnType), line(line),
          column(column) {}

    Token name;
    std::vector<Parameter> params;
    Type* returnType;
    int line;
    int column;
};

class FuncDefn {
public:
    FuncDefn(Prototype proto, std::unique_ptr<BlockStmt> body, int line, int column)
        : proto(std::move(proto)), body(std::move(body)), line(line), column(column) {}

    Prototype proto;
    std::unique_ptr<BlockStmt> body;
    int line;
    int column;
};