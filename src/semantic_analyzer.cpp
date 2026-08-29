#include "semantic_analyzer.h"
#include "type.h"
#include <algorithm>
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer() = default;

void SemanticAnalyzer::error(int line, int column, const std::string &message) {
    std::cerr << "[line " << line << ", col " << column << "] semantic error: " << message << "\n";
    hadError_ = true;
}

void SemanticAnalyzer::analyze(Program &program) {
    for (const auto &fn : program.functions) {
        functions_[fn->proto.name.lexeme_] = fn.get();
    }

    for (const auto &fn : program.functions) {
        analyzeFunction(*fn);
    }
    for (const auto &stmt : program.statements) 
    {
        stmt->accept(*this);
    }
}

void SemanticAnalyzer::analyzeFunction(FuncDefn &fn) {
    auto fnSymbol =
        std::make_unique<Symbol>(Symbol{fn.proto.name.lexeme_, fn.proto.returnType,
                                        SymbolKind::FUNCTION, fn.proto.line, fn.proto.column});
    if (!symbols_.declare(std::move(fnSymbol))) {
        error(fn.proto.line, fn.proto.column,
              "function '" + fn.proto.name.lexeme_ + "' already declared");
    }

    symbols_.beginScope();

    for (const Parameter &p : fn.proto.params) {
        auto paramSymbol = std::make_unique<Symbol>(
            Symbol{p.name.lexeme_, p.type, SymbolKind::PARAMETER, p.line, p.column});
        if (!symbols_.declare(std::move(paramSymbol))) {
            error(p.line, p.column, "parameter '" + p.name.lexeme_ + "' already declared");
        }
    }

    Type *savedReturnType = currentFunctionReturnType_;
    currentFunctionReturnType_ = fn.proto.returnType;

    fn.body->accept(*this);

    currentFunctionReturnType_ = savedReturnType;
    symbols_.endScope();
}


void SemanticAnalyzer::visitLiteralExpr(LiteralExpr &expr) {
    lastType_ = types_.getType(TypeKind::I32);
}

void SemanticAnalyzer::visitVariableExpr(VariableExpr &expr) {
    Symbol *sym = symbols_.resolve(expr.name.lexeme_);
    if (!sym) {
        error(expr.line, expr.column, "undeclared variable '" + expr.name.lexeme_ + "'");
        lastType_ = types_.getType(TypeKind::UNKNOWN);
        return;
    }
    lastType_ = sym->type;
}

void SemanticAnalyzer::visitBinaryExpr(BinaryExpr &expr) {
    expr.left->accept(*this);
    Type *leftType = lastType_;
    expr.right->accept(*this);
    Type *rightType = lastType_;

    if (leftType->kind == TypeKind::UNKNOWN || rightType->kind == TypeKind::UNKNOWN) {
        lastType_ = types_.getType(TypeKind::UNKNOWN);
        return;
    }

    switch (expr.op) {
    case OperatorType::AND:
    case OperatorType::OR:
        if (leftType->kind != TypeKind::BOOL || rightType->kind != TypeKind::BOOL) {
            error(expr.line, expr.column, "'and'/'or' require bool operands");
            lastType_ = types_.getType(TypeKind::UNKNOWN);
            return;
        }
        lastType_ = types_.getType(TypeKind::BOOL);
        return;

    case OperatorType::EQUAL_EQUAL:
    case OperatorType::BANG_EQUAL:
        if (!leftType->equals(rightType)) {
            error(expr.line, expr.column, "cannot compare mismatched types");
            lastType_ = types_.getType(TypeKind::UNKNOWN);
            return;
        }
        lastType_ = types_.getType(TypeKind::BOOL);
        return;

    case OperatorType::GREATER:
    case OperatorType::GREATER_EQUAL:
    case OperatorType::LESS:
    case OperatorType::LESS_EQUAL:
        if (!leftType->isNumeric() || !rightType->isNumeric() || !leftType->equals(rightType)) {
            error(expr.line, expr.column, "comparison requires matching numeric operands");
            lastType_ = types_.getType(TypeKind::UNKNOWN);
            return;
        }
        lastType_ = types_.getType(TypeKind::BOOL);
        return;

    case OperatorType::PLUS:
    case OperatorType::MINUS:
    case OperatorType::STAR:
    case OperatorType::SLASH:
    case OperatorType::MOD:
        if (!leftType->isNumeric() || !rightType->isNumeric() || !leftType->equals(rightType)) {
            error(expr.line, expr.column, "arithmetic requires matching numeric operands");
            lastType_ = types_.getType(TypeKind::UNKNOWN);
            return;
        }
        lastType_ = leftType;
        return;

    default:
        error(expr.line, expr.column, "unsupported binary operator");
        lastType_ = types_.getType(TypeKind::UNKNOWN);
        return;
    }
}

