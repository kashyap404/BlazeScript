#include "codegen.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <variant>

CodeGen::CodeGen()
    : module_(std::make_unique<llvm::Module>("blazescript", context_)), builder_(context_) {}

void CodeGen::dump() const { module_->print(llvm::outs(), nullptr); }

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
        lastValue_ = builder_.CreateNeg(operand, "negtmp");
        break;

    case OperatorType::BANG:
        lastValue_ = builder_.CreateICmpEQ(
            operand, llvm::Constant::getNullValue(operand->getType()), "nottmp");
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
        lastValue_ = builder_.CreateAdd(left, right, "addtmp");
        break;

    case OperatorType::MINUS:
        lastValue_ = builder_.CreateSub(left, right, "subtmp");
        break;

    case OperatorType::STAR:
        lastValue_ = builder_.CreateMul(left, right, "multmp");
        break;

    case OperatorType::SLASH:
        lastValue_ = builder_.CreateSDiv(left, right, "divtmp");
        break;

    case OperatorType::EQUAL_EQUAL:
        lastValue_ = builder_.CreateICmpEQ(left, right, "eqtmp");
        break;

    case OperatorType::BANG_EQUAL:
        lastValue_ = builder_.CreateICmpNE(left, right, "netmp");
        break;

    case OperatorType::LESS:
        lastValue_ = builder_.CreateICmpSLT(left, right, "lttmp");
        break;

    case OperatorType::LESS_EQUAL:
        lastValue_ = builder_.CreateICmpSLE(left, right, "letmp");
        break;

    case OperatorType::GREATER:
        lastValue_ = builder_.CreateICmpSGT(left, right, "gttmp");
        break;

    case OperatorType::GREATER_EQUAL:
        lastValue_ = builder_.CreateICmpSGE(left, right, "getmp");
        break;

    case OperatorType::AND:
        lastValue_ = builder_.CreateAnd(left, right, "andtmp");
        break;

    case OperatorType::OR:
        lastValue_ = builder_.CreateOr(left, right, "ortmp");
        break;

    default:
        lastValue_ = nullptr;
        break;
    }
}