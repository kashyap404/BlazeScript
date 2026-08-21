#include "codegen.h"

#include <llvm/IR/Constants.h>

#include <iostream>

llvm::Type* CodeGen::toLLVMType(Type* type) {
    if (!type) {
        std::cerr << "warning: missing type in declaration, defaulting to i32\n";
        return llvm::Type::getInt32Ty(context_);
    }

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
    case TypeKind::UNKNOWN:
    default:
        std::cerr << "error: unknown type, defaulting to i32\n";
        return llvm::Type::getInt32Ty(context_);
    }
}

llvm::AllocaInst* CodeGen::createEntryBlockAlloca(llvm::Function* function, llvm::Type* type,
                                                  const std::string& varName) {
    llvm::IRBuilder<> entryBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
    return entryBuilder.CreateAlloca(type, nullptr, varName);
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
    llvm::AllocaInst* alloca = createEntryBlockAlloca(currentFunction, varTy, stmt.name.lexeme_);

    if (stmt.initializer) {
        stmt.initializer->accept(*this);
        llvm::Value* initVal = lastValue_;

        if (!initVal) {
            std::cerr << "error: null initializer for '" << stmt.name.lexeme_ << "' at line "
                      << stmt.line << "\n";
        } else {
            builder_.CreateStore(initVal, alloca);
        }
    }

    values_[stmt.name.lexeme_] = alloca;
}