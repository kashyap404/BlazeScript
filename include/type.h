#pragma once
#include <unordered_map>
#include <memory>

enum class TypeKind {
    I32,
    I64,
    U32,
    U64,
    F32,
    F64,
    BOOL,
    UNKNOWN
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
    TypeTable() {
        table[TypeKind::I32] = std::make_unique<Type>(TypeKind::I32);
        table[TypeKind::I64] = std::make_unique<Type>(TypeKind::I64);
        table[TypeKind::U32] = std::make_unique<Type>(TypeKind::U32);
        table[TypeKind::U64] = std::make_unique<Type>(TypeKind::U64);
        table[TypeKind::F32] = std::make_unique<Type>(TypeKind::F32);
        table[TypeKind::F64] = std::make_unique<Type>(TypeKind::F64);
        table[TypeKind::BOOL] = std::make_unique<Type>(TypeKind::BOOL);
        table[TypeKind::UNKNOWN] = std::make_unique<Type>(TypeKind::UNKNOWN);
    }

    Type* getType(TypeKind kind) {
        return table[kind].get();
    }

};
 
