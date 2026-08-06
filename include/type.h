#pragma once

enum class TypeKind {
    I32,
    I64,
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
};