#pragma once

#include "visitor.h"
#include "parser.h"
#include "type.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <string>
#include <unordered_map>

class CodeGen : public Visitor {
private:
    llvm::LLVMContext context_;
    std::unique_ptr<llvm::Module> module_;
    llvm::IRBuilder<> builder_;
    std::unordered_map<std::string, llvm::AllocaInst*> values_;
    llvm::Value* lastValue_ = nullptr;

    llvm::AllocaInst* lookupValue(const std::string& name) const;
    llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* function,
                                             const std::string& name,
                                             llvm::Type* type);
    llvm::Type* toLLVMType(Type* type);

public:
    explicit CodeGen(const std::string& moduleName);
    
    void dump() const;
    void dumpToFile(const std::string& path) const;
    void optimize();

    void generate(const Program& program);
    void generateFunction(const FuncDefn& fn);

    // Expressions
    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitVariableExpr(VariableExpr& expr) override;
    void visitBinaryExpr(BinaryExpr& expr) override;
    void visitUnaryExpr(UnaryExpr& expr) override;
    void visitAssignmentExpr(AssignmentExpr& expr) override;
    void visitCallExpr(CallExpr& expr) override;
    void visitArrayLiteralExpr(ArrayLiteralExpr& expr) override;
    void visitIndexExpr(IndexExpr& expr) override;

    // Statements
    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitIfStmt(IfStmt& stmt) override;
    void visitWhileStmt(WhileStmt& stmt) override;
    void visitReturnStmt(ReturnStmt& stmt) override;
    void visitBlockStmt(BlockStmt& stmt) override;
    void visitVarDeclStmt(VarDeclStmt& stmt) override;
};