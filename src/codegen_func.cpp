#include "codegen.h"
#include <iostream>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>

void CodeGen::generate(const Program& program) {
    for (const auto& fn : program.functions) {
        std::vector<llvm::Type*> paramTypes;
        for (const auto& p : fn->proto.params) {
            paramTypes.push_back(toLLVMType(p.type));
        }
        llvm::Type* retTy = toLLVMType(fn->proto.returnType);
        llvm::FunctionType* fnTy = llvm::FunctionType::get(retTy, paramTypes, false);
        llvm::Function* function = llvm::Function::Create(fnTy, llvm::Function::ExternalLinkage,
                                                          fn->proto.name.lexeme_, module_.get());

        unsigned idx = 0;
        for (auto& arg : function->args()) {
            arg.setName(fn->proto.params[idx++].name.lexeme_);
        }
    }
    for (const auto& fn : program.functions) {
        generateFunction(*fn);
    }
    if (!program.statements.empty()) {
        llvm::FunctionType* mainTy =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(context_), false);
        llvm::Function* mainFn = llvm::Function::Create(mainTy, llvm::Function::ExternalLinkage,
                                                        "__blaze_main", module_.get());
        llvm::BasicBlock* entry = llvm::BasicBlock::Create(context_, "entry", mainFn);
        builder_.SetInsertPoint(entry);

        for (const auto& stmt : program.statements) {
            stmt->accept(*this);
        }

        if (!entry->getTerminator()) {
            builder_.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context_), 0));
        }
    }
}

void CodeGen::generateFunction(const FuncDefn& fn) {
    llvm::Function* function = module_->getFunction(fn.proto.name.lexeme_);
    if (!function) {
        std::cerr << "codegen error: function '" << fn.proto.name.lexeme_ << "' not found\n";
        return;
    }

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context_, "entry", function);
    builder_.SetInsertPoint(entry);

    for (auto& arg : function->args()) {
        const Parameter& param = fn.proto.params[arg.getArgNo()];
        llvm::AllocaInst* alloca =
            createEntryBlockAlloca(function, param.name.lexeme_, arg.getType());
        builder_.CreateStore(&arg, alloca);
        values_[param.name.lexeme_] = alloca;
    }

    fn.body->accept(*this);

    if (!entry->getTerminator()) {
        if (function->getReturnType()->isVoidTy()) {
            builder_.CreateRetVoid();
        } else {
            builder_.CreateRet(llvm::Constant::getNullValue(function->getReturnType()));
        }
    }
    std::string err;
    llvm::raw_string_ostream errStream(err);
    if (llvm::verifyFunction(*function, &errStream)) {
        std::cerr << "codegen error: verification failed for '" << fn.proto.name.lexeme_ << "'\n"
                  << errStream.str() << "\n";
    }

    values_.clear();
}