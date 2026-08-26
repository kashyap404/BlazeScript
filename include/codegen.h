#pragma once
#include "expr.h"
#include "function.h"
#include "parser.h"
#include "stmt.h"
#include "type.h"
#include "visitor.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>
#include <unordered_map>

class CodeGen : public Visitor {
public:
    CodeGen();
    void generate(Program& program);
    void dump() const;
    void dumpToFile(const std::string& path) const;

    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitVariableExpr(VariableExpr& expr) override;
    void visitBinaryExpr(BinaryExpr& expr) override;
    void visitUnaryExpr(UnaryExpr& expr) override;
    void visitAssignmentExpr(AssignmentExpr& expr) override;
    void visitCallExpr(CallExpr& expr) override;

    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitIfStmt(IfStmt& stmt) override;
    void visitWhileStmt(WhileStmt& stmt) override;
    void visitReturnStmt(ReturnStmt& stmt) override;
    void visitBlockStmt(BlockStmt& stmt) override;
    void visitVarDeclStmt(VarDeclStmt& stmt) override;

private:
    llvm::LLVMContext context_;
    std::unique_ptr<llvm::Module> module_;
    llvm::IRBuilder<> builder_;
    llvm::Function* currentFunction_ = nullptr;
    std::unordered_map<std::string, llvm::AllocaInst*> values_;
    llvm::Value* lastValue_ = nullptr;
    llvm::Type* toLLVMType(Type* type);

    llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* fn, const std::string& name,
                                             llvm::Type* type);

    llvm::AllocaInst* lookupValue(const std::string& name) const;

    void generateFunction(FuncDefn& fn);

    void error(int line, int column, const std::string& message);

    bool hadError_ = false;
};