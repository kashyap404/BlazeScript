#include "codegen.h"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>

#include <iostream>


llvm::AllocaInst* CodeGen::createEntryBlockAlloca(llvm::Function* function,
                                                  const std::string& varName, llvm::Type* type) {
    llvm::IRBuilder<> builder(&function->getEntryBlock(), function->getEntryBlock().begin());

    return builder.CreateAlloca(type, nullptr, varName);
}

void CodeGen::visitExpressionStmt(ExpressionStmt& stmt) { stmt.expression->accept(*this); }

void CodeGen::visitReturnStmt(ReturnStmt& stmt) {
    if (!stmt.value) {
        builder_.CreateRetVoid();
        return;
    }

    stmt.value->accept(*this);
    llvm::Value* retVal = lastValue_;

    if (!retVal) {
        std::cerr << "error: empty return value at line " << stmt.line << "\n";
        return;
    }

    builder_.CreateRet(retVal);
}

void CodeGen::visitVarDeclStmt(VarDeclStmt& stmt) {
    llvm::Type* varTy = toLLVMType(stmt.type);

    llvm::Function* currentFunction = builder_.GetInsertBlock()->getParent();
    llvm::AllocaInst* alloca = createEntryBlockAlloca(currentFunction, stmt.name.lexeme_, varTy);

    if (stmt.initializer) {
        stmt.initializer->accept(*this);
        llvm::Value* initVal = lastValue_;

        if (!initVal) {
            std::cerr << "error: initializer for '" << stmt.name.lexeme_
                      << "' didn't produce a value (line " << stmt.line << ")\n";
        } else {
            builder_.CreateStore(initVal, alloca);
        }
    }

    values_[stmt.name.lexeme_] = alloca;
}

void CodeGen::visitBlockStmt(BlockStmt& stmt) {
    for (auto& s : stmt.statements) {
        s->accept(*this);

        if (builder_.GetInsertBlock()->getTerminator()) {
            break;
        }
    }
}

void CodeGen::visitIfStmt(IfStmt& stmt) {
    stmt.condition->accept(*this);
    llvm::Value* condVal = lastValue_;

    if (!condVal) {
        std::cerr << "error: if condition didn't produce a value (line " << stmt.line << ")\n";
        return;
    }

    llvm::Function* function = builder_.GetInsertBlock()->getParent();

    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(context_, "then", function);

    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context_, "ifcont");

    llvm::BasicBlock* elseBB =
        stmt.elseBranch ? llvm::BasicBlock::Create(context_, "else") : mergeBB;

    builder_.CreateCondBr(condVal, thenBB, elseBB);

    builder_.SetInsertPoint(thenBB);
    stmt.thenBranch->accept(*this);

    if (!builder_.GetInsertBlock()->getTerminator()) {
        builder_.CreateBr(mergeBB);
    }

    if (stmt.elseBranch) {
        function->insert(function->end(), elseBB);

        builder_.SetInsertPoint(elseBB);
        stmt.elseBranch->accept(*this);

        if (!builder_.GetInsertBlock()->getTerminator()) {
            builder_.CreateBr(mergeBB);
        }
    }

    function->insert(function->end(), mergeBB);
    builder_.SetInsertPoint(mergeBB);
}

void CodeGen::visitWhileStmt(WhileStmt& stmt) {
    llvm::Function* function = builder_.GetInsertBlock()->getParent();

    llvm::BasicBlock* condBB = llvm::BasicBlock::Create(context_, "whilecond", function);

    llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(context_, "whilebody");

    llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(context_, "whileend");

    builder_.CreateBr(condBB);

    builder_.SetInsertPoint(condBB);

    stmt.condition->accept(*this);
    llvm::Value* condVal = lastValue_;

    if (!condVal) {
        std::cerr << "error: while condition didn't produce a value (line " << stmt.line << ")\n";
        return;
    }

    builder_.CreateCondBr(condVal, loopBB, afterBB);

    function->insert(function->end(), loopBB);

    builder_.SetInsertPoint(loopBB);
    stmt.body->accept(*this);

    if (!builder_.GetInsertBlock()->getTerminator()) {
        builder_.CreateBr(condBB);
    }

    function->insert(function->end(), afterBB);

    builder_.SetInsertPoint(afterBB);
}