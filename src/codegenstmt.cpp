#include "codegen.h"
#include <iostream>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>

llvm::Type* CodeGen::toLLVMType(Type* type) {
    if (!type)
        return llvm::Type::getVoidTy(context_);

    switch (type->kind) {
    case TypeKind::I32:
    case TypeKind::U32:
        return llvm::Type::getInt32Ty(context_);
    case TypeKind::I64:
    case TypeKind::U64:
        return llvm::Type::getInt64Ty(context_);
    case TypeKind::F32:
        return llvm::Type::getFloatTy(context_);
    case TypeKind::F64:
        return llvm::Type::getDoubleTy(context_);
    case TypeKind::BOOL:
        return llvm::Type::getInt1Ty(context_);
    case TypeKind::VOID:
        return llvm::Type::getVoidTy(context_);
    case TypeKind::ARRAY: {
        auto* arrType = static_cast<ArrayType*>(type);
        llvm::Type* elemTy = toLLVMType(arrType->elementType);
        return llvm::ArrayType::get(elemTy, arrType->size);
    }
    default:
        std::cerr << "codegen error: unknown type\n";
        return llvm::Type::getInt32Ty(context_);
    }
}

llvm::AllocaInst* CodeGen::createEntryBlockAlloca(llvm::Function* function, const std::string& name,
                                                  llvm::Type* type) {
    llvm::IRBuilder<> tmpB(&function->getEntryBlock(), function->getEntryBlock().begin());
    return tmpB.CreateAlloca(type, nullptr, name);
}

void CodeGen::visitExpressionStmt(ExpressionStmt& stmt) { stmt.expression->accept(*this); }

void CodeGen::visitVarDeclStmt(VarDeclStmt& stmt) {
    llvm::Function* function = builder_.GetInsertBlock()->getParent();
    llvm::Type* varTy = toLLVMType(stmt.type);
    llvm::AllocaInst* alloca = createEntryBlockAlloca(function, stmt.name.lexeme_, varTy);
    values_[stmt.name.lexeme_] = alloca;

    if (stmt.initializer) {
        stmt.initializer->accept(*this);
        if (lastValue_) {
            builder_.CreateStore(lastValue_, alloca);
        }
    }
}

void CodeGen::visitIfStmt(IfStmt& stmt) {
    stmt.condition->accept(*this);
    llvm::Value* condVal = lastValue_;
    if (!condVal)
        return;

    if (!condVal->getType()->isIntegerTy(1)) {
        condVal = builder_.CreateICmpNE(condVal, llvm::Constant::getNullValue(condVal->getType()),
                                        "ifcond");
    }

    llvm::Function* function = builder_.GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(context_, "then", function);
    llvm::BasicBlock* elseBB =
        stmt.elseBranch ? llvm::BasicBlock::Create(context_, "else") : nullptr;
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context_, "ifcont");

    builder_.CreateCondBr(condVal, thenBB, elseBB ? elseBB : mergeBB);

    builder_.SetInsertPoint(thenBB);
    stmt.thenBranch->accept(*this);
    if (!builder_.GetInsertBlock()->getTerminator()) {
        builder_.CreateBr(mergeBB);
    }

    if (elseBB) {
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
    llvm::BasicBlock* bodyBB = llvm::BasicBlock::Create(context_, "whilebody");
    llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(context_, "whileafter");

    builder_.CreateBr(condBB);

    builder_.SetInsertPoint(condBB);
    stmt.condition->accept(*this);
    llvm::Value* condVal = lastValue_;
    if (!condVal)
        return;
    if (!condVal->getType()->isIntegerTy(1)) {
        condVal = builder_.CreateICmpNE(condVal, llvm::Constant::getNullValue(condVal->getType()),
                                        "whilecond");
    }
    builder_.CreateCondBr(condVal, bodyBB, afterBB);

    function->insert(function->end(), bodyBB);
    builder_.SetInsertPoint(bodyBB);
    stmt.body->accept(*this);
    if (!builder_.GetInsertBlock()->getTerminator()) {
        builder_.CreateBr(condBB);
    }

    function->insert(function->end(), afterBB);
    builder_.SetInsertPoint(afterBB);
}

void CodeGen::visitReturnStmt(ReturnStmt& stmt) {
    if (stmt.value) {
        stmt.value->accept(*this);
        if (lastValue_) {
            builder_.CreateRet(lastValue_);
        }
    } else {
        builder_.CreateRetVoid();
    }
}

void CodeGen::visitBlockStmt(BlockStmt& stmt) {
    for (const auto& s : stmt.statements) {
        s->accept(*this);
        if (builder_.GetInsertBlock()->getTerminator())
            break;
    }
}