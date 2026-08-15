#include "ast_printer.h"
#include <iostream>

void AstPrinter::indent() {
    for (int i = 0; i < depth_; ++i) {
        *out_ << "|   ";
    }
}

void AstPrinter::emit(const char* text) {
    indent();
    *out_ << text << '\n';
}

void AstPrinter::emitOperator(OperatorType op) {
    indent();
    switch (op) {
    case OperatorType::PLUS:
        *out_ << '+';
        break;
    case OperatorType::MINUS:
        *out_ << '-';
        break;
    case OperatorType::STAR:
        *out_ << '*';
        break;
    case OperatorType::SLASH:
        *out_ << '/';
        break;
    case OperatorType::BANG:
        *out_ << '!';
        break;
    case OperatorType::AND:
        *out_ << "and";
        break;
    case OperatorType::OR:
        *out_ << "or";
        break;
    case OperatorType::EQUAL_EQUAL:
        *out_ << "==";
        break;
    case OperatorType::BANG_EQUAL:
        *out_ << "!=";
        break;
    case OperatorType::GREATER:
        *out_ << '>';
        break;
    case OperatorType::GREATER_EQUAL:
        *out_ << ">=";
        break;
    case OperatorType::LESS:
        *out_ << '<';
        break;
    case OperatorType::LESS_EQUAL:
        *out_ << "<=";
        break;
    default:
        *out_ << '?';
        break;
    }
    *out_ << '\n';
}

void AstPrinter::print(Expr& expr, std::ostream& out) {
    out_ = &out;
    expr.accept(*this);
}

void AstPrinter::print(Stmt& stmt, std::ostream& out) {
    out_ = &out;
    stmt.accept(*this);
}

void AstPrinter::printFuncDefn(const FuncDefn& fn) {
    indent();
    *out_ << "Function " << fn.proto.name.lexeme_ << '\n';
    depth_++;
    if (!fn.proto.params.empty()) {
        emit("params");
        depth_++;
        for (const Parameter& p : fn.proto.params) {
            indent();
            *out_ << p.name.lexeme_ << '\n';
        }
        depth_--;
    }
    fn.body->accept(*this);
    depth_--;
}

void AstPrinter::print(const FuncDefn& fn, std::ostream& out) {
    out_ = &out;
    printFuncDefn(fn);
}

void AstPrinter::print(const Program& program, std::ostream& out) {
    out_ = &out;
    for (const auto& fn : program.functions) {
        printFuncDefn(*fn);
    }
    for (const auto& stmt : program.statements) {
        stmt->accept(*this);
    }
}

// expressions
void AstPrinter::visitLiteralExpr(LiteralExpr& expr) {
    indent();
    *out_ << "Literal ";

    if (std::holds_alternative<int>(expr.value)) {
        *out_ << std::get<int>(expr.value);
    } else if (std::holds_alternative<bool>(expr.value)) {
        *out_ << (std::get<bool>(expr.value) ? "true" : "false");
    } else if (std::holds_alternative<std::string>(expr.value)) {
        *out_ << "\"" << std::get<std::string>(expr.value) << "\"";
    }

    *out_ << '\n';
}

void AstPrinter::visitVariableExpr(VariableExpr& expr) {
    indent();
    *out_ << "Variable " << expr.name.lexeme_ << '\n';
}

void AstPrinter::visitBinaryExpr(BinaryExpr& expr) {
    emit("Binary");
    depth_++;
    emitOperator(expr.op);
    expr.left->accept(*this);
    expr.right->accept(*this);
    depth_--;
}

void AstPrinter::visitUnaryExpr(UnaryExpr& expr) {
    emit("Unary");
    depth_++;
    emitOperator(expr.op);
    expr.right->accept(*this);
    depth_--;
}

void AstPrinter::visitAssignmentExpr(AssignmentExpr& expr) {
    emit("Assign");
    depth_++;
    expr.left->accept(*this);
    expr.value->accept(*this);
    depth_--;
}

void AstPrinter::visitCallExpr(CallExpr& expr) {
    emit("Call");
    depth_++;
    expr.callee->accept(*this);
    for (const auto& arg : expr.arguments) {
        arg->accept(*this);
    }
    depth_--;
}

// statements

void AstPrinter::visitExpressionStmt(ExpressionStmt& stmt) {
    emit("ExpressionStmt");
    depth_++;
    stmt.expression->accept(*this);
    depth_--;
}

void AstPrinter::visitIfStmt(IfStmt& stmt) {
    emit("If");
    depth_++;

    emit("condition");
    depth_++;
    stmt.condition->accept(*this);
    depth_--;

    emit("then");
    depth_++;
    stmt.thenBranch->accept(*this);
    depth_--;

    if (stmt.elseBranch) {
        emit("else");
        depth_++;
        stmt.elseBranch->accept(*this);
        depth_--;
    }
    depth_--;
}

void AstPrinter::visitWhileStmt(WhileStmt& stmt) {
    emit("While");
    depth_++;

    emit("condition");
    depth_++;
    stmt.condition->accept(*this);
    depth_--;

    emit("body");
    depth_++;
    stmt.body->accept(*this);
    depth_--;

    depth_--;
}

void AstPrinter::visitReturnStmt(ReturnStmt& stmt) {
    emit("Return");
    if (stmt.value) {
        depth_++;
        stmt.value->accept(*this);
        depth_--;
    }
}

void AstPrinter::visitBlockStmt(BlockStmt& stmt) {
    emit("Block");
    depth_++;
    for (const auto& s : stmt.statements) {
        s->accept(*this);
    }
    depth_--;
}
void AstPrinter::visitVarDeclStmt(VarDeclStmt& stmt) {

    for (int i = 0; i < depth_; ++i) {
        std::cout << "  ";
    }

    std::cout << "VarDecl: " << stmt.name.lexeme_ << "\n";

    if (stmt.initializer) {
        depth_++;
        stmt.initializer->accept(*this);
        depth_--;
    }
}