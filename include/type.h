#pragma once
#include <memory>
#include <string>
#include <unordered_map>

enum class TypeKind { I32, I64, U32, U64, F32, F64, BOOL, VOID, UNKNOWN, ARRAY };

class Type {
public:
    TypeKind kind;
    Type(TypeKind k) : kind(k) {}
    virtual ~Type() = default;

    virtual bool equals(const Type* other) const {
        if (other == nullptr) return false;
        return this->kind == other->kind;
    }
    virtual std::string toString() const { return "unknown"; }

    bool isInteger() const {
        return kind == TypeKind::I32 || kind == TypeKind::I64 || kind == TypeKind::U32 ||
               kind == TypeKind::U64;
    }
    bool isFloat() const { return kind == TypeKind::F32 || kind == TypeKind::F64; }
    bool isNumeric() const { return isInteger() || isFloat(); }
};

class PrimitiveType : public Type {
public:
    PrimitiveType(TypeKind k) : Type(k) {}
    std::string toString() const override {
        switch (kind) {
            case TypeKind::I32: return "i32";
            case TypeKind::I64: return "i64";
            case TypeKind::U32: return "u32";
            case TypeKind::U64: return "u64";
            case TypeKind::F32: return "f32";
            case TypeKind::F64: return "f64";
            case TypeKind::BOOL: return "bool";
            case TypeKind::VOID: return "void";
            case TypeKind::UNKNOWN: return "unknown";
            default: return "unknown";
        }
    }
};

class ArrayType : public Type {
public:
    Type* elementType;
    size_t size;

    ArrayType(Type* elem, size_t sz) : Type(TypeKind::ARRAY), elementType(elem), size(sz) {}

    bool equals(const Type* other) const override {
        if (!other || other->kind != TypeKind::ARRAY) return false;
        auto* arr = static_cast<const ArrayType*>(other);
        return size == arr->size && elementType->equals(arr->elementType);
    }

    std::string toString() const override {
        return "[" + elementType->toString() + "; " + std::to_string(size) + "]";
    }
};

class TypeTable {
private:
    std::unordered_map<TypeKind, std::unique_ptr<Type>> table;

public:
    TypeTable();
    Type* getType(TypeKind kind);
    ArrayType* getArrayType(Type* elem, size_t size);
};