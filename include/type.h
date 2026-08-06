#pragma once
<<<<<<< HEAD

enum class TypeKind {
    I32,
    I64,
    F32,
    F64,
    BOOL,
    UNKNOWN
=======
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
>>>>>>> 4afa78904b5d0668a967c14be118a186884beacc
};

class Type {
public:
<<<<<<< HEAD
    TypeKind kind;

    Type(TypeKind k) : kind(k) {}
    virtual ~Type() = default;

    bool isInteger() const {
        return kind == TypeKind::I32 || kind == TypeKind::I64;
    }

    bool isFloat() const {
        return kind == TypeKind::F32 || kind == TypeKind::F64;
    }

    bool isNumeric() const {
        return isInteger() || isFloat();
    }

    bool equals(const Type* other) const {
        if (other == nullptr) return false;
        return this->kind == other->kind;
    }

    static Type* getI32() {
        static Type instance(TypeKind::I32);
        return &instance;
    }
    static Type* getI64() {
        static Type instance(TypeKind::I64);
        return &instance;
    }
    static Type* getF32() {
        static Type instance(TypeKind::F32);
        return &instance;
    }
    static Type* getF64() {
        static Type instance(TypeKind::F64);
        return &instance;
    }
    static Type* getBool() {
        static Type instance(TypeKind::BOOL);
        return &instance;
    }
    static Type* getUnknown() {
        static Type instance(TypeKind::UNKNOWN);
        return &instance;
    }
=======
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
>>>>>>> 4afa78904b5d0668a967c14be118a186884beacc
};