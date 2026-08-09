#pragma once
#include <unordered_map>
#include <memory>

enum class TypeKind {
    I32, I64, U32, U64, F32, F64, BOOL, VOID, UNKNOWN
};

class Type {
public:
    TypeKind kind;

    Type(TypeKind k) : kind(k) {}
    virtual ~Type() = default;

    bool isInteger() const {
        return kind == TypeKind::I32 || kind == TypeKind::I64 || 
               kind == TypeKind::U32 || kind == TypeKind::U64;
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
};

class TypeTable {
private:
    std::unordered_map<TypeKind, std::unique_ptr<Type>> table;
public:
    TypeTable(); 
    Type* getType(TypeKind kind);
};