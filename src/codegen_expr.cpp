#include "codegen.h"
#include <fstream>
#include <iostream>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <variant>
#include <vector>

CodeGen::CodeGen()
    : module_(std::make_unique<llvm::Module>("blazescript", context_)), builder_(context_) {}

void CodeGen::dump() const { module_->print(llvm::outs(), nullptr); }

void CodeGen::dumpToFile(const std::string& path) const {
    std::error_code EC;
    llvm::raw_fd_ostream dest(path, EC, llvm::sys::fs::OF_None);
    if (EC) {
        std::cerr << "Could not open file '" << path << "': " << EC.message() << "\n";
        return;
    }
    module_->print(dest, nullptr);
}

llvm::AllocaInst* CodeGen::lookupValue(const std::string& name) const {
    auto it = values_.find(name);

    if (it != values_.end()) {
        return it->second;
    }

    return nullptr;
}

void CodeGen::visitLiteralExpr(LiteralExpr& expr) {
    if (std::holds_alternative<int>(expr.value)) {
        int value = std::get<int>(expr.value);

        lastValue_ = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context_), value, true);
    } else if (std::holds_alternative<bool>(expr.value)) {
        bool value = std::get<bool>(expr.value);

        lastValue_ = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context_), value);
    } else {
        std::cerr << "codegen error: unsupported literal type\n";
        lastValue_ = nullptr;
    }
}

void CodeGen::visitVariableExpr(VariableExpr& expr) {
    llvm::AllocaInst* alloca = lookupValue(expr.name.lexeme_);

    if (!alloca) {
        std::cerr << "codegen error: undefined variable '" << expr.name.lexeme_ << "'\n";
        lastValue_ = nullptr;
        return;
    }

    lastValue_ = builder_.CreateLoad(alloca->getAllocatedType(), alloca, expr.name.lexeme_);
}

void CodeGen::visitUnaryExpr(UnaryExpr& expr) {
    expr.right->accept(*this);

    llvm::Value* operand = lastValue_;

    if (!operand) {
        lastValue_ = nullptr;
        return;
    }

    switch (expr.op) {
    case OperatorType::MINUS:
        lastValue_ = builder_.CreateNeg(operand, "neg");
        break;

    case OperatorType::BANG:
        lastValue_ =
            builder_.CreateICmpEQ(operand, llvm::Constant::getNullValue(operand->getType()), "not");
        break;

    default:
        lastValue_ = nullptr;
        break;
    }
}

void CodeGen::visitBinaryExpr(BinaryExpr& expr) {
    expr.left->accept(*this);
    llvm::Value* left = lastValue_;

    expr.right->accept(*this);
    llvm::Value* right = lastValue_;

    if (!left || !right) {
        lastValue_ = nullptr;
        return;
    }

    switch (expr.op) {
    case OperatorType::PLUS:
        lastValue_ = builder_.CreateAdd(left, right, "add");
        break;

    case OperatorType::MINUS:
        lastValue_ = builder_.CreateSub(left, right, "sub");
        break;

    case OperatorType::STAR:
        lastValue_ = builder_.CreateMul(left, right, "mul");
        break;

    case OperatorType::SLASH:
        lastValue_ = builder_.CreateSDiv(left, right, "div");
        break;

    case OperatorType::EQUAL_EQUAL:
        lastValue_ = builder_.CreateICmpEQ(left, right, "eq");
        break;

    case OperatorType::BANG_EQUAL:
        lastValue_ = builder_.CreateICmpNE(left, right, "neq");
        break;

    case OperatorType::LESS:
        lastValue_ = builder_.CreateICmpSLT(left, right, "lt");
        break;

    case OperatorType::LESS_EQUAL:
        lastValue_ = builder_.CreateICmpSLE(left, right, "le");
        break;

    case OperatorType::GREATER:
        lastValue_ = builder_.CreateICmpSGT(left, right, "gt");
        break;

    case OperatorType::GREATER_EQUAL:
        lastValue_ = builder_.CreateICmpSGE(left, right, "ge");
        break;

    case OperatorType::AND:
        lastValue_ = builder_.CreateAnd(left, right, "and");
        break;

    case OperatorType::OR:
        lastValue_ = builder_.CreateOr(left, right, "or");
        break;

    default:
        lastValue_ = nullptr;
        break;
    }
}

void CodeGen::visitAssignmentExpr(AssignmentExpr& expr) {
    auto* variable = dynamic_cast<VariableExpr*>(expr.left.get());

    if (!variable) {
        std::cerr << "codegen error: assignment target is not a variable\n";
        lastValue_ = nullptr;
        return;
    }

    llvm::AllocaInst* alloca = lookupValue(variable->name.lexeme_);

    if (!alloca) {
        std::cerr << "codegen error: undefined variable '" << variable->name.lexeme_ << "'\n";
        lastValue_ = nullptr;
        return;
    }

    expr.value->accept(*this);

    llvm::Value* value = lastValue_;

    if (!value) {
        lastValue_ = nullptr;
        return;
    }

    builder_.CreateStore(value, alloca);
    lastValue_ = value;
}

void CodeGen::visitCallExpr(CallExpr& expr) {
    auto* variable = dynamic_cast<VariableExpr*>(expr.callee.get());

    if (!variable) {
        std::cerr << "codegen error: unsupported callee\n";
        lastValue_ = nullptr;
        return;
    }

    llvm::Function* callee = module_->getFunction(variable->name.lexeme_);

    if (!callee) {
        std::cerr << "codegen error: unknown function '" << variable->name.lexeme_ << "'\n";
        lastValue_ = nullptr;
        return;
    }

    std::vector<llvm::Value*> args;

    for (auto& argument : expr.arguments) {
        argument->accept(*this);

        llvm::Value* value = lastValue_;

        if (!value) {
            lastValue_ = nullptr;
            return;
        }

        args.push_back(value);
    }

    if (args.size() != callee->arg_size()) {
        std::cerr << "codegen error: incorrect number of arguments for '" << variable->name.lexeme_
                  << "'\n";
        lastValue_ = nullptr;
        return;
    }

    lastValue_ = builder_.CreateCall(callee, args, "call");
}