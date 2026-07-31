#pragma once
#include <string>
#include <vector>
#include <memory>
#include "token.h"
#include "stmt.h"

class Parameter
{
public:
    Parameter(Token name, int line, int column)
        : name(std::move(name)), line(line), column(column) {}

    Token name;
    int line;
    int column;
};

class Prototype
{
public:
    Prototype(Token name, std::vector<Parameter> params, int line, int column)
        : name(std::move(name)), params(std::move(params)), line(line), column(column) {}

    Token name;
    std::vector<Parameter> params;
    int line;
    int column;
};

class FuncDefn
{
public:
    FuncDefn(Prototype proto, std::unique_ptr<BlockStmt> body, int line, int column)
        : proto(std::move(proto)), body(std::move(body)), line(line), column(column) {}

    Prototype proto;
    std::unique_ptr<BlockStmt> body;
    int line;
    int column;
};