void SemanticAnalyzer::visitUnaryExpr(UnaryExpr &expr) {
    expr.right->accept(*this);
    Type *rightType = lastType_;

    if (rightType->kind == TypeKind::UNKNOWN) {
        lastType_ = types_.getType(TypeKind::UNKNOWN);
        return;
    }

    if (expr.op == OperatorType::BANG) {
        if (rightType->kind != TypeKind::BOOL) {
            error(expr.line, expr.column, "'!' requires a bool operand");
            lastType_ = types_.getType(TypeKind::UNKNOWN);
            return;
        }
        lastType_ = types_.getType(TypeKind::BOOL);
        return;
    }

    if (expr.op == OperatorType::MINUS) {
        if (!rightType->isNumeric()) {
            error(expr.line, expr.column, "unary '-' requires a numeric operand");
            lastType_ = types_.getType(TypeKind::UNKNOWN);
            return;
        }
        lastType_ = rightType;
        return;
    }

    error(expr.line, expr.column, "unsupported unary operator");
    lastType_ = types_.getType(TypeKind::UNKNOWN);
}

void SemanticAnalyzer::visitAssignmentExpr(AssignmentExpr &expr) {
    expr.left->accept(*this);
    Type *targetType = lastType_;

    expr.value->accept(*this);
    Type *valueType = lastType_;

    if (targetType->kind != TypeKind::UNKNOWN && valueType->kind != TypeKind::UNKNOWN &&
        !targetType->equals(valueType)) {
        error(expr.line, expr.column, "cannot assign mismatched type");
    }

    lastType_ = targetType;
}

void SemanticAnalyzer::visitCallExpr(CallExpr &expr) {
    auto *varExpr = dynamic_cast<VariableExpr *>(expr.callee.get());
    if (!varExpr) {
        error(expr.line, expr.column, "callee is not a function name");
        lastType_ = types_.getType(TypeKind::UNKNOWN);
        return;
    }

    auto it = functions_.find(varExpr->name.lexeme_);
    if (it == functions_.end()) {
        error(expr.line, expr.column,
              "call to undeclared function '" + varExpr->name.lexeme_ + "'");
        lastType_ = types_.getType(TypeKind::UNKNOWN);
        return;
    }

    FuncDefn *fn = it->second;
    const auto &params = fn->proto.params;

    if (expr.arguments.size() != params.size()) {
        error(expr.line, expr.column,
              "expected " + std::to_string(params.size()) + " argument(s), got " +
                  std::to_string(expr.arguments.size()));
    }

    std::size_t checkCount = std::min(expr.arguments.size(), params.size());
    for (std::size_t i = 0; i < checkCount; ++i) {
        expr.arguments[i]->accept(*this);
        if (lastType_->kind != TypeKind::UNKNOWN && !lastType_->equals(params[i].type)) {
            error(expr.arguments[i]->line, expr.arguments[i]->column,
                  "argument " + std::to_string(i + 1) + " type mismatch");
        }
    }

    lastType_ = fn->proto.returnType;
}

// ---------- Statements ----------

void SemanticAnalyzer::visitExpressionStmt(ExpressionStmt &stmt) { stmt.expression->accept(*this); }

void SemanticAnalyzer::visitIfStmt(IfStmt &stmt) {
    stmt.condition->accept(*this);
    if (lastType_->kind != TypeKind::UNKNOWN && lastType_->kind != TypeKind::BOOL) {
        error(stmt.condition->line, stmt.condition->column, "if condition must be bool");
    }

    stmt.thenBranch->accept(*this);
    if (stmt.elseBranch) {
        stmt.elseBranch->accept(*this);
    }
}

void SemanticAnalyzer::visitWhileStmt(WhileStmt &stmt) {
    stmt.condition->accept(*this);
    if (lastType_->kind != TypeKind::UNKNOWN && lastType_->kind != TypeKind::BOOL) {
        error(stmt.condition->line, stmt.condition->column, "while condition must be bool");
    }

    stmt.body->accept(*this);
}

void SemanticAnalyzer::visitReturnStmt(ReturnStmt &stmt) {
    if (stmt.value) {
        stmt.value->accept(*this);
        if (lastType_->kind != TypeKind::UNKNOWN &&
            !lastType_->equals(currentFunctionReturnType_)) {
            error(stmt.line, stmt.column,
                  "return type does not match function's declared return type");
        }
    } else {
        if (currentFunctionReturnType_->kind != TypeKind::VOID) {
            error(stmt.line, stmt.column, "missing return value for non-void function");
        }
    }
}

void SemanticAnalyzer::visitBlockStmt(BlockStmt &stmt) {
    symbols_.beginScope();
    for (const auto &s : stmt.statements) {
        s->accept(*this);
    }
    symbols_.endScope();
}
void SemanticAnalyzer::visitVarDeclStmt(VarDeclStmt &stmt) {
    Type *initializerType = nullptr;
    if (stmt.initializer) {
        stmt.initializer->accept(*this);
        initializerType = lastType_;
    }

    if (initializerType && initializerType->kind != TypeKind::UNKNOWN) {
        if (!stmt.type->equals(initializerType)) {

            error(stmt.name.line_, stmt.name.column_, "Type mismatch in variable declaration.");
        }
    }

    auto sym = std::make_unique<Symbol>(Symbol{stmt.name.lexeme_, stmt.type, SymbolKind::VARIABLE,

                                               stmt.name.line_, stmt.name.column_});

    if (!symbols_.declare(std::move(sym))) {

        error(stmt.name.line_, stmt.name.column_,
              "Variable '" + stmt.name.lexeme_ + "' is already declared in this scope.");
    }
}