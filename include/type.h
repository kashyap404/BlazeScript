#pragma once
#include <vector>
#include <memory>

enum class TypeKind {
    I32, I64, U32, U64,
    F32, F64,
    BOOL,
    STRING,
    VOID,
    NIL,
    FUNCTION,
    UNKNOWN     
};

class Type {
public:
    explicit Type(TypeKind kind) : kind(kind) {}
    virtual ~Type() = default;

    TypeKind kind;

    bool isInteger() const;
    bool isFloat() const;
    bool isNumeric() const;
    bool equals(const Type* other) const;

    static Type* getI32();
    static Type* getI64();
    static Type* getU32();
    static Type* getU64();
    static Type* getF32();
    static Type* getF64();
    static Type* getBool();
    static Type* getString();
    static Type* getVoid();
    static Type* getNil();
    static Type* getUnknown();
};

class FunctionType : public Type {
public:
    FunctionType(std::vector<Type*> paramTypes, Type* returnType)
        : Type(TypeKind::FUNCTION),
          paramTypes(std::move(paramTypes)),
          returnType(returnType) {}

    std::vector<Type*> paramTypes;
    Type* returnType;
};

class TypeArena {
public:
    FunctionType* makeFunctionType(std::vector<Type*> paramTypes, Type* returnType);

private:
    std::vector<std::unique_ptr<FunctionType>> owned_;
};