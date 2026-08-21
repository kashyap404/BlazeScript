#pragma once

#include "expr.h"
#include "stmt.h"
#include "type.h"
#include "visitor.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <map>
#include <memory>
#include <string>

class CodeGen : public Visitor {
public:
    CodeGen();

    void dump() const;

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

    llvm::Value* lastValue_ = nullptr;

    std::map<std::string, llvm::AllocaInst*> values_;
    llvm::AllocaInst* lookupValue(const std::string& name) const;

    llvm::Type* toLLVMType(Type* type);

    llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* function, llvm::Type* type,
                                             const std::string& varName);
};