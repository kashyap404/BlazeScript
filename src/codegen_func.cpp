#include "codegen.h"
#include <iostream>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

llvm::Type* CodeGen::getLLVMType(Type* type) {
    if (!type)
        return llvm::Type::getVoidTy(context_);

    switch (type->kind) {
    case TypeKind::I32:
        return llvm::Type::getInt32Ty(context_);
    case TypeKind::I64:
        return llvm::Type::getInt64Ty(context_);
    case TypeKind::U32:
        return llvm::Type::getInt32Ty(context_);
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
    default:
        std::cerr << "codegen error: unsupported type mapping\n";
        return llvm::Type::getVoidTy(context_);
    }
}

void CodeGen::generateProgram(Program& program) {
    for (auto& fn : program.functions) {
        createFunction(*fn);
    }

    for (auto& fn : program.functions) {
        generateFunction(*fn);
    }

    std::string error;
    llvm::raw_string_ostream errorStream(error);
    if (llvm::verifyModule(*module_, &errorStream)) {
        std::cerr << "LLVM Verification Failed:\n" << errorStream.str() << "\n";
    }
}

llvm::Function* CodeGen::createFunction(FuncDefn& fn) {
    llvm::Type* retType = getLLVMType(fn.proto.returnType);

    std::vector<llvm::Type*> paramTypes;
    for (const auto& p : fn.proto.params) {
        paramTypes.push_back(getLLVMType(p.type));
    }

    llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);
    llvm::Function* function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
                                                      fn.proto.name.lexeme_, module_.get());

    unsigned idx = 0;
    for (auto& arg : function->args()) {
        arg.setName(fn.proto.params[idx++].name.lexeme_);
    }

    return function;
}

void CodeGen::generateFunction(FuncDefn& fn) {
    llvm::Function* function = module_->getFunction(fn.proto.name.lexeme_);
    if (!function) {
        std::cerr << "codegen error: function not found during generation\n";
        return;
    }

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context_, "entry", function);
    builder_.SetInsertPoint(entry);

    values_.clear();

    for (auto& arg : function->args()) {
        llvm::AllocaInst* alloca = builder_.CreateAlloca(arg.getType(), nullptr, arg.getName());
        builder_.CreateStore(&arg, alloca);
        values_[std::string(arg.getName())] = alloca;
    }

    fn.body->accept(*this);

    if (!builder_.GetInsertBlock()->getTerminator()) {
        if (function->getReturnType()->isVoidTy()) {
            builder_.CreateRetVoid();
        } else {
            builder_.CreateRet(llvm::Constant::getNullValue(function->getReturnType()));
        }
    }
}