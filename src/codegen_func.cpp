#include "codegen.h"

#include <iostream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

llvm::Type* CodeGen::toLLVMType(Type* type) {
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

void CodeGen::generate(Program& program) {
    // First create all function declarations.
    for (auto& fn : program.functions) {
        llvm::Type* retType = toLLVMType(fn->proto.returnType);

        std::vector<llvm::Type*> paramTypes;
        for (const auto& p : fn->proto.params) {
            paramTypes.push_back(toLLVMType(p.type));
        }

        llvm::FunctionType* funcType =
            llvm::FunctionType::get(retType, paramTypes, false);

        llvm::Function* function =
            llvm::Function::Create(
                funcType,
                llvm::Function::ExternalLinkage,
                fn->proto.name.lexeme_,
                module_.get());

        unsigned idx = 0;
        for (auto& arg : function->args()) {
            arg.setName(fn->proto.params[idx++].name.lexeme_);
        }
    }

    // Then generate the body of every function.
    for (auto& fn : program.functions) {
        generateFunction(*fn);
    }

    // Verify the generated LLVM module.
    std::string error;
    llvm::raw_string_ostream errorStream(error);

    if (llvm::verifyModule(*module_, &errorStream)) {
        std::cerr << "LLVM Verification Failed:\n"
                  << errorStream.str() << "\n";
    }
}

void CodeGen::generateFunction(FuncDefn& fn) {
    llvm::Function* function =
        module_->getFunction(fn.proto.name.lexeme_);

    if (!function) {
        std::cerr << "codegen error: function not found during generation\n";
        return;
    }

    currentFunction_ = function;

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(context_, "entry", function);

    builder_.SetInsertPoint(entry);

    values_.clear();

    // Store function parameters into allocas.
    for (auto& arg : function->args()) {
        llvm::AllocaInst* alloca =
            builder_.CreateAlloca(arg.getType(), nullptr, arg.getName());

        builder_.CreateStore(&arg, alloca);

        values_[std::string(arg.getName())] = alloca;
    }

    // Generate the function body.
    fn.body->accept(*this);

    // Add a default return if the function body didn't terminate.
    if (!builder_.GetInsertBlock()->getTerminator()) {
        if (function->getReturnType()->isVoidTy()) {
            builder_.CreateRetVoid();
        } else {
            builder_.CreateRet(
                llvm::Constant::getNullValue(function->getReturnType()));
        }
    }

    currentFunction_ = nullptr;
}